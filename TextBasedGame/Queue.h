#pragma once

#include "SinglyLinkedNode.h"
#include <stdexcept>


template <class T>
class Queue
{
private:
	SinglyLinkedNode<T>* front;
	SinglyLinkedNode<T>* rear;
	int size;

public:
	//Constructor
	Queue(): front(nullptr), rear(nullptr), size(0){}

	//Destructor
	~Queue()
	{
		while (!isEmpty())
		{
			dequeue();
		}
	}

	// Copy constructor
	Queue(const Queue<T>& other) : front(nullptr), rear(nullptr), size(0) {
		SinglyLinkedNode<T>* current = other.front;
		while (current != nullptr) {
			this->enqueue(current->getValue());
			current = current->getNext();
		}
	}

	// Copy assignment operator
	Queue<T>& operator=(const Queue<T>& other) {
		if (this != &other) { // Protect against self-assignment
			// Clear the current queue
			while (!this->isEmpty()) {
				this->dequeue();
			}

			// Copy elements from the other queue
			SinglyLinkedNode<T>* current = other.front;
			while (current != nullptr) {
				this->enqueue(current->getValue());
				current = current->getNext();
			}
		}
		return *this;
	}

	//Enqueue
	void enqueue(const T& value)
	{
		SinglyLinkedNode<T>* newNode = new SinglyLinkedNode<T>(value, nullptr);
		if (isEmpty())
		{
			front = rear = newNode;
		}

		else
		{
			rear->append(newNode);
			rear = rear->getNext();
		}
		size++; 
	}

	//Dequeue
	T dequeue()
	{
		if (isEmpty())
		{
			throw out_of_range("Queue is empty");
		}

		T value = front->getValue();
		SinglyLinkedNode<T>* toRemove = front;
		front = front->getNext();
		delete toRemove;
		size--;

		if (isEmpty())
		{
			rear = nullptr;
		}

		return value;
	}


	//Check if the queue is empty
	bool isEmpty() const
	{
		return front == nullptr;
	}

	//Get the size of the the queue
	int getSize() const
	{
		return size;
	}

	//Peek at the front of the queue
	T& peek() const
	{
		if (isEmpty())
		{
			throw out_of_range("Queue is empty");
		}

		return front->getValue();
	}
};