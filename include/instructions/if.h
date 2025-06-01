#ifndef VORTEX_IF_INSTRUCTION_H
#define VORTEX_IF_INSTRUCTION_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

class IfStmt : public Instruction {
       private:
        const Box<Value> v1;
        const Box<Value> v2;
        bool (*const condition)(double, double);

        IfStmt(Box<Value> &&, Box<Value> &&, bool (*const)(double, double));
        static Box<Instruction> factory(AsmReader, bool (*const)(double, double));

       public:
        static Box<Instruction> ifeq(AsmReader);
        static Box<Instruction> ifneq(AsmReader);
        static Box<Instruction> iflt(AsmReader);
        static Box<Instruction> ifgt(AsmReader);
        static Box<Instruction> iflteq(AsmReader);
        static Box<Instruction> ifgteq(AsmReader);

        void execute(Vm &vm) const override;
};

#endif
