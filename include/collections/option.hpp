#ifndef VORTEX_OPTION_H
#define VORTEX_OPTION_H

#include <stdexcept>

template <typename T>
class Option final {
       private:
        T value;
        bool isSet;

       public:
        Option();
        Option(T);

        Option& operator=(const Option<T>&);

        bool isNone() const;
        bool isSome() const;
        T unwrap();
        const T unwrap() const;
        T expect(const char*);
        const T expect(const char*) const;
};

template <typename T>
class Option<T&> final {
       private:
        T* value;

       public:
        Option();
        Option(T&);

        Option& operator=(const Option<T>&);

        T& unwrap();
        const T& unwrap() const;
        T& expect(const char*);
        const T& expect(const char*) const;
};

template <typename T>
Option<T>::Option() : isSet(false) {
}

template <typename T>
Option<T>::Option(T _value) : value(_value), isSet(true) {
}

template <typename T>
Option<T>& Option<T>::operator=(const Option<T>& other) {
        if (this != other) {
                this->isSet = other.isSet;
                if (this->isSet) {
                        this->value = other.value;
                }
        }
        return *this;
}

template <typename T>
bool Option<T>::isNone() const {
        return !isSet;
}

template <typename T>
bool Option<T>::isSome() const {
        return isSet;
}

template <typename T>
T Option<T>::unwrap() {
        if (isNone()) {
                throw std::runtime_error("Unwrapping a None value.");
        }
        return value;
}

template <typename T>
const T Option<T>::unwrap() const {
        if (isNone()) {
                throw std::runtime_error("Unwrapping a None value.");
        }
        return value;
}

template <typename T>
T Option<T>::expect(const char* msg) {
        if (isNone()) {
                throw std::runtime_error(msg);
        }
        return value;
}

template <typename T>
const T Option<T>::expect(const char* msg) const {
        if (isNone()) {
                throw std::runtime_error(msg);
        }
        return value;
}

template <typename T>
Option<T&>::Option() : value(nullptr) {
}

template <typename T>
Option<T&>::Option(T& _value) : value(&_value) {
}

template <typename T>
T& Option<T&>::unwrap() {
        if (value == nullptr) {
                throw std::runtime_error("Unwrapping a None value.");
        }
        return *value;
}

template <typename T>
const T& Option<T&>::unwrap() const {
        if (value == nullptr) {
                throw std::runtime_error("Unwrapping a None value.");
        }
        return *value;
}

template <typename T>
T& Option<T&>::expect(const char* msg) {
        if (value == nullptr) {
                throw std::runtime_error(msg);
        }
        return *value;
}

template <typename T>
const T& Option<T&>::expect(const char* msg) const {
        if (value == nullptr) {
                throw std::runtime_error(msg);
        }
        return *value;
}

#endif
