
#include <iostream>

#include "collections/hash_map.hpp"
#include "collections/string.h"
#include "collections/vector.hpp"
//
#include "parser.h"
#include "vm.h"

int main() {
        try {
                Vm vm;
                Parser parser;
                Vector<Instruction *> instructions = parser.parseFile("./examples/fib.vx");

                size_t entry = parser.getLabels().get("main").expect("No entry point found");
                vm.setNextInstruction(entry);
                vm.execute(instructions);
        } catch (const VortexException &e) {
                std::cerr << e.what() << std::endl;
                return 1;
        } catch (const std::exception &e) {
                std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
                return 1;
        }
        return 0;
}
