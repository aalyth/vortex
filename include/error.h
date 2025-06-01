
#ifndef VORTEX_ERROR_H
#define VORTEX_ERROR_H

#include <exception>

#include "collections/string.h"

struct Context {
       public:
        size_t ln = 0;
        String filename;

       private:
        void clone(const Context &);

       public:
        Context() = default;
        Context(const String &);
        Context(const Context &);
        Context &operator=(const Context &);

        String messageContext() const;
};

class VortexException : public std::exception {
       protected:
        String message;

       public:
        VortexException(const Context &, const String &);

        virtual const char *what() const noexcept override;
        virtual ~VortexException() noexcept = default;
};

class ExpectedRegisterException : public VortexException {
       public:
        ExpectedRegisterException(const Context &, const String &);
};

class InvalidRegisterException : public VortexException {
       public:
        InvalidRegisterException(const Context &, String);
};

class ExpectedLiteralException : public VortexException {
       public:
        ExpectedLiteralException(const Context &, const String &);
};

class ExpectedArgumentException : public VortexException {
       public:
        ExpectedArgumentException(const Context &);
};

class UnexpectedArgumentsException : public VortexException {
       public:
        UnexpectedArgumentsException(const Context &);
};

class UnknownLabelException : public VortexException {
       public:
        UnknownLabelException(const Context &, const String &);
};

class InvalidLabelException : public VortexException {
       public:
        InvalidLabelException(const Context &, const String &);
};

class ConflictingLabelException : public VortexException {
       public:
        ConflictingLabelException(const Context &, const String &);
};

class UnknownInstructionException : public VortexException {
       public:
        UnknownInstructionException(const Context &, const String &);
};

class MissingEntryPointException : public VortexException {
       public:
        MissingEntryPointException(const Context &);
};

#endif
