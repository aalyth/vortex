#ifndef VORTEX_VM_H
#define VORTEX_VM_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "collections/vector.hpp"
#include "value.h"

class Vm;

class Instruction {
       public:
        virtual void execute(Vm &) const = 0;
        virtual ~Instruction() = default;
};

class Vm final {
       public:
        static constexpr size_t REGISTER_COUNT = 16;
        static constexpr size_t STACK_FRAMES = 4096;

       private:
        size_t nextInstruction = 0;
        int64_t registers[REGISTER_COUNT];

        Vector<int64_t> stack;

       public:
        Vm() = default;

        void execute(Vector<Instruction *> instructions) {
                while (nextInstruction < instructions.length()) {
                        const Instruction *instr = instructions[nextInstruction].unwrap();
                        instr->execute(*this);
                }
        }

        int64_t getRegister(size_t reg) const {
                return registers[reg];
        }

        int64_t getRegister(const Register &reg) const {
                return registers[reg.getReg()];
        }

        void setRegister(const Register &reg, int64_t value) {
                registers[reg.getReg()] = value;
        }

        size_t getNextInstruction() const {
                return nextInstruction;
        }

        void setNextInstruction(size_t next) {
                nextInstruction = next;
        }

        void goToNextInstruction() {
                nextInstruction += 1;
        }

        void push(int64_t value) {
                stack.pushBack(value);
        }

        int64_t pop() {
                return stack.popBack().expect("Calling VM::pop() on an empty stack");
        }

        void pushCallFrame(size_t location) {
                stack.pushBack((int64_t)location);
        }

        size_t popCallFrame() {
                return (size_t)stack.popBack().expect(
                    "Calling VM::popCallFrame() on an empty call stack");
        }
};

#endif
