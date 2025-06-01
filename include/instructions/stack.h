#ifndef VORTEX_STACK_INSTRUCTIONS_H
#define VORTEX_STACK_INSTRUCTIONS_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

class Push : public Instruction {
       public:
        const Box<Value> value;

        Push(Box<Value> &&);

       public:
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

class Pop : public Instruction {
       private:
        const Register dst;

        Pop(const Register &);

       public:
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

#endif
