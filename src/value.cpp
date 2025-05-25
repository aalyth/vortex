
#include "value.h"

#include "vm.h"

Register::Register(const Context &ctx, size_t _reg) : reg(_reg) {
        if (_reg >= Vm::REGISTER_COUNT) {
                throw InvalidRegisterException(ctx, _reg);
        }
}

int64_t Register::getValue(const Vm &vm) const {
        return vm.getRegister(reg);
}
