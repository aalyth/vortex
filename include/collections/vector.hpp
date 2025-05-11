#ifndef VORTEX_VECTOR_H
#define VORTEX_VECTOR_H

#include <algorithm>
#include <cstddef>

#include "option.hpp"
#include "utils.hpp"

template <typename T>
class Vector {
       private:
        class Iterator {
               private:
                T *ptr;

               public:
                Iterator(T *);

                bool operator==(const Iterator &) const;
                T operator*() const;
                Iterator &operator++();
        };

       private:
        static constexpr double ALLOCATOR_COEF = 1.5;
        static constexpr size_t DEFAULT_CAPACITY = 16;

        T *data;
        size_t len;
        size_t cap;

        void realloc(size_t);
        void copy(const Vector &);
        void free();
        void move(Vector &&) noexcept;

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
        Option<T> popBack();

        Option<const T &> operator[](size_t) const;
        Option<T &> operator[](size_t);

        Iterator begin() const;
        Iterator end() const;
};

template <typename T>
Vector<T>::Iterator::Iterator(T *_ptr) : ptr(_ptr) {
}

template <typename T>
bool Vector<T>::Iterator::operator==(const Iterator &other) const {
        return this->ptr == other.ptr;
}

template <typename T>
T Vector<T>::Iterator::operator*() const {
        return *ptr;
}

template <typename T>
Vector<T>::Iterator &Vector<T>::Iterator::operator++() {
        ptr += 1;
        return *this;
}

template <typename T>
void Vector<T>::realloc(size_t newCapacity) {
        cap = newCapacity;
        len = std::min(len, cap);

        T *newData = new T[cap];
        memtransfer(newData, data, len);
        delete[] data;
        data = newData;
}

template <typename T>
void Vector<T>::copy(const Vector<T> &other) {
        this->len = other.len;
        this->cap = other.cap;
        this->data = new T[this->cap];
        memtransfer(this->data, other.data, this->len);
}

template <typename T>
void Vector<T>::free() {
        delete[] data;
        data = nullptr;
        len = 0;
        cap = 0;
}

template <typename T>
void Vector<T>::move(Vector<T> &&other) noexcept {
        this->data = other.data;
        this->len = other.len;
        this->cap = other.cap;
        other.data = nullptr;
        other.len = 0;
        other.cap = 0;
}

template <typename T>
Vector<T>::Vector() : len(0), cap(DEFAULT_CAPACITY) {
        this->data = new T[this->cap];
}

template <typename T>
Vector<T>::Vector(size_t capacity) : len(0), cap(capacity) {
        this->data = new T[this->cap];
}
template <typename T>
Vector<T>::Vector(const Vector &other) {
        copy(other);
}

template <typename T>
Vector<T>::Vector(Vector &&other) noexcept {
        move(std::move(other));
}

template <typename T>
Vector<T>::~Vector() {
        free();
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
        if (this != &other) {
                free();
                copy(other);
        }
        return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> &&other) noexcept {
        if (this != &other) {
                free();
                move(std::move(other));
        }
        return *this;
}

template <typename T>
size_t Vector<T>::length() const {
        return len;
}

template <typename T>
void Vector<T>::pushBack(T &value) {
        if (len == cap) {
                // NOTE: this itself is not the proper way C++ way to do it,
                // since the implicit C-style in C++ compiles down to 5
                // different C++ casts
                cap = (size_t)((double)cap * ALLOCATOR_COEF);
                realloc(cap);
        }
        data[len++] = value;
}

template <typename T>
void Vector<T>::pushBack(T &&value) {
        if (len == cap) {
                cap = (size_t)((double)cap * ALLOCATOR_COEF);
                realloc(cap);
        }
        data[len++] = std::move(value);
}

template <typename T>
Option<T> Vector<T>::popBack() {
        if (len == 0) {
                return Option<T>();
        }
        T value = data[--len];
        return value;
}

template <typename T>
Option<const T &> Vector<T>::operator[](size_t idx) const {
        if (idx >= len) {
                return Option<const T &>();
        }
        return data[idx];
}

template <typename T>
Option<T &> Vector<T>::operator[](size_t idx) {
        if (idx >= len) {
                return Option<T &>();
        }
        return data[idx];
}

template <typename T>
Vector<T>::Iterator Vector<T>::begin() const {
        return Iterator(data);
}

template <typename T>
Vector<T>::Iterator Vector<T>::end() const {
        return Iterator(data + len);
}

#endif
