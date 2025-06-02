
#include "instructions/operations.h"

#include "parser.h"

BinOpr::BinOpr(const Register &_dst, Box<Value> &&_src) : dst(_dst), src(std::move(_src)) {
}

Box<Instruction> FloatingBinOpr::factory(AsmReader reader,
                                         double (*const operation)(double, double)) {
        const Register dst = reader.expectRegister();
        Box<Value> src = reader.expectValue();
        reader.expectEndOfArgs();
        return new FloatingBinOpr(dst, std::move(src), operation);
}

FloatingBinOpr::FloatingBinOpr(const Register &_dst, Box<Value> &&_src,
                               double (*const _operation)(double, double))
    : BinOpr(_dst, std::move(_src)), operation(_operation) {
}

Box<Instruction> FloatingBinOpr::addf(AsmReader reader) {
        return FloatingBinOpr::factory(reader, [](double a, double b) { return a + b; });
}

Box<Instruction> FloatingBinOpr::subf(AsmReader reader) {
        return FloatingBinOpr::factory(reader, [](double a, double b) { return a - b; });
}

Box<Instruction> FloatingBinOpr::mulf(AsmReader reader) {
        return FloatingBinOpr::factory(reader, [](double a, double b) { return a * b; });
}

Box<Instruction> FloatingBinOpr::divf(AsmReader reader) {
        return FloatingBinOpr::factory(reader, [](double a, double b) { return a / b; });
}

void FloatingBinOpr::execute(Vm &vm) const {
        const double result = operation(dst.getValue(vm), src->getValue(vm));
        vm.setRegister(dst, result);
        vm.goToNextInstruction();
}

Box<Instruction> IntegerBinOpr::factory(AsmReader reader,
                                        double (*const operation)(int64_t, int64_t)) {
        const Register dst = reader.expectRegister();
        Box<Value> src = reader.expectValue();
        reader.expectEndOfArgs();
        return new IntegerBinOpr(dst, std::move(src), operation);
}

IntegerBinOpr::IntegerBinOpr(const Register &_dst, Box<Value> &&_src,
                             double (*const _operation)(int64_t, int64_t))
    : BinOpr(_dst, std::move(_src)), operation(_operation) {
}

Box<Instruction> IntegerBinOpr::add(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double(a + b)); });
}
Box<Instruction> IntegerBinOpr::sub(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a - b); });
}
Box<Instruction> IntegerBinOpr::mul(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a * b); });
}
Box<Instruction> IntegerBinOpr::div(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a / b); });
}
Box<Instruction> IntegerBinOpr::mod(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a % b); });
}

Box<Instruction> IntegerBinOpr::binAnd(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a & b); });
}
Box<Instruction> IntegerBinOpr::binOr(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a | b); });
}
Box<Instruction> IntegerBinOpr::binXor(AsmReader reader) {
        return IntegerBinOpr::factory(reader, [](int64_t a, int64_t b) { return (double)(a ^ b); });
}

void IntegerBinOpr::execute(Vm &vm) const {
        const int64_t dstValue = (int64_t)dst.getValue(vm);
        const int64_t srcValue = (int64_t)src->getValue(vm);
        vm.setRegister(dst, operation(dstValue, srcValue));
        vm.goToNextInstruction();
}
