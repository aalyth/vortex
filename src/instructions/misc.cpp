
#include "instructions/misc.h"

#include "parser.h"
#include "vm.h"

Mov::Mov(const Register &_dst, Box<Value> &&_src) : dst(_dst), src(std::move(_src)) {
}

Box<Instruction> Mov::factory(AsmReader parser) {
        const Register dst = parser.expectRegister();
        Box<Value> src = parser.expectValue();
        parser.expectEndOfArgs();
        return new Mov(dst, std::move(src));
}

void Mov::execute(Vm &vm) const {
        vm.setRegister(dst, src->getValue(vm));
        vm.goToNextInstruction();
}

Print::Print(Box<Value> &&_value) : value(std::move(_value)) {
}

Box<Instruction> Print::factory(AsmReader reader) {
        Box<Value> value = reader.expectValue();
        reader.expectEndOfArgs();
        return new Print(std::move(value));
}

void Print::execute(Vm &vm) const {
        std::cout << value->getValue(vm) << std::endl;
        vm.goToNextInstruction();
}
