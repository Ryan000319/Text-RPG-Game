#pragma once

#include "SinglyLinkedNode.h"
#include <stdexcept>
#include <iostream> 

using namespace std;

template<class T>
class dynaStack
{
private:
	SinglyLinkedNode<T>* top;

public:
	dynaStack():top(nullptr){}

	~dynaStack()
	{
		clear();
	}

	//Add an element to top of the stack
	void push(const T& value)
	{
		top = new SinglyLinkedNode<T>(value, top);
	}

	//Return the top element from the stack without removing it
	T peek() const
	{
		if (isEmpty())
		{
			throw out_of_range("Stack is empty");
		}

		return top->getValue();
	}

	//Remove and return the top element from the stack
	T pop()
	{
		if (isEmpty())
		{
			throw out_of_range("Stack is empty");
		}

		T value = top->getValue();
		SinglyLinkedNode<T>* oldTop = top;
		top = top->getNext();
		delete oldTop;
		return value;
	}

	//Check if the stack is empty
	bool isEmpty() const
	{
		return top == nullptr;
	}

	//Clear the stack
	void clear()
	{
		while (!isEmpty())
		{
			pop();
		}
	}

	// Get the top node of the stack for traversal without modification
	SinglyLinkedNode<T>* getTopNode() const {
		return top;
	}


	

};

