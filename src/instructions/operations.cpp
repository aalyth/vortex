
#include "instructions/operations.h"

#include "parser.h"

BinOpr::BinOpr(const Register &_dst, Box<Value> &&_src, double (*const _operation)(double, double))
    : dst(_dst), src(std::move(_src)), operation(_operation) {
}

Box<Instruction> BinOpr::factory(AsmReader reader, double (*const operation)(double, double)) {
        const Register dst = reader.expectRegister();
        Box<Value> src = reader.expectValue();
        reader.expectEndOfArgs();
        return new BinOpr(dst, std::move(src), operation);
}

Box<Instruction> BinOpr::add(AsmReader reader) {
        return BinOpr::factory(reader, [](double a, double b) { return a + b; });
}

Box<Instruction> BinOpr::sub(AsmReader reader) {
        return BinOpr::factory(reader, [](double a, double b) { return a - b; });
}

Box<Instruction> BinOpr::mul(AsmReader reader) {
        return BinOpr::factory(reader, [](double a, double b) { return a * b; });
}

Box<Instruction> BinOpr::div(AsmReader reader) {
        return BinOpr::factory(reader, [](double a, double b) { return a / b; });
}

Box<Instruction> BinOpr::mod(AsmReader reader) {
        return BinOpr::factory(
            reader, [](double a, double b) { return (double)((int64_t)a % (int64_t)b); });
}

Box<Instruction> BinOpr::binAnd(AsmReader reader) {
        return BinOpr::factory(
            reader, [](double a, double b) { return (double)((int64_t)a & (int64_t)b); });
}

Box<Instruction> BinOpr::binOr(AsmReader reader) {
        return BinOpr::factory(
            reader, [](double a, double b) { return (double)((int64_t)a | (int64_t)b); });
}

Box<Instruction> BinOpr::binXor(AsmReader reader) {
        return BinOpr::factory(
            reader, [](double a, double b) { return (double)((int64_t)a ^ (int64_t)b); });
}

void BinOpr::execute(Vm &vm) const {
        const double result = operation(dst.getValue(vm), src->getValue(vm));
        vm.setRegister(dst, result);
        vm.goToNextInstruction();
}
