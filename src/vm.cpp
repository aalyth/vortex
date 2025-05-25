/*
#include "vm.h"

void Vm::execute(const Vector<Instruction *> instructions) {
        while (nextInstruction < instructions.length()) {
                const Instruction *instr = instructions[nextInstruction].unwrap();
                instr->execute(*this);
                nextInstruction++;
        }
}

int64_t Vm::getRegister(size_t reg) const {
        return registers[reg];
}

void Vm::setRegister(size_t reg, int64_t value) {
        registers[reg] = value;
}
*/

