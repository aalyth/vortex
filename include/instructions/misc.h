
#ifndef VORTEX_MISC_INSTRUCTIONS_H
#define VORTEX_MISC_INSTRUCTIONS_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

class Mov : public Instruction {
       private:
        const Register dst;
        const Box<Value> src;

        Mov(const Register &, Box<Value> &&);

       public:
        static Box<Instruction> factory(AsmReader);

        void execute(Vm &) const override;
};

class Print : public Instruction {
       private:
        const Box<Value> value;

        Print(Box<Value> &&);

       public:
        static Box<Instruction> factory(AsmReader);

        void execute(Vm &) const override;
};

#endif
