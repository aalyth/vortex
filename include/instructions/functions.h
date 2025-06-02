#ifndef VORTEX_FUNCTION_INSTRUCTIONS_H
#define VORTEX_FUNCTION_INSTRUCTIONS_H

#include <cstddef>

#include "base.h"
#include "collections/box.hpp"

/// Jumps to the given location inside the source code.
class Jmp : public Instruction {
       private:
        const size_t location;

       public:
        Jmp(size_t);
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

/// Jumps to the location inside the source code, whilst pushing the current
/// instruction pointer on the stack, simplifying the return process.
class Call : public Instruction {
       private:
        const size_t location;

       public:
        Call(size_t);
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

/// Pops the last value of the stack and jumps to that location. If no value is
/// present on the stack, this operation has undefined behvairour.
class Return : public Instruction {
       public:
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

#endif
