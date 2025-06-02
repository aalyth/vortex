
#include "instructions/functions.h"

#include "parser.h"
#include "vm.h"

Jmp::Jmp(size_t _location) : location(_location) {
}

Box<Instruction> Jmp::factory(AsmReader reader) {
        const size_t location = reader.expectLabelLocation();
        reader.expectEndOfArgs();
        return new Jmp(location);
}

void Jmp::execute(Vm &vm) const {
        vm.setNextInstruction(location);
}

Call::Call(size_t _location) : location(_location) {
}

Box<Instruction> Call::factory(AsmReader reader) {
        const size_t location = reader.expectLabelLocation();
        reader.expectEndOfArgs();
        return new Call(location);
}

void Call::execute(Vm &vm) const {
        vm.pushCallFrame();
        vm.setNextInstruction(location);
}

Box<Instruction> Return::factory(AsmReader reader) {
        reader.expectEndOfArgs();
        return new Return();
}

void Return::execute(Vm &vm) const {
        size_t location = vm.popCallFrame();
        vm.setNextInstruction(location);
        vm.goToNextInstruction();
}

