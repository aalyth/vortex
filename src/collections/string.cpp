
#include "collections/string.h"

#include <algorithm>
#include <cstring>

void String::realloc(size_t size) {
        this->cap = size;
        this->len = std::min(this->len, this->cap);

        char *newStr = new char[this->cap + 1];
        strncpy(newStr, this->str, this->len);
        newStr[this->len] = '\0';
        delete[] this->str;
        this->str = newStr;
}

void String::free() {
        delete[] this->str;
        this->str = nullptr;
        this->len = 0;
        this->cap = 0;
}

void String::copy(const String &other) {
        this->len = other.len;
        this->cap = other.cap;
        this->str = new char[this->cap + 1];
        strncpy(this->str, other.str, this->len);
        this->str[this->len] = '\0';
}

void String::move(String &&other) noexcept {
        this->str = other.str;
        this->len = other.len;
        this->cap = other.cap;
        other.str = nullptr;
        other.len = 0;
        other.cap = 0;
}

String::String() : len(0), cap(DEFAULT_CAPACITY) {
        this->str = new char[this->cap + 1];
        this->str[0] = '\0';
}

String::String(size_t capacity) : len(0), cap(capacity) {
        this->str = new char[this->cap + 1];
        this->str[0] = '\0';
}

String::String(const char *_str) : len(strlen(_str)), cap(len) {
        this->str = new char[this->cap + 1];
        strncpy(this->str, _str, this->len);
        this->str[this->len] = '\0';
}

String::String(const String &other) {
        copy(other);
}

String::String(String &&other) noexcept {
        move(std::move(other));
}

String::~String() {
        free();
}

String &String::operator=(const String &other) {
        if (this != &other) {
                free();
                copy(other);
        }
        return *this;
}

String &String::operator=(String &&other) noexcept {
        if (this != &other) {
                free();
                move(std::move(other));
        }
        return *this;
}

bool String::operator==(const String &other) const {
        const size_t minLen = std::min(this->len, other.len);
        if (this->len != other.len) {
                return false;
        }
        return 0 == strncmp(this->str, other.str, minLen);
}

std::ostream &operator<<(std::ostream &out, const String &str) {
        return out << str.cStr();
}

size_t String::length() const {
        return len;
}

const char *String::cStr() const {
        return str;
}
