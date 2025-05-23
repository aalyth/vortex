
#include <iostream>

#include "collections/hash_map.hpp"
#include "collections/string.h"
#include "collections/vector.hpp"

int main() {
        HashMap<String, int (*)(Vector<String>)> map2;

        map2.insert("hello", [](Vector<String> vec) { return 0 + (int)vec.length(); });
        map2.insert("world", [](Vector<String> vec) { return 1 + (int)vec.length(); });

        std::cout << map2.get("hello").unwrap()(Vector<String>()) << std::endl;

        std::cout << "hello world" << std::endl;
        return 0;
}
