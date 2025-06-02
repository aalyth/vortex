#ifndef VORTEX_OPTION_H
#define VORTEX_OPTION_H

#include <stdexcept>

/// Type wrapper class, providing a convenient way of indicating a possibly
/// unset value. The type is an analogue to Rust's `Option<T>` type with the
/// difference that this implementation does not have native support by the C++
/// language and cannot be pattern-matched.
///
/// Accessing an unset value throws an `std::runtime_error` exception.
template <typename T>
class Option {
       private:
        T value;
        bool isSet;

       public:
        /// Builds an empty value - similar to `None`.
        Option();
        /// Wraps the value - similar to `Some()`.
        Option(T);

        Option& operator=(const Option<T>&);

        bool isNone() const;
        bool isSome() const;
        /// Attempts to access the value inside the container; throws
        /// `std::runtime_error` exception if the value is not set.
        T unwrap();
        /// Attempts to access the value inside the container; throws
        /// `std::runtime_error` exception if the value is not set.
        const T unwrap() const;
        /// Attempts to access the value inside the container; throws
        /// `std::runtime_error` exception with the given message if the value
        /// is not set.
        T expect(const char*);
        /// Attempts to access the value inside the container; throws an
        /// `std::runtime_error` exception with the given message if the value
        /// is not set.
        const T expect(const char*) const;
};

/// Template specialization for the `Option<T>` type, which provides means to
/// safely use generic type references inside the struct.
template <typename T>
class Option<T&> {
       private:
        T* value;

       public:
        Option();
        Option(T&);

        Option& operator=(const Option<T>&);

        /// Attempts to access the value inside the container; throws
        /// `std::runtime_error` exception if the value is not set.
        T& unwrap();
        /// Attempts to access the value inside the container; throws
        /// `std::runtime_error` exception if the value is not set.
        const T& unwrap() const;
        /// Attempts to access the value inside the container; throws
        /// `std::runtime_error` exception with the given message if the value
        /// is not set.
        T& expect(const char*);
        /// Attempts to access the value inside the container; throws an
        /// `std::runtime_error` exception with the given message if the value
        /// is not set.
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
