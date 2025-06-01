#ifndef VORTEX_FUNCTION_INSTRUCTIONS_H
#define VORTEX_FUNCTION_INSTRUCTIONS_H

#include <cstddef>

#include "base.h"
#include "collections/box.hpp"

class Jmp : public Instruction {
       private:
        const size_t location;

        Jmp(size_t);

       public:
        static Box<Instruction> factory(AsmReader);

        void execute(Vm &) const override;
};

class Call : public Instruction {
       private:
        const size_t location;

        Call(size_t);

       public:
        static Box<Instruction> factory(AsmReader);
        void execute(Vm &) const override;
};

class Return : public Instruction {
       public:
        static Box<Instruction> factory(AsmReader);

        void execute(Vm &) const override;
};

#endif
