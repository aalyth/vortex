#ifndef VORTEX_INSTRUCTIONS_BASE_H
#define VORTEX_INSTRUCTIONS_BASE_H

class Vm;
class AsmReader;

/// The abstract base class, which provides the interface for all of the
/// different supported instructions in the language. The process of creating
/// the separate instances of the instructions classes can be viewed in greater
/// detail inside the `parser.h` header.
class Instruction {
       public:
        virtual void execute(Vm &) const = 0;
        virtual ~Instruction() = default;
};

#endif
