#ifndef VORTEX_OPERATIONS_H
#define VORTEX_OPERATIONS_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

class BinOpr : public Instruction {
       private:
        const Register dst;
        const Box<Value> src;
        double (*const operation)(double, double);

        BinOpr(const Register &, Box<Value> &&, double (*const)(double, double));

        static Box<Instruction> factory(AsmReader, double (*const)(double, double));

       public:
        static Box<Instruction> add(AsmReader);
        static Box<Instruction> sub(AsmReader);
        static Box<Instruction> mul(AsmReader);
        static Box<Instruction> div(AsmReader);
        static Box<Instruction> mod(AsmReader);

        static Box<Instruction> binAnd(AsmReader);
        static Box<Instruction> binOr(AsmReader);
        static Box<Instruction> binXor(AsmReader);

        void execute(Vm &vm) const override;
};

#endif
