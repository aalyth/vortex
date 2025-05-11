
#include <iostream>

#include "collections/hash_map.hpp"
#include "collections/string.h"
#include "collections/vector.hpp"

int main() {
        HashMap<String, int (*)(Vector<String>)> map2;

        map2.insert("hello", [](Vector<String> _) { return 0; });
        map2.insert("world", [](Vector<String> _) { return 1; });

        std::cout << map2.get("hello").unwrap()(Vector<String>()) << std::endl;

        std::cout << "hello world" << std::endl;
        return 0;
}
