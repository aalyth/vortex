#ifndef VORTEX_HASH_MAP_HPP
#define VORTEX_HASH_MAP_HPP

#include "collections/avl.hpp"
#include "option.hpp"
#include "traits.hpp"
#include "vector.hpp"

template <typename T>
concept Key = Hashable<T> && Comparable<T>;

/// A simple key-value map structure. The type `K` must implement the
/// `std::hash<K>` template specialization and is implied that the type can be
/// compared via the `operator==`.
///
/// The copy constructor and `operator=` are deleted since they imply type
/// cloning, which given that `K` is not type-bound to a custom `Cloneable`
/// concept, should not rely on an arbitrary means of deeply copying data.
///
/// This implementation follows the general structure of a `HashMap`, but
/// instead of using AVL trees for the individual bucket stores, it uses Singly
/// Linked Lists (SLL), which provide less time complexity amortization, but
/// still good enough performance.
template <Key K, typename V>
class HashMap {
       private:
        static constexpr size_t DEFAULT_BUCKETS = 16;

        struct Entry : public Compare<Entry>, public Compare<K> {
               private:
                K key;
                V value;

               public:
                Entry(K k, V v) : key(k), value(v) {
                }

                virtual Ordering compare(const Entry &other) const override {
                        return key.compare(other.getKey());
                }

                virtual Ordering compare(const K &other) const override {
                        return key.compare(other);
                }
                friend bool operator<(const Entry &lhs, const Entry &rhs) {
                        return lhs.compare(rhs) == Ordering::Less;
                }

                friend bool operator>(const Entry &lhs, const Entry &rhs) {
                        return lhs.compare(rhs) == Ordering::Greater;
                }

                friend bool operator==(const Entry &lhs, const Entry &rhs) {
                        return lhs.compare(rhs) == Ordering::Equal;
                }

                friend bool operator<(const Entry &lhs, const K &rhs) {
                        return lhs.getKey().compare(rhs) == Ordering::Less;
                }

                friend bool operator>(const Entry &lhs, const K &rhs) {
                        return lhs.getKey().compare(rhs) == Ordering::Greater;
                }

                friend bool operator==(const Entry &lhs, const K &rhs) {
                        return lhs.getKey().compare(rhs) == Ordering::Equal;
                }

                const K &getKey() const {
                        return key;
                }

                const V &getValue() const {
                        return value;
                }
        };

       private:
        size_t elementsCount = 0;
        Vector<AvlTree<Entry>> buckets;

        void free();
        void move(HashMap &&) noexcept;
        size_t getBucketIndex(const K &) const;

       public:
        HashMap();
        HashMap(size_t);
        HashMap(const HashMap &) = delete;
        HashMap(HashMap &&) noexcept;

        HashMap &operator=(const HashMap &) = delete;
        HashMap &operator=(HashMap &&) noexcept;

        void insert(K, V);
        Option<V> get(const K &) const;
        bool contains(const K &) const;

        /// Returns the total number of elements, present across all buckets
        /// inside the `HashMap`.
        size_t size() const;
        bool isEmpty() const;
};

template <Key K, typename V>
void HashMap<K, V>::free() {
        elementsCount = 0;
        buckets.~Vector();
}

template <Key K, typename V>
void HashMap<K, V>::move(HashMap<K, V> &&other) noexcept {
        this->elementsCount = other.elementsCount;
        this->buckets = std::move(other.buckets);
        other.elementsCount = 0;
}

template <Key K, typename V>
size_t HashMap<K, V>::getBucketIndex(const K &key) const {
        return hash(key) % buckets.length();
}

template <Key K, typename V>
HashMap<K, V>::HashMap() : HashMap(DEFAULT_BUCKETS) {
        for (size_t i = 0; i < DEFAULT_BUCKETS; i++) {
                buckets.pushBack(AvlTree<Entry>());
        }
}

template <Key K, typename V>
HashMap<K, V>::HashMap(size_t bucketsCount) : buckets(bucketsCount) {
        for (size_t i = 0; i < bucketsCount; i++) {
                buckets.pushBack(AvlTree<Entry>());
        }
}

template <Key K, typename V>
HashMap<K, V>::HashMap(HashMap &&other) noexcept {
        move(other);
}

template <Key K, typename V>
HashMap<K, V> &HashMap<K, V>::operator=(HashMap &&other) noexcept {
        if (this != &other) {
                this->~HashMap();
                move(std::move(other));
        }
        return *this;
}

template <Key K, typename V>
void HashMap<K, V>::insert(K key, V value) {
        elementsCount += 1;
        const size_t index = getBucketIndex(key);
        buckets[index].unwrap().insert(Entry(key, value));
}

template <Key K, typename V>
Option<V> HashMap<K, V>::get(const K &key) const {
        const size_t index = getBucketIndex(key);
        Option<const Entry &> entry = buckets[index].unwrap().find(key);
        if (entry.isNone()) {
                return Option<V>();
        }
        return Option<V>(entry.unwrap().getValue());
}

template <Key K, typename V>
bool HashMap<K, V>::contains(const K &key) const {
        return get(key).isSome();
}

template <Key K, typename V>
size_t HashMap<K, V>::size() const {
        return elementsCount;
}

template <Key K, typename V>
bool HashMap<K, V>::isEmpty() const {
        return size() == 0;
}

#endif
