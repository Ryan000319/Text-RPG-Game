#pragma once
#include <memory>
#include <stdexcept>

template <class T>
class BTree {
public:
    static std::unique_ptr<BTree<T>> NIL; // Sentinel

    // Constructors
    BTree(); // Empty BTree
    BTree(T aKey); // Root with 2 subtrees
    ~BTree() = default; 

    // Member functions
    bool isEmpty() const;
    const T& key() const;
    std::unique_ptr<BTree<T>>& left();
    std::unique_ptr<BTree<T>>& right();
    void attachLeft(std::unique_ptr<BTree<T>> aBTree);
    void attachRight(std::unique_ptr<BTree<T>> aBTree);
    std::unique_ptr<BTree<T>> detachLeft();
    std::unique_ptr<BTree<T>> detachRight();

private:
    std::unique_ptr<T> fKey; 
    std::unique_ptr<BTree<T>> fLeft;
    std::unique_ptr<BTree<T>> fRight;
};

// Static member initialization
template<class T>
std::unique_ptr<BTree<T>> BTree<T>::NIL = nullptr;

// Implementation of the BTree methods
template <class T>
BTree<T>::BTree() : fKey(nullptr), fLeft(nullptr), fRight(nullptr) {}

//Constructor for Root with 2 subtrees
template<class T>
BTree<T>::BTree(T aKey) : fKey(std::make_unique<T>(std::move(aKey))), fLeft(nullptr), fRight(nullptr) {}

//Check if Binary Tree is empty
template<class T>
bool BTree<T>::isEmpty() const {
    return fKey == nullptr;
}

//Return smart pointer
template <class T>
const T& BTree<T>::key() const {
    if (isEmpty()) throw std::logic_error("Empty tree has no key");
    return *fKey;
}

//Return left subtree
template<class T>
std::unique_ptr<BTree<T>>& BTree<T>::left() {
    if (isEmpty()) throw std::logic_error("Empty tree has no left child");
    return fLeft;
}

//Return right subtree
template<class T>
std::unique_ptr<BTree<T>>& BTree<T>::right() {
    if (isEmpty()) throw std::logic_error("Empty tree has no right child");
    return fRight;
}

//Attach to the left subtree
template<class T>
void BTree<T>::attachLeft(std::unique_ptr<BTree<T>> aBTree) {
    if (!isEmpty() && !fLeft) {
        fLeft = std::move(aBTree);
    }
    else {
        throw std::logic_error("Tree already has a left child or is empty");
    }
}

//Attach to the right subtree
template<class T>
void BTree<T>::attachRight(std::unique_ptr<BTree<T>> aBTree) {
    if (!isEmpty() && !fRight) {
        fRight = std::move(aBTree);
    }
    else {
        throw std::logic_error("Tree already has a right child or is empty");
    }
}

//Detach from left subtree
template <class T>
std::unique_ptr<BTree<T>> BTree<T>::detachLeft() {
    if (isEmpty()) {
        throw std::logic_error("Tree is empty");
    }
    return std::move(fLeft);
}

//Detach from right subtree
template <class T>
std::unique_ptr<BTree<T>> BTree<T>::detachRight() {
    if (isEmpty()) {
        throw std::logic_error("Tree is empty");
    }
    return std::move(fRight);
}
