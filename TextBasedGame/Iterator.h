#pragma once

#include "SinglyLinkedNode.h"

template <class DataType>
class Iterator
{
private:
	SinglyLinkedNode<DataType>* currentNode;

public:
	//Constructor
	Iterator(SinglyLinkedNode<DataType>* node = nullptr): currentNode(node){}

	//Dereference operator 
	DataType& operator*()
	{
		return currentNode->getValue();
	}

	//Prefix incremenet operator
	Iterator& operator++()
	{
		if (currentNode)
		{
			currentNode = currentNode->getNext();
		}
		return *this;
	}

	//Postfix increment operator
	Iterator operator++ (int)
	{
		Iterator temp = *this;
		++(*this);
		return this;
	}

	//Equality check operator
	bool operator==(const Iterator& other) const
	{
		return currentNode == other.currentNode;
	}

	bool operator!=(const Iterator& other) const
	{
		return !(*this == other);
	}
};