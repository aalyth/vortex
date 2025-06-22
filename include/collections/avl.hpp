#ifndef VORTEX_AVL_H
#define VORTEX_AVL_H

#include <cmath>

#include "option.hpp"
#include "string.h"
#include "traits.hpp"

class AvlTreeException : public std::exception {
       protected:
        String message;

       public:
        AvlTreeException(const String &);

        virtual const char *what() const noexcept override;
};

template <Comparable T>
class AvlTree;

template <Comparable T>
class AvlTreeNode {
       private:
        enum class Balance;

       private:
        T value;
        AvlTreeNode<T> *left = nullptr;
        AvlTreeNode<T> *right = nullptr;
        unsigned depth = 1;

       private:
        static void safeUpdateDepth(AvlTreeNode *);

        unsigned leftDepth() const;
        unsigned rightDepth() const;
        int depthDiff() const;

        Balance getBalance() const;

        /* Performs the following movement:
         *     R               K
         *    / \             / \
         *   T1  K     ->    R  T3
         *      / \         / \
         *     T2  T3      T1 T2
         */
        AvlTreeNode *rotateLeft();

        /* Performs the following movement:
         *      R          K
         *     / \        / \
         *    K  T3  ->  T1  R
         *   / \            / \
         *  T1  T2         T2  T3
         */
        AvlTreeNode *rotateRight();

        AvlTreeNode(T);
        static AvlTreeNode *insert(AvlTreeNode *, T);

        template <typename Comparator>
                requires Comparable<T, Comparator>
        static Option<const T &> find(const AvlTreeNode *, const Comparator &);

       private:
        enum class Balance {
                Balanced,
                LeftLeft,
                LeftRight,
                RightLeft,
                RightRight,
        };

        friend class AvlTree<T>;
};

template <Comparable T>
class AvlTree {
       private:
        AvlTreeNode<T> *root = nullptr;

       public:
        AvlTree() = default;
        void insert(T value);

        template <typename F>
                requires Comparable<T, F>
        Option<const T &> find(const F &) const;
};

template <Comparable T>
void AvlTreeNode<T>::safeUpdateDepth(AvlTreeNode<T> *node) {
        if (node != nullptr) {
                node->depth = 1 + std::max(node->leftDepth(), node->rightDepth());
        }
}

template <Comparable T>
unsigned AvlTreeNode<T>::leftDepth() const {
        if (nullptr == left) {
                return 0;
        }
        return left->depth;
}

template <Comparable T>
unsigned AvlTreeNode<T>::rightDepth() const {
        if (nullptr == right) {
                return 0;
        }
        return right->depth;
}

template <Comparable T>
int AvlTreeNode<T>::depthDiff() const {
        return static_cast<int>(leftDepth()) - static_cast<int>(rightDepth());
}

template <Comparable T>
AvlTreeNode<T>::Balance AvlTreeNode<T>::getBalance() const {
        const int rootDepthDiff = depthDiff();
        if (std::abs(rootDepthDiff) < 2) {
                return Balance::Balanced;
        }

        if (rootDepthDiff > 1) {
                const int childDepthDiff = left->depthDiff();
                if (childDepthDiff > 0) {
                        return Balance::LeftLeft;
                }
                return Balance::LeftRight;
        }
        if (rootDepthDiff < -1) {
                const int childDepthDiff = right->depthDiff();
                if (childDepthDiff > 0) {
                        return Balance::RightLeft;
                }
                return Balance::RightRight;
        }
        return Balance::RightRight;
}

template <Comparable T>
AvlTreeNode<T> *AvlTreeNode<T>::rotateLeft() {
        if (nullptr == right) {
                throw AvlTreeException("Left rotation without a right child node");
        }

        AvlTreeNode *k = right;
        AvlTreeNode *t2 = k->left;

        this->right = t2;
        k->left = this;

        safeUpdateDepth(this);
        safeUpdateDepth(k);
        return k;
}

template <Comparable T>
AvlTreeNode<T> *AvlTreeNode<T>::rotateRight() {
        if (nullptr == left) {
                throw AvlTreeException("Right rotation without a left child node");
        }

        AvlTreeNode *k = left;
        AvlTreeNode *t2 = k->right;

        this->left = t2;
        k->right = this;

        safeUpdateDepth(this);
        safeUpdateDepth(k);
        return k;
}

template <Comparable T>
AvlTreeNode<T>::AvlTreeNode(T val) : value(val) {
}

template <Comparable T>
AvlTreeNode<T> *AvlTreeNode<T>::insert(AvlTreeNode *node, T value) {
        if (node == nullptr) {
                return new AvlTreeNode<T>(value);
        }

        if (value < node->value) {
                node->left = insert(node->left, value);
        } else if (value > node->value) {
                node->right = insert(node->right, value);
        } else {
                throw AvlTreeException("Inserting a duplicate value inside AVL tree");
        }

        safeUpdateDepth(node);

        const Balance balance = node->getBalance();
        switch (balance) {
                case Balance::Balanced:
                        return node;
                case Balance::LeftLeft:
                        return node->rotateRight();
                case Balance::LeftRight:
                        node->left = node->left->rotateLeft();
                        return node->rotateRight();
                case Balance::RightLeft:
                        node->right = node->right->rotateRight();
                        return node->rotateLeft();
                case Balance::RightRight:
                        return node->rotateLeft();
        }
        return node;
}

template <Comparable T>
template <typename Comparator>
        requires Comparable<T, Comparator>
Option<const T &> AvlTreeNode<T>::find(const AvlTreeNode *node, const Comparator &searchVal) {
        if (nullptr == node) {
                return Option<const T &>();
        }

        if (node->value == searchVal) {
                return Option<const T &>(node->value);
        }
        if (node->value > searchVal) {
                return find(node->left, searchVal);
        }
        return find(node->right, searchVal);
}

template <Comparable T>
void AvlTree<T>::insert(T value) {
        root = AvlTreeNode<T>::insert(root, value);
}

template <Comparable T>
template <typename F>
        requires Comparable<T, F>
Option<const T &> AvlTree<T>::find(const F &searchVal) const {
        return AvlTreeNode<T>::find(root, searchVal);
}

#endif
