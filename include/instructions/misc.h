
#ifndef VORTEX_MISC_INSTRUCTIONS_H
#define VORTEX_MISC_INSTRUCTIONS_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

/// Moves the source value to the destination register. If the value is of type
/// `Register`, then the value is copied from the source.
class Mov : public Instruction {
       private:
        const Register dst;
        const Box<Value> src;

       public:
        Mov(const Register &, Box<Value> &&);
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

/// Prints the value or the contents of a given register.
class Print : public Instruction {
       private:
        const Box<Value> value;

       public:
        Print(Box<Value> &&);
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

#endif
