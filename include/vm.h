#ifndef VORTEX_VM_H
#define VORTEX_VM_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "collections/vector.hpp"
#include "instructions/instructions.h"
#include "value.h"

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

        void pushCallFrame(size_t);
        size_t popCallFrame();
};

#endif
