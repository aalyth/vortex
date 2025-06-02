
#ifndef VORTEX_ERROR_H
#define VORTEX_ERROR_H

#include <exception>

#include "collections/string.h"

/// Provides information about the context, in which an error has risen. This
/// way the user can more easily identify the wrong line of the source code,
/// which caused the failure.
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

        /// Returns a formatted message, that is designed to be used as a prefix
        /// to the different exceptions, which shows the file name and the line,
        /// which caused the issue.
        String messageContext() const;
};

/// An abstract class, grouping the different errors that could occur when
/// dealing with `vortex` programs.
class VortexException : public std::exception {
       protected:
        String message;

       public:
        VortexException(const Context &, const String &);

        virtual const char *what() const noexcept override;
        virtual ~VortexException() noexcept = default;
};

/// Used when expecing a register as the next argument to an instruction, but
/// receiving an invalid register value. For example, calling the instruction
/// `pop 4` triggers such error, since `4` is not a valid register.
class ExpectedRegisterException : public VortexException {
       public:
        ExpectedRegisterException(const Context &, const String &);
};

/// Used when the register signature is correct, but no such register exists -
/// for example, having the register `r157`.
class InvalidRegisterException : public VortexException {
       public:
        InvalidRegisterException(const Context &, String);
};

/// Used when failing to parse a literal integer value. By default, when
/// expecting a value object, the parser first checks if the string starts with
/// the letter `r` to see if the value is a register - if not, then it tries to
/// parse it into an integer literal.
class ExpectedLiteralException : public VortexException {
       public:
        ExpectedLiteralException(const Context &, const String &);
};

/// Thrown when an instruction requires more arguments than what are given.
class ExpectedArgumentException : public VortexException {
       public:
        ExpectedArgumentException(const Context &);
};

/// Thrown when an instruction requires less arguments than what are given.
class UnexpectedArgumentsException : public VortexException {
       public:
        UnexpectedArgumentsException(const Context &);
};

/// Used when encountering a label that does not exist. For more detailed
/// explanation on the process of parsing labes, refer to the class `Parser` in
/// the `parser.h` file.
class UnknownLabelException : public VortexException {
       public:
        UnknownLabelException(const Context &, const String &);
};

/// Thrown when trying to parse a label, but it is not a valid alphabetic
/// identifier.
class InvalidLabelException : public VortexException {
       public:
        InvalidLabelException(const Context &, const String &);
};

/// Thrown when redeclaring the same label for a second time.
class ConflictingLabelException : public VortexException {
       public:
        ConflictingLabelException(const Context &, const String &);
};

/// When the parser encounteres and instruction which does not exist.
class UnknownInstructionException : public VortexException {
       public:
        UnknownInstructionException(const Context &, const String &);
};

/// When the `main` label is missing from the executable script.
class MissingEntryPointException : public VortexException {
       public:
        MissingEntryPointException(const Context &);
};

#endif
