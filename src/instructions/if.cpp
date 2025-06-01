
#include "instructions/if.h"

#include "parser.h"
#include "vm.h"

IfStmt::IfStmt(Box<Value> &&_v1, Box<Value> &&_v2, bool (*const _condition)(double, double))
    : v1(std::move(_v1)), v2(std::move(_v2)), condition(_condition) {
}

Box<Instruction> IfStmt::factory(AsmReader reader, bool (*const condition)(double, double)) {
        Box<Value> v1 = reader.expectValue();
        Box<Value> v2 = reader.expectValue();
        reader.expectEndOfArgs();
        return new IfStmt(std::move(v1), std::move(v2), condition);
}

Box<Instruction> IfStmt::ifeq(AsmReader reader) {
        return IfStmt::factory(reader, [](double a, double b) { return a - b < 0.00001; });
}

Box<Instruction> IfStmt::ifneq(AsmReader reader) {
        return IfStmt::factory(reader, [](double a, double b) { return a != b; });
}

Box<Instruction> IfStmt::iflt(AsmReader reader) {
        return IfStmt::factory(reader, [](double a, double b) { return a < b; });
}

Box<Instruction> IfStmt::ifgt(AsmReader reader) {
        return IfStmt::factory(reader, [](double a, double b) { return a > b; });
}

Box<Instruction> IfStmt::iflteq(AsmReader reader) {
        return IfStmt::factory(reader, [](double a, double b) { return a <= b; });
}

Box<Instruction> IfStmt::ifgteq(AsmReader reader) {
        return IfStmt::factory(reader, [](double a, double b) { return a >= b; });
}

void IfStmt::execute(Vm &vm) const {
        if (!condition(v1->getValue(vm), v2->getValue(vm))) {
                vm.goToNextInstruction();
        }
        vm.goToNextInstruction();
}
