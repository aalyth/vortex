
#include "instructions/stack.h"

#include "parser.h"
#include "vm.h"

Push::Push(Box<Value> &&_value) : value(std::move(_value)) {
}

Box<Instruction> Push::factory(AsmReader reader) {
        Box<Value> value = reader.expectValue();
        reader.expectEndOfArgs();
        return new Push(std::move(value));
}

void Push::execute(Vm &vm) const {
        vm.push(value->getValue(vm));
        vm.goToNextInstruction();
}

Pop::Pop(const Register &_dst) : dst(_dst) {
}

Box<Instruction> Pop::factory(AsmReader reader) {
        const Register dst = reader.expectRegister();
        reader.expectEndOfArgs();
        return new Pop(dst);
}

void Pop::execute(Vm &vm) const {
        const double value = vm.pop();
        vm.setRegister(dst, value);
        vm.goToNextInstruction();
}

