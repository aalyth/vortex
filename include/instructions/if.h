#ifndef VORTEX_IF_INSTRUCTION_H
#define VORTEX_IF_INSTRUCTION_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

/// Conditional instruction, which checks if the passed predicate is matched,
/// then tthe next statement in the list is execute - otherwise the next
/// instruction is skipped
class IfStmt : public Instruction {
       private:
        /// Since it is not advisable to compare values of type `double` for
        /// direct equality, this member annotates the chosen precision of the
        /// value equality.
        static constexpr double EQUALITY_EPSILON = 1e-10;

       private:
        const Box<Value> v1;
        const Box<Value> v2;
        bool (*const condition)(double, double);

        static Box<Instruction> factory(AsmReader, bool (*const)(double, double));

       public:
        IfStmt(Box<Value> &&, Box<Value> &&, bool (*const)(double, double));
        static Box<Instruction> ifeq(AsmReader);
        static Box<Instruction> ifneq(AsmReader);
        static Box<Instruction> iflt(AsmReader);
        static Box<Instruction> ifgt(AsmReader);
        static Box<Instruction> iflteq(AsmReader);
        static Box<Instruction> ifgteq(AsmReader);

        void execute(Vm &vm) const override;
};

#endif
