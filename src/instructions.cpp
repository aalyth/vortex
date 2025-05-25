
#include "instructions.h"

#include "parser.h"

Instruction *Mov::factory(AsmVisitor parser) {
        const Register dst = parser.expectRegister();
        const Value *src = parser.expectValue();
        parser.expectEndOfArgs();
        return new Mov(dst, src);
}

Instruction *IfStmt::factory(AsmVisitor visitor, bool (*const condition)(int64_t, int64_t)) {
        const Value *v1 = visitor.expectValue();
        const Value *v2 = visitor.expectValue();
        visitor.expectEndOfArgs();
        return new IfStmt(v1, v2, condition);
}

Instruction *IfStmt::ifeq(AsmVisitor visitor) {
        return IfStmt::factory(visitor, [](int64_t a, int64_t b) { return a == b; });
}

Instruction *IfStmt::ifneq(AsmVisitor visitor) {
        return IfStmt::factory(visitor, [](int64_t a, int64_t b) { return a != b; });
}

Instruction *IfStmt::iflt(AsmVisitor visitor) {
        return IfStmt::factory(visitor, [](int64_t a, int64_t b) { return a < b; });
}

Instruction *IfStmt::ifgt(AsmVisitor visitor) {
        return IfStmt::factory(visitor, [](int64_t a, int64_t b) { return a > b; });
}

Instruction *IfStmt::iflteq(AsmVisitor visitor) {
        return IfStmt::factory(visitor, [](int64_t a, int64_t b) { return a <= b; });
}

Instruction *IfStmt::ifgteq(AsmVisitor visitor) {
        return IfStmt::factory(visitor, [](int64_t a, int64_t b) { return a >= b; });
}

Instruction *Jmp::factory(AsmVisitor visitor) {
        const size_t location = visitor.expectLabelLocation();
        visitor.expectEndOfArgs();
        return new Jmp(location);
}

Instruction *Call::factory(AsmVisitor visitor) {
        const size_t location = visitor.expectLabelLocation();
        visitor.expectEndOfArgs();
        return new Call(location);
}

Instruction *Return::factory(AsmVisitor visitor) {
        visitor.expectEndOfArgs();
        return new Return();
}

Instruction *BinOpr::factory(AsmVisitor visitor, int64_t (*const operation)(int64_t, int64_t)) {
        const Register dst = visitor.expectRegister();
        const Value *src = visitor.expectValue();
        visitor.expectEndOfArgs();
        return new BinOpr(dst, src, operation);
}

Instruction *BinOpr::add(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a + b; });
}

Instruction *BinOpr::sub(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a - b; });
}

Instruction *BinOpr::mul(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a * b; });
}

Instruction *BinOpr::div(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a / b; });
}

Instruction *BinOpr::mod(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a % b; });
}

Instruction *BinOpr::binAnd(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a & b; });
}

Instruction *BinOpr::binOr(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a | b; });
}

Instruction *BinOpr::binXor(AsmVisitor visitor) {
        return BinOpr::factory(visitor, [](int64_t a, int64_t b) { return a ^ b; });
}

Instruction *Push::factory(AsmVisitor visitor) {
        const Value *value = visitor.expectValue();
        visitor.expectEndOfArgs();
        return new Push(value);
}

Instruction *Pop::factory(AsmVisitor visitor) {
        const Register dst = visitor.expectRegister();
        visitor.expectEndOfArgs();
        return new Pop(dst);
}

Instruction *Print::factory(AsmVisitor visitor) {
        const Value *value = visitor.expectValue();
        visitor.expectEndOfArgs();
        return new Print(value);
}
