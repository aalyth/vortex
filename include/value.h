
#ifndef VORTEX_VALUE_H
#define VORTEX_VALUE_H

#include <cstddef>
#include <cstdint>

#include "error.h"

class Vm;

class Value {
       public:
        virtual int64_t getValue(const Vm &) const = 0;
        virtual ~Value() = default;
};

class Register : public Value {
       private:
        size_t reg;

       public:
        Register(const Context &, size_t);

        int64_t getValue(const Vm &) const override;
        size_t getReg() const {
                return reg;
        }
};

class Literal : public Value {
       private:
        int64_t literal;

       public:
        Literal(int64_t _literal) : literal(_literal) {
        }
        int64_t getValue(const Vm &) const override {
                return literal;
        }
};

#endif
