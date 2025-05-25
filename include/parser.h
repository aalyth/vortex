#ifndef VORTEX_PARSER_H
#define VORTEX_PARSER_H

#include "collections/hash_map.hpp"
#include "collections/string.h"
#include "error.h"
#include "instructions.h"
#include "value.h"
#include "vm.h"

class AsmVisitor {
       private:
        const Context &ctx;
        const Vector<String> &args;
        const HashMap<String, size_t> &labels;

        unsigned readPos = 0;

        String expectArg() {
                if (readPos >= args.length()) {
                        throw ExpectedArgumentException(ctx);
                }
                return args[readPos++].unwrap();
        }

       public:
        AsmVisitor(const Context &_cxt, const Vector<String> &_args,
                   const HashMap<String, size_t> &_labels)
            : ctx(_cxt), args(_args), labels(_labels) {
        }

        Register expectRegister() {
                const String str = expectArg();
                if (!str.startsWith('r')) {
                        throw ExpectedRegisterException(ctx, str);
                }
                const char *regStr = str.cStr() + 1;
                try {
                        const size_t reg = atou(regStr);
                        return Register(ctx, reg);
                } catch (const std::invalid_argument &) {
                        throw InvalidRegisterException(ctx, regStr);
                }
        }

        Literal expectLiteral() {
                const String str = expectArg();
                try {
                        return Literal(atoi64(str.cStr()));
                } catch (const std::invalid_argument &) {
                        throw ExpectedLiteralException(ctx, str);
                }
        }

        Value *expectValue() {
                const String str = expectArg();
                if (str.startsWith('r')) {
                        return new Register(expectRegister());
                } else {
                        return new Literal(expectLiteral());
                }
        }

        size_t expectLabelLocation() {
                const String label = expectArg();
                const Option<size_t> location = labels.get(label);
                if (location.isNone()) {
                        throw UnknownLabelException(ctx, label);
                }
                return location.unwrap();
        }

        void expectEndOfArgs() {
                if (readPos < args.length()) {
                        throw UnexpectedArgumentsException(ctx);
                }
        }
};

/// Could also be named `InstructionFactory`, since that's the design pattern it
/// implements, but `Parser` is the more commonly used name in the context of
/// compilers/interpreters.
class Parser {
        using InstructionFactoryMethod = Instruction *(*)(AsmVisitor);
        using InstructionFactory = HashMap<String, InstructionFactoryMethod>;

       private:
        static InstructionFactory GLOBAL_INSTRUCTION_FACTORY;

       private:
        HashMap<String, size_t> labels;
        const InstructionFactory &instructionFactory;

        // Flyweight design pattern - no need to create separate instances of
        // this global instruction factory for each individual parser instance,
        // since the syntax of the language remains the same.
        static const InstructionFactory &getGlobalInstructionFactory() {
                if (GLOBAL_INSTRUCTION_FACTORY.isEmpty()) {
                        GLOBAL_INSTRUCTION_FACTORY.insert("mov", Mov::factory);

                        GLOBAL_INSTRUCTION_FACTORY.insert("ifeq", IfStmt::ifeq);
                        GLOBAL_INSTRUCTION_FACTORY.insert("ifneq", IfStmt::ifneq);
                        GLOBAL_INSTRUCTION_FACTORY.insert("iflt", IfStmt::iflt);
                        GLOBAL_INSTRUCTION_FACTORY.insert("ifgt", IfStmt::ifgt);
                        GLOBAL_INSTRUCTION_FACTORY.insert("iflteq", IfStmt::iflteq);
                        GLOBAL_INSTRUCTION_FACTORY.insert("ifgteq", IfStmt::ifgteq);

                        GLOBAL_INSTRUCTION_FACTORY.insert("jmp", Jmp::factory);
                        GLOBAL_INSTRUCTION_FACTORY.insert("call", Call::factory);
                        GLOBAL_INSTRUCTION_FACTORY.insert("return", Return::factory);

                        GLOBAL_INSTRUCTION_FACTORY.insert("add", BinOpr::add);
                        GLOBAL_INSTRUCTION_FACTORY.insert("sub", BinOpr::sub);
                        GLOBAL_INSTRUCTION_FACTORY.insert("mul", BinOpr::mul);
                        GLOBAL_INSTRUCTION_FACTORY.insert("div", BinOpr::div);
                        GLOBAL_INSTRUCTION_FACTORY.insert("mod", BinOpr::mod);
                        GLOBAL_INSTRUCTION_FACTORY.insert("and", BinOpr::binAnd);
                        GLOBAL_INSTRUCTION_FACTORY.insert("or", BinOpr::binOr);
                        GLOBAL_INSTRUCTION_FACTORY.insert("xor", BinOpr::binXor);

                        GLOBAL_INSTRUCTION_FACTORY.insert("push", Push::factory);
                        GLOBAL_INSTRUCTION_FACTORY.insert("pop", Pop::factory);
                }
                return GLOBAL_INSTRUCTION_FACTORY;
        }

       public:
        Parser() : instructionFactory(getGlobalInstructionFactory()) {
        }

        Vector<Instruction *> parseFile(const String &) {
                return Vector<Instruction *>();
        }
};

#endif
