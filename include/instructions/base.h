#ifndef VORTEX_INSTRUCTIONS_BASE_H
#define VORTEX_INSTRUCTIONS_BASE_H

class Vm;
class AsmReader;

class Instruction {
       public:
        virtual void execute(Vm &) const = 0;
        virtual ~Instruction() = default;
};

#endif
