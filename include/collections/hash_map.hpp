#ifndef VORTEX_HASH_MAP_HPP
#define VORTEX_HASH_MAP_HPP

#include <functional>

#include "option.hpp"
#include "vector.hpp"

template <typename K, typename V>
class HashMap {
       private:
        class SllNode {
                friend class Sll;
                friend class HashMap;

               private:
                K key;
                V value;
                SllNode *next;

               public:
                SllNode(K, V);
        };

        class Sll {
                friend class HashMap;

               private:
                SllNode *head = nullptr;
                SllNode *tail = nullptr;

               public:
                Sll() = default;
                ~Sll();

                void pushBack(K, V);
                Option<SllNode> popFront();
                Option<V> get(const K &) const;
                bool contains(const K &) const;
        };

       private:
        static constexpr size_t DEFAULT_BUCKETS = 16;

       private:
        size_t elementsCount = 0;
        Vector<Sll> buckets;

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

        size_t size() const;
        bool isEmpty() const;
};

template <typename K, typename V>
HashMap<K, V>::SllNode::SllNode(K _key, V _value) : key(_key), value(_value), next(nullptr) {
}

template <typename K, typename V>
void HashMap<K, V>::Sll::pushBack(K key, V value) {
        SllNode *node = new SllNode(key, value);
        if (head == nullptr) {
                head = node;
                tail = node;
        } else {
                tail->next = node;
                tail = node;
        }
}

template <typename K, typename V>
HashMap<K, V>::Sll::~Sll() {
        SllNode *node = head;
        while (node != nullptr) {
                SllNode *next = node->next;
                delete node;
                node = next;
        }
}

template <typename K, typename V>
Option<typename HashMap<K, V>::SllNode> HashMap<K, V>::Sll::popFront() {
        if (head == nullptr) {
                return Option<SllNode>();
        }
        SllNode *node = head;
        head = head->next;
        if (head == nullptr) {
                tail = nullptr;
        }
        return *node;
}

template <typename K, typename V>
Option<V> HashMap<K, V>::Sll::get(const K &key) const {
        SllNode *node = head;
        while (node != nullptr) {
                if (node->key == key) {
                        return Option<V>(node->value);
                }
                node = node->next;
        }
        return Option<V>();
}

template <typename K, typename V>
bool HashMap<K, V>::Sll::contains(const K &key) const {
        return get(key);
}

template <typename K, typename V>
void HashMap<K, V>::free() {
        elementsCount = 0;
        buckets.~Vector();
}

template <typename K, typename V>
void HashMap<K, V>::move(HashMap<K, V> &&other) noexcept {
        this->elementsCount = other.elementsCount;
        this->buckets = std::move(other.buckets);
        other.elementsCount = 0;
}

template <typename K, typename V>
size_t HashMap<K, V>::getBucketIndex(const K &key) const {
        return std::hash<K>()(key) % buckets.length();
}

template <typename K, typename V>
HashMap<K, V>::HashMap() : HashMap(DEFAULT_BUCKETS) {
        for (size_t i = 0; i < DEFAULT_BUCKETS; i++) {
                this->buckets.pushBack(Sll());
        }
}

template <typename K, typename V>
HashMap<K, V>::HashMap(size_t bucketsCount) : buckets(bucketsCount) {
        for (size_t i = 0; i < bucketsCount; i++) {
                this->buckets.pushBack(Sll());
        }
}

template <typename K, typename V>
HashMap<K, V>::HashMap(HashMap &&other) noexcept {
        move(other);
}

template <typename K, typename V>
HashMap<K, V> &HashMap<K, V>::operator=(HashMap &&other) noexcept {
        if (this != &other) {
                this->~HashMap();
                move(std::move(other));
        }
        return *this;
}

template <typename K, typename V>
void HashMap<K, V>::insert(K key, V value) {
        elementsCount += 1;
        const size_t index = getBucketIndex(key);
        buckets[index].unwrap().pushBack(key, value);
}

template <typename K, typename V>
Option<V> HashMap<K, V>::get(const K &key) const {
        const size_t index = getBucketIndex(key);
        return buckets[index].unwrap().get(key);
}

template <typename K, typename V>
bool HashMap<K, V>::contains(const K &key) const {
        return get(key).isSome();
}

template <typename K, typename V>
size_t HashMap<K, V>::size() const {
        return elementsCount;
}

template <typename K, typename V>
bool HashMap<K, V>::isEmpty() const {
        return size() == 0;
}

#endif
