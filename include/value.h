
#ifndef VORTEX_VALUE_H
#define VORTEX_VALUE_H

#include <cstddef>
#include <cstdint>

#include "error.h"

class Vm;

/// An abstract class, representing anything which can be interpreted as a value
/// in the language. In C++ terms, this `Value` type is neither lvalue, nor
/// rvalue, but merely a label, unifying both when applicable.
class Value {
       public:
        virtual double getValue(const Vm &) const = 0;
        virtual ~Value() = default;
};

/// Represents a register of the VM. In C++ terms this can be interpreted as an
/// lvalue type, which can store data and itself resolve to a value.
class Register : public Value {
       private:
        size_t reg;

       public:
        Register(const Context &, size_t);

        double getValue(const Vm &) const override;
        size_t getReg() const;
};

/// Represents a raw integer value. In C++ terms this can be interpreted as an
/// rvalue type, which can only be used for its value.
class Literal : public Value {
       private:
        int64_t literal;

       public:
        Literal(int64_t);
        double getValue(const Vm &) const override;
};

#endif
