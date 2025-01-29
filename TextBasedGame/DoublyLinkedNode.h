#pragma once

template<class DataType>
class DoublyLinkedNode {
public:
    typedef DoublyLinkedNode<DataType> Node;

    private:
    DataType fValue;
    Node* fNext;
    Node* fPrevious;

public:
    // Constructor for node with a value
    DoublyLinkedNode(const DataType& aValue) : fValue(aValue), fNext(nullptr), fPrevious(nullptr) {}

    // Methods to manipulate the list
    void prepend(Node* aNode);
    void append(Node* aNode);
    void remove();

    // Accessors
    const DataType& getValue() const { return fValue; }
    Node* getNext() const { return fNext; }
    Node* getPrevious() const { return fPrevious; }

    // Check if the node is the end of the list
    bool isEnd() const { return fNext == nullptr; }
};

template<class DataType>
void DoublyLinkedNode<DataType>::prepend(Node* aNode) {
    if (!aNode) return;

    aNode->fNext = this;
    aNode->fPrevious = fPrevious;

    if (fPrevious) {
        fPrevious->fNext = aNode;
    }

    fPrevious = aNode;
};

template<class DataType>
void DoublyLinkedNode<DataType>::append(Node* aNode) {
    if (!aNode) return;

    aNode->fPrevious = this;
    aNode->fNext = fNext;

    if (fNext) {
        fNext->fPrevious = aNode;
    }

    fNext = aNode;
};

template<class DataType>
void DoublyLinkedNode<DataType>::remove() {
    if (fPrevious) {
        fPrevious->fNext = fNext;
    }

    if (fNext) {
        fNext->fPrevious = fPrevious;
    }

    fNext = nullptr;
    fPrevious = nullptr;
};

