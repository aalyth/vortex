
#include "collections/avl.hpp"

AvlTreeException::AvlTreeException(const String &msg) : message(msg) {
}

const char *AvlTreeException::what() const noexcept {
        return message.cStr();
}
