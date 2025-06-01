#include "vm.h"

void Vm::execute(const Vector<Box<Instruction>> &instructions) {
        while (nextInstruction < instructions.length()) {
                const Box<Instruction> &instr = instructions[nextInstruction].unwrap();
                instr->execute(*this);
        }
}

double Vm::getRegister(const Register &reg) const {
        return registers[reg.getReg()];
}

void Vm::setRegister(const Register &reg, double value) {
        registers[reg.getReg()] = value;
}

size_t Vm::getNextInstruction() const {
        return nextInstruction;
}

void Vm::setNextInstruction(size_t next) {
        nextInstruction = next;
}

void Vm::goToNextInstruction() {
        nextInstruction += 1;
}

void Vm::push(double value) {
        stack.pushBack(value);
}

double Vm::pop() {
        return stack.popBack().expect("Calling VM::pop() on an empty stack");
}

void Vm::pushCallFrame(size_t location) {
        stack.pushBack((int64_t)location);
}

size_t Vm::popCallFrame() {
        return (size_t)stack.popBack().expect("Calling VM::popCallFrame() on an empty call stack");
}
