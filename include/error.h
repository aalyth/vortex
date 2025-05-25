
#ifndef VORTEX_ERROR_H
#define VORTEX_ERROR_H

#include <exception>

#include "collections/string.h"

struct Context {
        size_t ln;
        String filename;

        String msg() const {
                return String("[") + filename + ": " + String::fromNumber(ln) + "] ";
        }
};

class VortexException : public std::exception {
       protected:
        String message;

       public:
        VortexException(const Context &ctx, const String &msg) : message(ctx.msg() + msg) {
        }

        virtual const char *what() const noexcept override {
                return message.cStr();
        }

        virtual ~VortexException() noexcept = default;
};

class ExpectedRegisterException : public VortexException {
       public:
        ExpectedRegisterException(const Context &ctx, const String &received)
            : VortexException(ctx, "Expected a register, but received: " + received) {
        }
};

class InvalidRegisterException : public VortexException {
       public:
        InvalidRegisterException(const Context &ctx, String reg)
            : VortexException(ctx, "Invalid register: " + reg) {
        }
};

class ExpectedLiteralException : public VortexException {
       public:
        ExpectedLiteralException(const Context &ctx, const String &received)
            : VortexException(ctx, "Expected a literal, but received: " + received) {
        }
};

class ExpectedArgumentException : public VortexException {
       public:
        ExpectedArgumentException(const Context &ctx)
            : VortexException(ctx, "Expected an argument") {
        }
};

class UnexpectedArgumentsException : public VortexException {
       public:
        UnexpectedArgumentsException(const Context &ctx)
            : VortexException(ctx, "Received more than expected arguments") {
        }
};

class UnknownLabelException : public VortexException {
       public:
        UnknownLabelException(const Context &ctx, const String &label)
            : VortexException(ctx, "Unknown label: " + label) {
        }
};

#endif
