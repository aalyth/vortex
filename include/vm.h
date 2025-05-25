#ifndef VORTEX_VM_H
#define VORTEX_VM_H

#include <cstddef>
#include <cstdint>

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
        static constexpr size_t STACK_FRAMES = 1024;

       private:
        size_t nextInstruction = 0;
        int64_t registers[REGISTER_COUNT];

        Vector<int64_t> stack;
        Vector<size_t> callStack;

       public:
        Vm() = default;

        void execute(const Vector<Instruction *> instructions) {
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

        void setRegister(size_t reg, int64_t value) {
                registers[reg] = value;
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
                callStack.pushBack(location);
        }

        size_t popCallFrame() {
                return callStack.popBack().expect(
                    "Calling VM::popCallFrame() on an empty call stack");
        }
};

#endif
