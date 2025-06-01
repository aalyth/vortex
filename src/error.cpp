
#include "error.h"

void Context::clone(const Context &other) {
        this->ln = other.ln;
        this->filename = other.filename;
}

Context::Context(const String &_filename) : ln(0), filename(_filename) {
}

Context::Context(const Context &other) {
        clone(other);
}

Context &Context::operator=(const Context &other) {
        if (this != &other) {
                clone(other);
        }
        return *this;
}

String Context::messageContext() const {
        return String("[") + filename + ": " + String::fromNumber(ln) + "] ";
}

VortexException::VortexException(const Context &ctx, const String &msg)
    : message(ctx.messageContext() + msg) {
}

const char *VortexException::what() const noexcept {
        return message.cStr();
}

ExpectedRegisterException::ExpectedRegisterException(const Context &ctx, const String &received)
    : VortexException(ctx, "Expected a register, but received: " + received) {
}

InvalidRegisterException::InvalidRegisterException(const Context &ctx, String reg)
    : VortexException(ctx, "Invalid register: " + reg) {
}

ExpectedLiteralException::ExpectedLiteralException(const Context &ctx, const String &received)
    : VortexException(ctx, "Expected a literal, but received: " + received) {
}

ExpectedArgumentException::ExpectedArgumentException(const Context &ctx)
    : VortexException(ctx, "Expected an argument") {
}

UnexpectedArgumentsException::UnexpectedArgumentsException(const Context &ctx)
    : VortexException(ctx, "Received more than expected arguments") {
}

UnknownLabelException::UnknownLabelException(const Context &ctx, const String &label)
    : VortexException(ctx, "Unknown label: " + label) {
}

InvalidLabelException::InvalidLabelException(const Context &ctx, const String &label)
    : VortexException(ctx, "Invalid label identifier: " + label) {
}

ConflictingLabelException::ConflictingLabelException(const Context &ctx, const String &label)
    : VortexException(ctx, "Conflicting definition for label: " + label) {
}

UnknownInstructionException::UnknownInstructionException(const Context &ctx,
                                                         const String &instruction)
    : VortexException(ctx, "Unknown instruction: " + instruction) {
}

MissingEntryPointException::MissingEntryPointException(const Context &ctx)
    : VortexException(ctx, "No entry point found (label `main` is not defined)") {
}

