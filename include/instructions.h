#ifndef VORTEX_INSTRUCTIONS_H
#define VORTEX_INSTRUCTIONS_H

#include "value.h"
#include "vm.h"

class AsmVisitor;

class Mov : public Instruction {
       private:
        const Register dst;
        const Value &src;

        Mov(const Register &_dst, const Value &_src) : dst(_dst), src(_src) {
        }

       public:
        static Instruction *factory(AsmVisitor);

        void execute(Vm &vm) const override {
                vm.setRegister(dst, src.getValue(vm));
                vm.goToNextInstruction();
        }
};

class IfStmt : public Instruction {
       private:
        const Value *v1;
        const Value *v2;
        bool (*const condition)(int64_t, int64_t);

        IfStmt(const Value *_v1, const Value *_v2, bool (*const _condition)(int64_t, int64_t))
            : v1(_v1), v2(_v2), condition(_condition) {
        }

        static Instruction *factory(AsmVisitor, bool (*const)(int64_t, int64_t));

       public:
        static Instruction *ifeq(AsmVisitor);
        static Instruction *ifneq(AsmVisitor);
        static Instruction *iflt(AsmVisitor);
        static Instruction *ifgt(AsmVisitor);
        static Instruction *iflteq(AsmVisitor);
        static Instruction *ifgteq(AsmVisitor);

        void execute(Vm &vm) const override {
                if (!condition(v1->getValue(vm), v2->getValue(vm))) {
                        vm.goToNextInstruction();
                }
                vm.goToNextInstruction();
        }
};

class Jmp : public Instruction {
       private:
        const size_t location;

        Jmp(size_t _location) : location(_location) {
        }

       public:
        static Instruction *factory(AsmVisitor);

        void execute(Vm &vm) const override {
                vm.setNextInstruction(location);
        }
};

class Call : public Instruction {
       private:
        const size_t location;

        Call(size_t _location) : location(_location) {
        }

       public:
        static Instruction *factory(AsmVisitor);
        void execute(Vm &vm) const override {
                vm.pushCallFrame(vm.getNextInstruction());
                vm.setNextInstruction(location);
        }
};

class Return : public Instruction {
       public:
        static Instruction *factory(AsmVisitor);

        void execute(Vm &vm) const override {
                vm.setNextInstruction(vm.popCallFrame());
                vm.getNextInstruction();
        }
};

class BinOpr : public Instruction {
       private:
        const Register dst;
        const Value *src;
        int64_t (*const operation)(int64_t, int64_t);

        BinOpr(const Register &_dst, const Value *_src,
               int64_t (*const _operation)(int64_t, int64_t))
            : dst(_dst), src(_src), operation(_operation) {
        }

        static Instruction *factory(AsmVisitor, int64_t (*const)(int64_t, int64_t));

       public:
        static Instruction *add(AsmVisitor);
        static Instruction *sub(AsmVisitor);
        static Instruction *mul(AsmVisitor);
        static Instruction *div(AsmVisitor);
        static Instruction *mod(AsmVisitor);

        static Instruction *binAnd(AsmVisitor);
        static Instruction *binOr(AsmVisitor);
        static Instruction *binXor(AsmVisitor);

        void execute(Vm &vm) const override {
                const int64_t result = operation(dst.getValue(vm), src->getValue(vm));
                vm.setRegister(dst, result);
                vm.goToNextInstruction();
        }
};

class Push : public Instruction {
       public:
        const Value *value;
        Push(const Value *_value) : value(_value) {
        }

       public:
        static Instruction *factory(AsmVisitor);

        void execute(Vm &vm) const override {
                vm.push(value->getValue(vm));
                vm.goToNextInstruction();
        }
};

class Pop : public Instruction {
       private:
        const Register dst;
        Pop(const Register &_dst) : dst(_dst) {
        }

       public:
        static Instruction *factory(AsmVisitor);
        void execute(Vm &vm) const override {
                const int64_t value = vm.pop();
                vm.setRegister(dst, value);
                vm.goToNextInstruction();
        }
};

/*
iflt r1 2
        jmp fib_base_case
jmp fib_default_case

fib_base_case:
mov r0 0
return

fib_default_case:
push r1
sub r1 1
call fib

pop r1
push r0
sub r1 2
call fib

pop r1
add r0 r1
return

main:
mov r1 35
call fib
print r0
*/

#endif
