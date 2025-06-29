
#include "collections/string.h"

#include <algorithm>
#include <cstring>
#include <iostream>

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
        memcpy(this->str, other.str, this->len);
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
        (void)memcpy(this->str, _str, this->len);
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

String String::fromNumber(size_t number) {
        String result;
        size_t reversed = 0;
        while (number > 0) {
                reversed = reversed * 10 + (number % 10);
                number /= 10;
        }
        while (reversed > 0) {
                const char digit = '0' + (char)(reversed % 10);
                result.append(digit);
                reversed /= 10;
        }
        return result;
}

String String::readLine(std::istream &in) {
        String result;
        char c;
        in.get(c);
        while (c != '\n' && !in.eof()) {
                result.append(c);
                in.get(c);
        }
        return result;
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

String String::operator+(char c) const {
        String result(*this);
        result.append(c);
        return result;
}

String String::operator+(const String &other) const {
        String result(*this);
        result.append(other.str);
        return result;
}

String operator+(const char *str, const String &appended) {
        return String(str) + appended;
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

void String::append(char c) {
        if (len == cap) {
                cap = (size_t)((double)cap * ALLOCATOR_COEF);
                realloc(cap);
        }
        str[len++] = c;
        str[len] = '\0';
}

void String::append(const char *added) {
        size_t addedLen = strlen(added);
        if (len + addedLen >= cap) {
                cap = (size_t)((double)(len + addedLen) * ALLOCATOR_COEF);
                realloc(cap);
        }
        (void)memcpy(str + len, added, addedLen);
        str[len + addedLen] = '\0';
        len += addedLen;
        str[len] = '\0';
}

bool String::startsWith(char c) const {
        return str[0] == c;
}

bool String::endsWith(char c) const {
        return str[len - 1] == c;
}

bool String::isEmpty() const {
        return len == 0;
}

bool String::all(bool (*predicate)(char)) const {
        for (size_t i = 0; i < len; ++i) {
                if (!predicate(str[i])) {
                        return false;
                }
        }
        return true;
}

String String::between(const char *start, const char *end) {
        size_t len = (size_t)(end - start);
        String result(len + 1);
        (void)strncpy(result.str, start, len);
        result.str[len] = '\0';
        result.len = len;
        return result;
}

Vector<String> String::split(char delimiter) const {
        Vector<String> result;
        const char *start = str;
        const char *end = strchr(start, delimiter);
        while (end != nullptr) {
                if (start != end) {
                        String res = between(start, end);
                        result.pushBack(std::move(res));
                }
                start = end + 1;
                end = strchr(start, delimiter);
        }
        if (*start != '\0') {
                String res = between(start, str + len);
                result.pushBack(std::move(res));
        }
        return result;
}

String String::substr(size_t startIdx, size_t endIdx) const {
        if (startIdx > endIdx || startIdx > len || endIdx > len) {
                throw std::out_of_range("Invalid substring range");
        }
        return between(str + startIdx, str + endIdx);
}

void String::truncateAfter(char c) {
        char *pos = strchr(str, c);
        if (pos != nullptr) {
                *pos = '\0';
                len = (size_t)(pos - str);
        }
}

static bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n';
}

String String::trim() const {
        size_t start = 0;
        while (start < len && isWhitespace(str[start])) {
                ++start;
        }
        size_t end = len;
        while (end > start && isWhitespace(str[end - 1])) {
                --end;
        }
        return substr(start, end);
}

vortex::Ordering String::compare(const String &other) const {
        const size_t minLen = std::min(len, other.len);
        const int cmp = strncmp(str, other.str, minLen);
        if (cmp < 0) {
                return vortex::Ordering::Less;
        }
        if (cmp > 0) {
                return vortex::Ordering::Greater;
        }
        return vortex::Ordering::Equal;
}

bool operator<(const String &lhs, const String &rhs) {
        return lhs.compare(rhs) == vortex::Ordering::Less;
}

bool operator>(const String &lhs, const String &rhs) {
        return lhs.compare(rhs) == vortex::Ordering::Greater;
}

bool operator==(const String &lhs, const String &rhs) {
        return lhs.compare(rhs) == vortex::Ordering::Equal;
}

size_t String::hash() const {
        size_t result = 0;
        for (size_t i = 0; i < len; ++i) {
                result = result * 31 + (size_t)str[i];
        }
        return result;
}

bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isNumeric(char c) {
        return (c >= '0' && c <= '9');
}

bool isAlphanumeric(char c) {
        return isAlpha(c) || isNumeric(c);
}

bool isIdentifier(char c) {
        return isAlphanumeric(c) || c == '_';
}

size_t atou(const char *str) {
        static const char *NULL_ARG_MSG = "NULL argument to atou()";
        static const String INVALID_CHAR_MSG = "Invalid character in string for atou(): ";

        if (nullptr == str) {
                throw std::invalid_argument(NULL_ARG_MSG);
        }

        size_t result = 0;
        while (*str != '\0') {
                const char currentChar = *str;
                if (!isNumeric(currentChar)) {
                        String msg = INVALID_CHAR_MSG + currentChar;
                        throw std::invalid_argument(msg.cStr());
                }
                result = result * 10 + (size_t)(currentChar - '0');
                ++str;
        }

        return result;
}

int64_t atoi64(const char *str) {
        static const char *NULL_ARG_MSG = "NULL argument to atoi64()";
        static const String INVALID_CHAR_MSG = "Invalid character in string for atoi64(): ";

        if (nullptr == str) {
                throw std::invalid_argument(NULL_ARG_MSG);
        }

        int64_t result = 0;
        bool isNegative = false;

        if (*str == '-') {
                isNegative = true;
                ++str;
        }

        while (*str != '\0') {
                const char currentChar = *str;
                if (!isNumeric(currentChar)) {
                        const String msg = INVALID_CHAR_MSG + currentChar;
                        throw std::invalid_argument(msg.cStr());
                }
                result = result * 10 + (int64_t)(currentChar - '0');
                ++str;
        }

        if (isNegative) {
                result *= -1;
        }
        return result;
}

