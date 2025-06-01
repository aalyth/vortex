
#ifndef VORTEX_STRING_H
#define VORTEX_STRING_H

#include <cstddef>
#include <fstream>

#include "vector.hpp"

class String {
       private:
        static constexpr double ALLOCATOR_COEF = 1.5;
        static constexpr size_t DEFAULT_CAPACITY = 16;

        char *str;
        size_t len;
        size_t cap;

        void realloc(size_t);
        void free();
        void copy(const String &);
        void move(String &&) noexcept;

        static String between(const char *, const char *);

       public:
        String();
        String(size_t);
        String(const char *);
        String(const String &);
        String(String &&) noexcept;
        ~String();

        static String fromNumber(size_t);
        static String readLine(std::istream &);

        String &operator=(const String &);
        String &operator=(String &&) noexcept;

        String operator+(char) const;
        String operator+(const String &) const;

        bool operator==(const String &) const;

        size_t length() const;
        const char *cStr() const;

        void append(char);
        void append(const char *);

        bool startsWith(char) const;
        bool endsWith(char) const;
        bool isEmpty() const;
        bool all(bool (*)(char)) const;

        Vector<String> split(char) const;
        String substr(size_t, size_t) const;

        String trim() const;

        void truncateAfter(char);
};

namespace std {
template <>
struct hash<String> {
        size_t operator()(const String &value) const {
                size_t result = 0;
                const char *str = value.cStr();
                for (size_t i = 0; i < value.length(); ++i) {
                        result = result * 31 + (size_t)str[i];
                }
                return result;
        }
};
}  // namespace std

String operator+(const char *, const String &);
std::ostream &operator<<(std::ostream &, const String &);

bool isAlpha(char);
bool isNumeric(char);
bool isAlphanumeric(char);
bool isIdentifier(char);

size_t atou(const char *);
int64_t atoi64(const char *);

#endif
