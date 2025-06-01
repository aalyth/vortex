
#include "vortex.h"

#include "parser.h"

void Vortex::execute(const String &filename) {
        try {
                Parser parser;
                const Vector<Box<Instruction>> instructions = parser.parseFile(filename);

                const size_t entry =
                    parser.getLabels().get(ENTRYPOINT_LABEL).expect("No entry point found");
                vm.setNextInstruction(entry);
                vm.execute(instructions);

        } catch (const VortexException &e) {
                std::cerr << e.what() << std::endl;
                return;

        } catch (const std::exception &e) {
                std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
                return;
        }
}

void Vortex::showSynopsis() {
        std::cout << "Usage: vortext [<script>|help]" << std::endl;
        std::cout << "A simple register-based virtual machine for executing programs.\n"
                  << "Each program must have a `main` label as the entry point." << std::endl;
}
