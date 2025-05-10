
#ifndef VORTEX_HASH_MAP_H
#define VORTEX_HASH_MAP_H

#include <optional>

#include "vector.h"

template <typename K, typename V>
class HashMap {
       private:
        class SllNode {
                friend class Sll;

               private:
                SllNode *next;
                K key;
                V value;

               public:
                SllNode(K, V);
        };

        class Sll {
               private:
                SllNode *head = nullptr;
                SllNode *tail = nullptr;

               public:
                Sll() = default;

                void pushBack(K, V);
                std::optional<SllNode> popFront();
                std::optional<V> contains(const K &) const;
        };

       private:
        Vector<Sll> buckets;

       public:
        HashMap(size_t);
        HashMap(const HashMap &) = delete;
        HashMap(HashMap &&) = delete;

        HashMap &operator=(const HashMap &) = delete;
        HashMap &operator=(HashMap &&) = delete;

        void insert(K, V);
        std::optional<V> get(const K &) const;
};

#endif
