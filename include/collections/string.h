
#ifndef VORTEX_STRING_H
#define VORTEX_STRING_H

#include <cstddef>

class String {
       private:
        static constexpr double ALLOCATOR_COEF = 1.5;

        char *str;
        size_t len;
        size_t cap;

        void realloc(size_t);
        void move(String &&);
        void copy(const String &);

       public:
        String();
        String(size_t);
        String(const char *);
        String(const String &);
        String(String &&) noexcept;
        ~String();

        String &operator=(const String &);
        String &operator=(String &&) noexcept;

        size_t length() const;
        const char *cStr() const;

        void append(const char *);
        void append(char);
};

#endif
