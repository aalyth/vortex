#ifndef VORTEX_VECTOR_H
#define VORTEX_VECTOR_H

#include <cstddef>
#include <optional>

template <typename T>
class Vector {
       private:
        static constexpr double ALLOCATOR_COEF = 1.5;

        class Iterator {
               private:
                T *ptr;

               public:
                Iterator(T *);

                bool operator==(const Iterator &) const;
                T *operator*() const;
                Iterator &operator++();
        };

       private:
        T *data;
        size_t len;
        size_t cap;

       public:
        Vector();
        Vector(size_t);
        Vector(const Vector &);
        Vector(Vector &&) noexcept;
        ~Vector();

        Vector &operator=(const Vector &);
        Vector &operator=(Vector &&) noexcept;

        size_t length() const;

        void pushBack(T &);
        void pushBack(T &&);
        std::optional<T> popBack();

        const T &operator[](size_t) const;
        T &operator[](size_t);

        Iterator begin() const;
        Iterator end() const;
};

#endif
