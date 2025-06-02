#ifndef VORTEX_STACK_INSTRUCTIONS_H
#define VORTEX_STACK_INSTRUCTIONS_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

/// Pushes the value onto the stack. Primarily used when calling external
/// functions in order to preserve local values and when using recursion.
class Push : public Instruction {
       public:
        const Box<Value> value;

       public:
        Push(Box<Value> &&);
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

/// Pops the top value off the stack into the specified register.Primarily used
/// when calling external functions in order to preserve local values and when
/// using recursion.
class Pop : public Instruction {
       private:
        const Register dst;

       public:
        Pop(const Register &);
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

#endif
