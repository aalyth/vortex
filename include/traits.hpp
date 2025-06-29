#ifndef VORTEX_TRAITS_H
#define VORTEX_TRAITS_H

#include <concepts>
#include <cstddef>
#include <functional>
#include <type_traits>

namespace vortex {

/// The reason for the direct use of the operators `==` and `!=` is due to the
/// fact that C++ does not expose a default interface that could be used to
/// categorize general comparison between types (e.g. Rust's `Eq` trait), which
/// would work both on primitive types and user-defined ones.
template <typename L, typename R = L>
concept Equatable = requires(const L &lhs, const R &rhs) {
        { lhs == rhs } -> std::convertible_to<bool>;
        { lhs != rhs } -> std::convertible_to<bool>;
};

enum class Ordering {
        Less,
        Equal,
        Greater,
};

/// The core idea of this class was to provide an interface, similar to Rust's
/// trait `Ord`, which automatically overloads the operators `<`, `>`, `<=`,
/// `>=`, `==` and `!=`, using the Barton-Nackman tick, but since the C++
/// compiler cannot automatically work out the Argument-dependent lookup (ADL),
/// since the operators are within the `Compare` class namespace.
template <typename R>
class Compare {
       public:
        virtual Ordering compare(const R &) const = 0;
        virtual ~Compare() = default;
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

/// The goal is to unite any cloning logic that needs to be in place, including
/// during the cloning of abstract types in polymorphic hierarchies.
template <typename T>
T clone(const T &value) {
        if constexpr (std::is_pointer_v<T>) {
                static_assert(std::is_base_of_v<Clone, std::remove_pointer_t<T>>,
                              "Type is not a clonable pointer");
                return value->clone();
        }
        if constexpr (std::is_abstract_v<T>) {
                static_assert(std::is_base_of_v<Clone, T>, "Type is not a clonable abstract class");
                return value.clone();
        }
        static_assert(std::copyable<T>, "Type is not copyable");
        return value;
}

template <typename T>
concept Cloneable = requires(const T &value) {
        { clone(value) } -> std::convertible_to<T>;
};

template <typename T>
concept Moveable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

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

}  // namespace vortex

#endif
