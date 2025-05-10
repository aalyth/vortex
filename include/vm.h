#ifndef VORTEX_VM_H
#define VORTEX_VM_H

#include <cstddef>
#include <cstdint>

#include "collections/vector.h"

class VM;

class Instruction {
       public:
        virtual void execute(VM&) const;
};

class VM {
       private:
        static constexpr size_t REGISTER_COUNT = 16;
        static constexpr size_t STACK_FRAMES = 1024;

        size_t nextInstruction = 0;
        uint64_t registers[REGISTER_COUNT];

        Vector<uint64_t> stack;

       public:
        VM() = default;

        void execute(const Vector<Instruction&>);

        uint64_t getRegister(size_t) const;
        void setRegister(size_t, uint64_t);
};

#endif
