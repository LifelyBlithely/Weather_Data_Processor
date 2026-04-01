#ifndef BST_H
#define BST_H

#include <iostream>

/**
 * @class Bst
 * @brief Template Binary Search Tree (BST)
 *
 * A generic BST that stores elements of type T.
 * Uses recursive algorithms for insertion, searching, traversal,
 * deletion, copying, and validation.
 *
 * Representation Invariant (RI):
 * - left subtree values < node value < right subtree values
 * - requires type T to support comparison operators (<, >, ==)
 */
template <class T>
class Bst
{
private:
    /**
     * @struct Node
     * @brief Internal node structure for BST.
     */
    struct Node
    {
        T data;
        Node* left;
        Node* right;

        Node(const T& value);
    };

    Node* root;

    // Recursive helpers
    Node* insert(Node* node, const T& value);
    bool search(Node* node, const T& value) const;

    void inOrder(Node* node, void (*visit)(const T&)) const;
    void preOrder(Node* node, void (*visit)(const T&)) const;
    void postOrder(Node* node, void (*visit)(const T&)) const;

    Node* remove(Node* node, const T& value, bool& removed);
    Node* findMin(Node* node) const;

    void deleteTree(Node*& node);
    void copyTree(Node*& copiedTreeRoot, Node* otherTreeRoot);

    bool debug(Node* node, const T* minValue, const T* maxValue) const;

public:
    Bst();
    Bst(const Bst& other);
    ~Bst();

    const Bst& operator=(const Bst& other);

    void insert(const T& value);
    bool search(const T& value) const;

    void inOrder(void (*visit)(const T&)) const;
    void preOrder(void (*visit)(const T&)) const;
    void postOrder(void (*visit)(const T&)) const;

    bool remove(const T& value);

    void deleteTree();
    bool debug() const;
};

// ===== Implementation =====

template <class T>
Bst<T>::Node::Node(const T& value): data(value), left(nullptr), right(nullptr)
{
}

template <class T>
Bst<T>::Bst() : root(nullptr)
{
}

template <class T>
Bst<T>::Bst(const Bst& other) : root(nullptr)
{
    copyTree(root, other.root);
}

template <class T>
Bst<T>::~Bst()
{
    deleteTree();
}

template <class T>
const Bst<T>& Bst<T>::operator=(const Bst& other)
{
    if (this != &other)
    {
        deleteTree();
        copyTree(root, other.root);
    }
    return *this;
}

// ===== INSERT (CREATE) =====
template <class T>
void Bst<T>::insert(const T& value)
{
    root = insert(root, value);
}

template <class T>
typename Bst<T>::Node* Bst<T>::insert(Node* node, const T& value)
{
    if (node == nullptr)
    {
        return new Node(value);
    }

    if (value < node->data)
    {
        node->left = insert(node->left, value);
    }
    else if (value > node->data)
    {
        node->right = insert(node->right, value);
    }

    return node;
}

// ===== SEARCH (READ) =====
template <class T>
bool Bst<T>::search(const T& value) const
{
    return search(root, value);
}

template <class T>
bool Bst<T>::search(Node* node, const T& value) const
{
    if (node == nullptr)
    {
        return false;
    }

    if (value == node->data)
    {
        return true;
    }

    if (value < node->data)
    {
        return search(node->left, value);
    }

    return search(node->right, value);
}

// ===== TRAVERSALS (READ) =====
template <class T>
void Bst<T>::inOrder(void (*visit)(const T&)) const
{
    if (visit != nullptr)
    {
        inOrder(root, visit);
    }
}

template <class T>
void Bst<T>::inOrder(Node* node, void (*visit)(const T&)) const
{
    if (node != nullptr)
    {
        inOrder(node->left, visit);
        visit(node->data);
        inOrder(node->right, visit);
    }
}

template <class T>
void Bst<T>::preOrder(void (*visit)(const T&)) const
{
    if (visit != nullptr)
    {
        preOrder(root, visit);
    }
}

template <class T>
void Bst<T>::preOrder(Node* node, void (*visit)(const T&)) const
{
    if (node != nullptr)
    {
        visit(node->data);
        preOrder(node->left, visit);
        preOrder(node->right, visit);
    }
}

template <class T>
void Bst<T>::postOrder(void (*visit)(const T&)) const
{
    if (visit != nullptr)
    {
        postOrder(root, visit);
    }
}

template <class T>
void Bst<T>::postOrder(Node* node, void (*visit)(const T&)) const
{
    if (node != nullptr)
    {
        postOrder(node->left, visit);
        postOrder(node->right, visit);
        visit(node->data);
    }
}

// ===== REMOVE ONE VALUE (DELETE) =====
template <class T>
bool Bst<T>::remove(const T& value)
{
    bool removed = false;
    root = remove(root, value, removed);
    return removed;
}

template <class T>
typename Bst<T>::Node* Bst<T>::remove(Node* node, const T& value, bool& removed)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (value < node->data)
    {
        node->left = remove(node->left, value, removed);
    }
    else if (value > node->data)
    {
        node->right = remove(node->right, value, removed);
    }
    else
    {
        removed = true;

        // Case 1: no children
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }

        // Case 2: one child (right only)
        if (node->left == nullptr)
        {
            Node* temp = node->right;
            delete node;
            return temp;
        }

        // Case 2: one child (left only)
        if (node->right == nullptr)
        {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: two children
        Node* successor = findMin(node->right);
        node->data = successor->data;
        node->right = remove(node->right, successor->data, removed);
    }

    return node;
}

template <class T>
typename Bst<T>::Node* Bst<T>::findMin(Node* node) const
{
    while (node != nullptr && node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

// ===== DELETE TREE (DELETE) =====
template <class T>
void Bst<T>::deleteTree()
{
    deleteTree(root);
}

template <class T>
void Bst<T>::deleteTree(Node*& node)
{
    if (node != nullptr)
    {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
        node = nullptr;
    }
}

// ===== COPY TREE =====
template <class T>
void Bst<T>::copyTree(Node*& copiedTreeRoot, Node* otherTreeRoot)
{
    if (otherTreeRoot == nullptr)
    {
        copiedTreeRoot = nullptr;
    }
    else
    {
        copiedTreeRoot = new Node(otherTreeRoot->data);
        copyTree(copiedTreeRoot->left, otherTreeRoot->left);
        copyTree(copiedTreeRoot->right, otherTreeRoot->right);
    }
}

// ===== DEBUG (RI CHECK) =====
template <class T>
bool Bst<T>::debug() const
{
    return debug(root, nullptr, nullptr);
}

template <class T>
bool Bst<T>::debug(Node* node, const T* minValue, const T* maxValue) const
{
    if (node == nullptr)
    {
        return true;
    }

    if (minValue != nullptr && !(node->data > *minValue))
    {
        return false;
    }

    if (maxValue != nullptr && !(node->data < *maxValue))
    {
        return false;
    }

    return debug(node->left, minValue, &(node->data)) &&
           debug(node->right, &(node->data), maxValue);
}

#endif
