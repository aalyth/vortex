#ifndef VORTEX_BOX_H
#define VORTEX_BOX_H

#include <stdexcept>

template <typename T>
class Box {
       private:
        T* ptr = nullptr;

        void free();
        void move(Box&&) noexcept;

       public:
        Box();
        Box(T*);
        Box(const Box&) = delete;
        Box(Box&&) noexcept;
        ~Box();

        Box& operator=(const Box&) = delete;
        Box& operator=(Box&&) noexcept;

        T& operator*();
        const T& operator*() const;
        T* operator->();
        const T* operator->() const;
};

template <typename T>
void Box<T>::free() {
        delete ptr;
        ptr = nullptr;
}

template <typename T>
void Box<T>::move(Box<T>&& other) noexcept {
        this->ptr = other.ptr;
        other.ptr = nullptr;
}

template <typename T>
Box<T>::Box() : ptr(nullptr) {
}

template <typename T>
Box<T>::Box(T* _ptr) {
        if (nullptr == _ptr) {
                throw std::runtime_error("Box cannot be initialized with a null pointer.");
        }
        this->ptr = _ptr;
}

template <typename T>
Box<T>::Box(Box&& other) noexcept {
        move(std::move(other));
}

template <typename T>
Box<T>::~Box() {
        free();
}

template <typename T>
Box<T>& Box<T>::operator=(Box<T>&& other) noexcept {
        if (this != &other) {
                free();
                move(std::move(other));
        }
        return *this;
}

template <typename T>
T& Box<T>::operator*() {
        return ptr;
}

template <typename T>
const T& Box<T>::operator*() const {
        return ptr;
}

template <typename T>
T* Box<T>::operator->() {
        return ptr;
}

template <typename T>
const T* Box<T>::operator->() const {
        return ptr;
}

#endif
