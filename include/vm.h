#ifndef VORTEX_VM_H
#define VORTEX_VM_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "collections/vector.hpp"
#include "instructions/instructions.h"
#include "value.h"

/// The core of the whole language, used to execute the parsed user programs.
/// This implementation follows the register based virtual machine architecture,
/// which allows for more powerful instructions, but harder to programatically
/// integrate code. Such implementations are also easier to fine-tune and to
/// optimize compared to stack based alternatives.
class Vm {
       public:
        static constexpr size_t REGISTER_COUNT = 16;
        static constexpr size_t STACK_FRAMES = 4096;

       private:
        size_t nextInstruction = 0;
        double registers[REGISTER_COUNT];

        Vector<double> stack;

       public:
        Vm() = default;

        void execute(const Vector<Box<Instruction>> &);
        double getRegister(const Register &) const;
        void setRegister(const Register &, double);

        size_t getNextInstruction() const;
        void setNextInstruction(size_t);
        void goToNextInstruction();

        void push(double);
        double pop();

        /// Pushes the `nextInstruction` to the program stack.
        void pushCallFrame();
        /// Pops the top value off the stack and interprets it as an instruction
        /// location.
        size_t popCallFrame();
};

#endif
