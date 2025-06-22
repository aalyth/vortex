
#include <cstring>

#include "vortex.h"

int main(int argc, char* argv[]) {
        Vortex vortex;
        if (argc != 2) {
                vortex.showSynopsis();
                return 1;
        }

        if (0 == strcmp(argv[1], "help")) {
                vortex.showSynopsis();
        } else {
                vortex.execute(argv[1]);
        }

        return 0;
}
