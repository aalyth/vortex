
#ifndef VORTEX_VALUE_H
#define VORTEX_VALUE_H

#include <cstddef>
#include <cstdint>

#include "error.h"

class Vm;

class Value {
       public:
        virtual double getValue(const Vm &) const = 0;
        virtual ~Value() = default;
};

class Register : public Value {
       private:
        size_t reg;

       public:
        Register(const Context &, size_t);

        double getValue(const Vm &) const override;
        size_t getReg() const;
};

class Literal : public Value {
       private:
        int64_t literal;

       public:
        Literal(int64_t);
        double getValue(const Vm &) const override;
};

#endif
