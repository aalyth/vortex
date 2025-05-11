
#ifndef VORTEX_STRING_H
#define VORTEX_STRING_H

#include <cstddef>
#include <ostream>

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

       public:
        String();
        String(size_t);
        String(const char *);
        String(const String &);
        String(String &&) noexcept;
        ~String();

        String &operator=(const String &);
        String &operator=(String &&) noexcept;

        bool operator==(const String &) const;

        size_t length() const;
        const char *cStr() const;
};

std::ostream &operator<<(std::ostream &, const String &);

#endif
