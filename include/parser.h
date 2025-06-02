#ifndef VORTEX_PARSER_H
#define VORTEX_PARSER_H

#include <iostream>
#include <istream>

#include "collections/hash_map.hpp"
#include "collections/string.h"
#include "error.h"
#include "instructions/instructions.h"
#include "value.h"
#include "vm.h"

/// An abstraction, used to encapsulate and simplify the parsing of instruction
/// arguments. The `expect` methods provide convenient abstraction over the data
/// validation.
class AsmReader {
       private:
        const Context &ctx;
        const Vector<String> &args;
        const HashMap<String, size_t> &labels;

        unsigned readPos = 0;

        String expectArg();

       public:
        AsmReader(const Context &, const Vector<String> &, const HashMap<String, size_t> &);

        Register expectRegister();
        Literal expectLiteral();
        Box<Value> expectValue();
        size_t expectLabelLocation();
        /// Used to signal that the instruction does not take any more arguments
        /// and that there should not be more passed.
        void expectEndOfArgs();
};

/// Used to encapsulate the heart of the parser - the instruction factory map.
/// When going over the instructions, the parser uses this map to check which
/// method it needs to call in order to create the appropriate `Instruction`
/// object. If no method is found, then the given instruction is invalid.
class InstructionFactory {
       public:
        using Method = Box<Instruction> (*)(AsmReader);
        using Map = HashMap<String, Method>;

       private:
        static Map GLOBAL_INSTRUCTION_FACTORY;

       public:
        // Flyweight design pattern - there is no need to create separate
        // instances of this global instruction factory for each individual
        // parser instance since the syntax of the language remains the same.
        static const Map &getGlobalInstructionFactory();
};

/// The component of the language, which takes in the user script and converts
/// it into an array of native `Instruction` objects, which are then fed to the
/// `VM`.
class Parser {
       private:
        /// The parser uses a 2-walk approach to parsing each script. First, it
        /// goes over the source code, builds its `labels` map and just maps out
        /// the raw signatures of the instructions. Then on the second walk, it
        /// turns these raw instructions into the actual `Instruction` objects,
        /// while performing the linking of any jump instructions.
        ///
        /// This approach allows for jumping to future defined labels, which
        /// greatly eases the user in most cases.
        struct RawInstruction {
                String name;
                Vector<String> args;
                Context ctx;
        };

       private:
        HashMap<String, size_t> labels;
        const InstructionFactory::Map &instructionFactory;

        void parseLabel(const String &, const Context &, size_t);
        RawInstruction parseInstruction(const String &, const Context &);

        /// The first walk of the parsing process, populating the `labels` map
        /// and parsing the file into a sequence of raw instructions.
        Vector<RawInstruction> parseFileContents(std::istream &, Context &);
        /// The second walk over the program, which turns the raw instructions
        /// into the result `Instruction` objects and dynamically links the
        /// instructions to the labels.
        Vector<Box<Instruction>> linkInstructions(const Vector<RawInstruction> &);

       public:
        Parser();

        /// Reads the source file and turns it into a sequence of program
        /// `Instruction` objects.
        Vector<Box<Instruction>> parseFile(const String &filename);
        /// Used to find and determine the entrypoint of the program.
        const HashMap<String, size_t> &getLabels() const;
};

#endif
