
#include "parser.h"

InstructionFactory::Map InstructionFactory::GLOBAL_INSTRUCTION_FACTORY;

String AsmReader::expectArg() {
        if (readPos >= args.length()) {
                throw ExpectedArgumentException(ctx);
        }
        return args[readPos++].unwrap();
}

AsmReader::AsmReader(const Context &_ctx, const Vector<String> &_args,
                     const HashMap<String, size_t> &_labels)
    : ctx(_ctx), args(_args), labels(_labels) {
}

Register AsmReader::expectRegister() {
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

Literal AsmReader::expectLiteral() {
        const String str = expectArg();
        try {
                return Literal(atoi64(str.cStr()));
        } catch (const std::invalid_argument &) {
                throw ExpectedLiteralException(ctx, str);
        }
}

Box<Value> AsmReader::expectValue() {
        const String str = args[readPos].unwrap();
        if (str.startsWith('r')) {
                return Box<Value>(new Register(expectRegister()));
        } else {
                return Box<Value>(new Literal(expectLiteral()));
        }
}

size_t AsmReader::expectLabelLocation() {
        const String label = expectArg();
        const Option<size_t> location = labels.get(label);
        if (location.isNone()) {
                throw UnknownLabelException(ctx, label);
        }
        return location.unwrap();
}

void AsmReader::expectEndOfArgs() {
        if (readPos < args.length()) {
                throw UnexpectedArgumentsException(ctx);
        }
}

const InstructionFactory::Map &InstructionFactory::getGlobalInstructionFactory() {
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

                GLOBAL_INSTRUCTION_FACTORY.insert("addf", FloatingBinOpr::addf);
                GLOBAL_INSTRUCTION_FACTORY.insert("subf", FloatingBinOpr::subf);
                GLOBAL_INSTRUCTION_FACTORY.insert("mulf", FloatingBinOpr::mulf);
                GLOBAL_INSTRUCTION_FACTORY.insert("divf", FloatingBinOpr::divf);

                GLOBAL_INSTRUCTION_FACTORY.insert("add", IntegerBinOpr::add);
                GLOBAL_INSTRUCTION_FACTORY.insert("sub", IntegerBinOpr::sub);
                GLOBAL_INSTRUCTION_FACTORY.insert("mul", IntegerBinOpr::mul);
                GLOBAL_INSTRUCTION_FACTORY.insert("div", IntegerBinOpr::div);
                GLOBAL_INSTRUCTION_FACTORY.insert("mod", IntegerBinOpr::mod);
                GLOBAL_INSTRUCTION_FACTORY.insert("and", IntegerBinOpr::binAnd);
                GLOBAL_INSTRUCTION_FACTORY.insert("or", IntegerBinOpr::binOr);
                GLOBAL_INSTRUCTION_FACTORY.insert("xor", IntegerBinOpr::binXor);

                GLOBAL_INSTRUCTION_FACTORY.insert("push", Push::factory);
                GLOBAL_INSTRUCTION_FACTORY.insert("pop", Pop::factory);

                GLOBAL_INSTRUCTION_FACTORY.insert("print", Print::factory);
        }
        return GLOBAL_INSTRUCTION_FACTORY;
}

Parser::Parser() : instructionFactory(InstructionFactory::getGlobalInstructionFactory()) {
}

void Parser::parseLabel(const String &line, const Context &ctx, size_t currentInstructionIdx) {
        const String label = line.substr(0, line.length() - 1);
        if (!label.all(isIdentifier)) {
                throw InvalidLabelException(ctx, label);
        }
        if (labels.contains(label)) {
                throw ConflictingLabelException(ctx, label);
        }
        labels.insert(label, currentInstructionIdx);
}

Parser::RawInstruction Parser::parseInstruction(const String &line, const Context &ctx) {
        Vector<String> args = line.split(' ');
        const String instruction = args.popFront().expect("Parsing an empty instruction");
        return {instruction, std::move(args), ctx};
}

Vector<Parser::RawInstruction> Parser::parseFileContents(std::istream &sourceCode, Context &ctx) {
        Vector<RawInstruction> rawInstructions;
        do {
                String line = String::readLine(sourceCode).trim();
                ctx.ln += 1;

                line.truncateAfter(';');  // Remove comments
                if (line.isEmpty()) {
                        continue;
                }

                if (line.endsWith(':')) {
                        parseLabel(line, ctx, rawInstructions.length());

                } else {
                        rawInstructions.pushBack(parseInstruction(line, ctx));
                }
        } while (!sourceCode.eof());
        return rawInstructions;
}

Vector<Box<Instruction>> Parser::linkInstructions(const Vector<RawInstruction> &rawInstructions) {
        Vector<Box<Instruction>> instructions;
        for (const RawInstruction &instr : rawInstructions) {
                const String &name = instr.name;
                Option<InstructionFactory::Method> factoryMethod = instructionFactory.get(name);
                if (factoryMethod.isNone()) {
                        throw UnknownInstructionException(instr.ctx, name);
                }
                instructions.pushBack(
                    factoryMethod.unwrap()(AsmReader(instr.ctx, instr.args, labels)));
        }

        return instructions;
}

Vector<Box<Instruction>> Parser::parseFile(const String &filename) {
        static const String COULD_NOT_OPEN_FILE_MSG = "Could not open file: ";

        std::ifstream sourceCode(filename.cStr());
        if (!sourceCode.is_open()) {
                const String msg = COULD_NOT_OPEN_FILE_MSG + filename;
                throw std::runtime_error(msg.cStr());
        }

        Context ctx(filename);
        Vector<RawInstruction> rawInstructions = parseFileContents(sourceCode, ctx);
        sourceCode.close();

        return linkInstructions(rawInstructions);
}

const HashMap<String, size_t> &Parser::getLabels() const {
        return labels;
}
