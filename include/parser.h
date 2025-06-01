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
        void expectEndOfArgs();
};

/// Could also be named `InstructionFactory`, since that's the design pattern it
/// implements, but `Parser` is the more commonly used name in the context of
/// compilers/interpreters.
class Parser {
       private:
        using InstructionFactoryMethod = Box<Instruction> (*)(AsmReader);
        using InstructionFactory = HashMap<String, InstructionFactoryMethod>;

        struct RawInstruction {
                String name;
                Vector<String> args;
                Context ctx;
        };

       private:
        static InstructionFactory GLOBAL_INSTRUCTION_FACTORY;

       private:
        HashMap<String, size_t> labels;
        const InstructionFactory &instructionFactory;

        // Flyweight design pattern - no need to create separate instances of
        // this global instruction factory for each individual parser instance,
        // since the syntax of the language remains the same.
        static const InstructionFactory &getGlobalInstructionFactory();

        void parseLabel(const String &, const Context &, size_t);
        RawInstruction parseInstruction(const String &, const Context &);
        Vector<RawInstruction> parseFileContents(std::istream &, Context &);
        Vector<Box<Instruction>> linkInstructions(const Vector<RawInstruction> &);

       public:
        Parser();

        Vector<Box<Instruction>> parseFile(const String &filename);
        const HashMap<String, size_t> &getLabels() const;
};

#endif
