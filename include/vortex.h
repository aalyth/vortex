#ifndef VORTEX_H
#define VORTEX_H

#include "vm.h"

class Vortex {
       private:
        static constexpr const char *ENTRYPOINT_LABEL = "main";

       private:
        Vm vm;

       public:
        void execute(const String &);
        static void showSynopsis();
};

#endif
