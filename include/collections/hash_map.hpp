#ifndef VORTEX_HASH_MAP_H
#define VORTEX_HASH_MAP_H

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

                void pushBack(K, V);
                Option<SllNode> popFront();
                Option<V> get(const K &) const;
                bool contains(const K &) const;
        };

       private:
        static constexpr size_t DEFAULT_BUCKETS = 16;

        Vector<Sll> buckets;

        size_t getBucketIndex(const K &) const;

       public:
        HashMap();
        HashMap(size_t);
        HashMap(const HashMap &) = delete;
        HashMap(HashMap &&) = delete;

        // TODO:
        // ~HashMap();

        HashMap &operator=(const HashMap &) = delete;
        HashMap &operator=(HashMap &&) = delete;

        void insert(K, V);
        Option<V> get(const K &) const;
        bool contains(const K &) const;
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
size_t HashMap<K, V>::getBucketIndex(const K &key) const {
        // using the memory address as the hash value
        return ((size_t)&key) % buckets.length();
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
void HashMap<K, V>::insert(K key, V value) {
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

#endif
