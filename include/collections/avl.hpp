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

template <vortex::Comparable T>
class AvlTree;

template <vortex::Comparable T>
class AvlTreeNode {
       private:
        enum class Balance;

       private:
        T value;
        AvlTreeNode<T> *left = nullptr;
        AvlTreeNode<T> *right = nullptr;
        unsigned depth = 1;

       private:
        AvlTreeNode(T);
        AvlTreeNode(const AvlTreeNode &)
                requires vortex::Cloneable<T>;
        AvlTreeNode(AvlTreeNode &&) noexcept
                requires vortex::Moveable<T>;
        ~AvlTreeNode();

        AvlTreeNode &operator=(const AvlTreeNode &)
                requires vortex::Cloneable<T>;
        AvlTreeNode &operator=(AvlTreeNode &&) noexcept
                requires vortex::Moveable<T>;

        static AvlTreeNode *insert(AvlTreeNode *, T);

        template <typename Comparator>
                requires vortex::Comparable<T, Comparator>
        static Option<const T &> find(const AvlTreeNode *, const Comparator &);

       private:
        void clone(const AvlTreeNode &)
                requires vortex::Cloneable<T>;
        void move(AvlTreeNode &&) noexcept
                requires vortex::Moveable<T>;
        void free();
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

template <vortex::Comparable T>
class AvlTree {
       private:
        AvlTreeNode<T> *root = nullptr;

        void copy(const AvlTree &)
                requires vortex::Cloneable<T>;
        void move(AvlTree &&) noexcept
                requires vortex::Moveable<T>;
        void free();

       public:
        AvlTree() = default;
        AvlTree(const AvlTree &)
                requires vortex::Cloneable<T>;
        AvlTree(AvlTree &&) noexcept
                requires vortex::Moveable<T>;
        ~AvlTree();

        AvlTree &operator=(const AvlTree &)
                requires vortex::Cloneable<T>;
        AvlTree &operator=(AvlTree &&) noexcept
                requires vortex::Moveable<T>;

        void insert(T value);

        template <typename Comparator>
                requires vortex::Comparable<T, Comparator>
        Option<const T &> find(const Comparator &) const;
};

template <vortex::Comparable T>
AvlTreeNode<T>::AvlTreeNode(T val) : value(val) {
}

template <vortex::Comparable T>
AvlTreeNode<T>::AvlTreeNode(const AvlTreeNode &other)
        requires vortex::Cloneable<T>
{
        clone(other);
}

template <vortex::Comparable T>
AvlTreeNode<T>::AvlTreeNode(AvlTreeNode &&other) noexcept
        requires vortex::Moveable<T>
{
        move(std::move(other));
}

template <vortex::Comparable T>
AvlTreeNode<T>::~AvlTreeNode() {
        free();
}

template <vortex::Comparable T>
AvlTreeNode<T> &AvlTreeNode<T>::operator=(const AvlTreeNode &other)
        requires vortex::Cloneable<T>
{
        if (this != &other) {
                free();
                clone(other);
        }
        return *this;
}

template <vortex::Comparable T>
AvlTreeNode<T> &AvlTreeNode<T>::operator=(AvlTreeNode &&other) noexcept
        requires vortex::Moveable<T>
{
        if (this != &other) {
                free();
                move(std::move(other));
        }
        return *this;
}

template <vortex::Comparable T>
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

template <vortex::Comparable T>
template <typename Comparator>
        requires vortex::Comparable<T, Comparator>
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

template <vortex::Comparable T>
void AvlTreeNode<T>::clone(const AvlTreeNode &other)
        requires vortex::Cloneable<T>
{
        value = other.value.clone();
        depth = other.depth.clone();
        if (other.left != nullptr) {
                left = new AvlTreeNode<T>(*other.left);
        } else {
                left = nullptr;
        }
        if (other.right != nullptr) {
                right = new AvlTreeNode<T>(*other.right);
        } else {
                right = nullptr;
        }
}

template <vortex::Comparable T>
void AvlTreeNode<T>::move(AvlTreeNode &&other) noexcept
        requires vortex::Moveable<T>
{
        value = std::move(other.value);
        depth = other.depth;
        left = other.left;
        right = other.right;
        other.left = nullptr;
        other.right = nullptr;
        other.depth = 0;
}

template <vortex::Comparable T>
void AvlTreeNode<T>::free() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
        depth = 0;
}

template <vortex::Comparable T>
void AvlTreeNode<T>::safeUpdateDepth(AvlTreeNode<T> *node) {
        if (node != nullptr) {
                node->depth = 1 + std::max(node->leftDepth(), node->rightDepth());
        }
}

template <vortex::Comparable T>
unsigned AvlTreeNode<T>::leftDepth() const {
        if (nullptr == left) {
                return 0;
        }
        return left->depth;
}

template <vortex::Comparable T>
unsigned AvlTreeNode<T>::rightDepth() const {
        if (nullptr == right) {
                return 0;
        }
        return right->depth;
}

template <vortex::Comparable T>
int AvlTreeNode<T>::depthDiff() const {
        return static_cast<int>(leftDepth()) - static_cast<int>(rightDepth());
}

template <vortex::Comparable T>
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

template <vortex::Comparable T>
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

template <vortex::Comparable T>
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

template <vortex::Comparable T>
AvlTree<T>::AvlTree(const AvlTree &other)
        requires vortex::Cloneable<T>
{
        copy(other);
}

template <vortex::Comparable T>
AvlTree<T>::AvlTree(AvlTree &&other) noexcept
        requires vortex::Moveable<T>
{
        move(std::move(other));
}

template <vortex::Comparable T>
AvlTree<T>::~AvlTree() {
        free();
}

template <vortex::Comparable T>
AvlTree<T> &AvlTree<T>::operator=(const AvlTree &other)
        requires vortex::Cloneable<T>
{
        if (this != &other) {
                free();
                copy(other);
        }
        return *this;
}

template <vortex::Comparable T>
AvlTree<T> &AvlTree<T>::operator=(AvlTree &&other) noexcept
        requires vortex::Moveable<T>
{
        if (this != &other) {
                free();
                move(std::move(other));
        }
        return *this;
}

template <vortex::Comparable T>
void AvlTree<T>::insert(T value) {
        root = AvlTreeNode<T>::insert(root, value);
}

template <vortex::Comparable T>
template <typename Comparator>
        requires vortex::Comparable<T, Comparator>
Option<const T &> AvlTree<T>::find(const Comparator &searchVal) const {
        return AvlTreeNode<T>::find(root, searchVal);
}

template <vortex::Comparable T>
void AvlTree<T>::copy(const AvlTree &other)
        requires vortex::Cloneable<T>

{
        root = other.root;
}

template <vortex::Comparable T>
void AvlTree<T>::move(AvlTree &&other) noexcept
        requires vortex::Moveable<T>
{
        this->root = other.root;
        other.root = nullptr;
}

template <vortex::Comparable T>
void AvlTree<T>::free() {
        delete root;
        root = nullptr;
}

#endif
