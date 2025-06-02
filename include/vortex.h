#ifndef VORTEX_H
#define VORTEX_H

#include "parser.h"
#include "vm.h"

/// An user-facing abstraction of the `vortex` language. Currently the purpose
/// is to encapsulate the CLI logic from the implementation, but could also
/// serve for internal purposes of integrating into other applications, which
/// require runtime compilation of code.
class Vortex {
       private:
        static constexpr const char *ENTRYPOINT_LABEL = "main";

       private:
        Vm vm;
        Parser parser;

       public:
        void execute(const String &);
        static void showSynopsis();
};

#endif
