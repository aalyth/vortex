#ifndef VORTEX_TRAITS_H
#define VORTEX_TRAITS_H

#include <concepts>
#include <cstddef>
#include <functional>

template <typename T>
concept Primitive = std::is_arithmetic_v<T>;

template <typename T>
class Eq {
       public:
        virtual bool eq(const T &) const = 0;
        virtual ~Eq() = default;

        friend bool operator==(const T &lhs, const T &rhs) {
                return lhs.eq(rhs);
        }

        friend bool operator!=(const T &lhs, const T &rhs) {
                return !lhs.eq(rhs);
        }
};

template <typename T>
concept Equatable = std::derived_from<T, Eq<T>> || Primitive<T>;

enum class Ordering {
        Less,
        Equal,
        Greater,
};

template <typename R>
class Compare {
       public:
        virtual Ordering compare(const R &) const = 0;
        virtual ~Compare() = default;

        /* TODO remove
        friend bool operator<(const R &lhs, const R &rhs) {
                return lhs.compare(rhs) == Ordering::Less;
        }

        friend bool operator>(const R &lhs, const R &rhs) {
                return lhs.compare(rhs) == Ordering::Greater;
        }

        friend bool operator<=(const R &lhs, const R &rhs) {
                const Ordering result = lhs.compare(rhs);
                return result == Ordering::Less || result == Ordering::Equal;
        }

        friend bool operator>=(const R &lhs, const R &rhs) {
                const Ordering result = lhs.compare(rhs);
                return result == Ordering::Greater || result == Ordering::Equal;
        }

        friend bool operator==(const R &lhs, const R &rhs) {
                return lhs.compare(rhs) == Ordering::Equal;
        }

        friend bool operator!=(const R &lhs, const R &rhs) {
                return lhs.compare(rhs) != Ordering::Equal;
        }
        */
};

template <typename L, typename R = L>
concept Comparable = requires(const L &lhs, const R &rhs) {
        { lhs < rhs } -> std::convertible_to<bool>;
        { lhs > rhs } -> std::convertible_to<bool>;
        { lhs == rhs } -> std::convertible_to<bool>;
};

template <typename L, typename R = L>
        requires Comparable<L, R>
Ordering compare(const L &lhs, const R &rhs) {
        if (lhs < rhs) {
                return Ordering::Less;
        }
        if (lhs > rhs) {
                return Ordering::Greater;
        }
        return Ordering::Equal;
}

class Clone {
       public:
        virtual Clone *clone() const = 0;
        virtual ~Clone() = default;
};

template <typename T>
concept Cloneable = std::derived_from<T, Clone>;

class Hash {
       public:
        virtual size_t hash() const = 0;
        virtual ~Hash() = default;
};

template <typename T>
concept Hashable = std::derived_from<T, Hash> || requires(T t) {
        { std::hash<T>()(t) } -> std::convertible_to<size_t>;
};

template <Hashable T>
size_t hash(const T &value) {
        if constexpr (std::derived_from<T, Hash>) {
                return value.hash();
        } else {
                return std::hash<T>()(value);
        }
}

#endif
