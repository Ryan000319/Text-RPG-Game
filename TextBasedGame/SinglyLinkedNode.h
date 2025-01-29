#pragma once
#include <stdexcept>

template<class DataType>
class SinglyLinkedNode {
public:
    typedef SinglyLinkedNode<DataType> Node;

private:
    DataType fValue;
    Node* fNext;

public:
    // Corrected constructor
    SinglyLinkedNode(const DataType& aValue, SinglyLinkedNode* aNext)
        : fValue(aValue), fNext(aNext) {}

    // Methods to manipulate the list
    void append(Node* aNode);
    void remove();
    int size() const;
    void setNext(Node* nextNode);
    DataType* get(int index);

    // Accessors
    DataType& getValue() { return fValue; }
    Node* getNext() const { return fNext; }

    // Check if the node is the end of the list
    bool isEnd() const { return fNext == nullptr; }
};

template<class DataType>
void SinglyLinkedNode<DataType>::append(Node* aNode) {
    if (!aNode) return;

    Node* current = this;
    while (current->fNext != nullptr) {
        current = current->fNext;
    }
    current->fNext = aNode;
};

template<class DataType>
void SinglyLinkedNode<DataType>::remove() {
    if (!fNext) return;

    Node* temp = fNext;
    fNext = fNext->fNext;
    delete temp;
};

template<class DataType>
int SinglyLinkedNode<DataType>::size() const {
    int count = 0;
    const Node* current = this;
    while (current != nullptr) {
        count++;
        current = current->fNext;
    }
    return count;
};

template<class DataType>
DataType* SinglyLinkedNode<DataType>::get(int index) {
    Node* current = this;
    int currentIndex = 0;

    while (current != nullptr && currentIndex < index) {
        current = current->fNext;
        currentIndex++;
    }

    if (current == nullptr) {
        return nullptr; // Index out of range
    }

    return &(current->fValue);
};

template<class DataType>
void SinglyLinkedNode<DataType>::setNext(Node* nextNode) {
    fNext = nextNode;
};

template<class DataType>
int countNodes(const SinglyLinkedNode<DataType>* head) {
    int count = 0;
    const SinglyLinkedNode<DataType>* currentNode = head;
    while (currentNode != nullptr) {
        count++;
        currentNode = currentNode->getNext();
    }
    return count;
};
