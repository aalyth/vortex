
#include "value.h"

#include "vm.h"

Register::Register(const Context &ctx, size_t _reg) : reg(_reg) {
        if (_reg >= Vm::REGISTER_COUNT) {
                throw InvalidRegisterException(ctx, _reg);
        }
}

double Register::getValue(const Vm &vm) const {
        return vm.getRegister(*this);
}

size_t Register::getReg() const {
        return reg;
}

Literal::Literal(int64_t _literal) : literal(_literal) {
}
double Literal::getValue(const Vm &) const {
        return literal;
}
