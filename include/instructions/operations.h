#ifndef VORTEX_OPERATIONS_H
#define VORTEX_OPERATIONS_H

#include "base.h"
#include "collections/box.hpp"
#include "value.h"

/// An abstract class, denoting binary operations, which take in 2 arguments.
/// The language syntax uses the following convention:
/// ```
/// <instruction> <register> <value>
/// ```
/// which means that:
/// ```
/// <register> = <register> <operation> <value>
/// ```
/// Where `<operation>` depends on the specified instruction.
class BinOpr : public Instruction {
       protected:
        const Register dst;
        const Box<Value> src;

       protected:
        BinOpr(const Register &, Box<Value> &&);
};

/// Represents binary operations between floating point numbers. Follows the
/// same syntax structure as described in the base class `BinOpr`. Currently in
/// the language floading point instruction mnemonics end with the suffix `f`.
class FloatingBinOpr : public BinOpr {
       private:
        double (*const operation)(double, double);

        static Box<Instruction> factory(AsmReader, double (*const)(double, double));

       public:
        FloatingBinOpr(const Register &, Box<Value> &&, double (*const)(double, double));

        static Box<Instruction> addf(AsmReader);
        static Box<Instruction> subf(AsmReader);
        static Box<Instruction> mulf(AsmReader);
        static Box<Instruction> divf(AsmReader);
        void execute(Vm &vm) const override;
};

/// Represents binary operations between integer values. Follows the same syntax
/// structure as described in the base class `BinOpr`.
class IntegerBinOpr : public BinOpr {
       private:
        double (*const operation)(int64_t, int64_t);

        static Box<Instruction> factory(AsmReader, double (*const)(int64_t, int64_t));

       public:
        IntegerBinOpr(const Register &, Box<Value> &&, double (*const)(int64_t, int64_t));

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
