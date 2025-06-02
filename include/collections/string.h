
#ifndef VORTEX_STRING_H
#define VORTEX_STRING_H

#include <cstddef>
#include <fstream>

#include "vector.hpp"

/// Implementation of a heap allocated string, providing convenient abstraction
/// methods for working with strings.
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
        /// Builds an empty string with the `DEFAULT_CAPACITY`.
        String();
        /// Builds an empty string with a given capacity.
        String(size_t);
        /// Builds a heap allocated version of the passed string with just
        /// enough capacity to hold the data.
        String(const char *);
        String(const String &);
        String(String &&) noexcept;
        ~String();

        /// Converts the given number to a string format.
        static String fromNumber(size_t);
        /// Reads from the input stream until a newline is found or the stream
        /// ends.
        static String readLine(std::istream &);

        String &operator=(const String &);
        String &operator=(String &&) noexcept;

        /// Appends the character to the end of the string.
        String operator+(char) const;
        /// Concatenates the other string to `this`.
        String operator+(const String &) const;

        bool operator==(const String &) const;

        size_t length() const;
        /// A C-like view of the underlying string data.
        const char *cStr() const;

        void append(char);
        void append(const char *);

        bool startsWith(char) const;
        bool endsWith(char) const;
        bool isEmpty() const;
        bool all(bool (*)(char)) const;

        /// Splits the string into a list of non-empty strings, based on the
        /// passed delimiter.
        Vector<String> split(char) const;
        /// Returns a copy of the substring between the two indices.
        String substr(size_t, size_t) const;

        /// Removes any heading and trailing whitespaces.
        String trim() const;

        /// Removes all characters after the given stop symbol.
        void truncateAfter(char);
};

/// Implementation of the `std::hash<String>()` template specialization, used
/// for `HasHmap` indices.
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
