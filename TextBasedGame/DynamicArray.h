#pragma once

template <typename T>
class dynaArr
{
private:
	unsigned cap;
	unsigned initialCap;
	unsigned numOfEl;
	T* *arr;

	void expand();
	void initialize(unsigned from);

public:
	dynaArr(unsigned size = 5);
	dynaArr(const dynaArr& obj);
	~dynaArr();

	T& operator [] (const unsigned index);
	void operator= (const dynaArr& obj);

	unsigned max_cap();
	unsigned size();
	void push (const T element);
	void remove(const unsigned index, bool ordered = false);
	void clear();
};

template <typename T>
dynaArr<T>::dynaArr(unsigned size)
{
	this->initialCap = size;
	this->cap = size;
	this->numOfEl = 0;

	this->arr = new T * [this->cap];

	this->initialize(0);

};

template <typename T>
dynaArr<T>::dynaArr(const dynaArr& obj)
{
	this->initialCap = obj.initialCap;
	this->cap = obj.cap;
	this->numOfEl = obj.numOfEl;

	this->arr = new T * [this->cap];

	for (size_t i = 0; i < this->numOfEl; i++)
	{
		this->arr[i] = new T(*obj.arr[i]);
	}

	this->initialize(0);

};

template <typename T>
dynaArr<T>::~dynaArr()
{
	for (size_t i = 0; i < this->numOfEl; i++)
	{
		delete this->arr[i];
	}

	delete[] arr;
};

template <typename T>
T& dynaArr<T>:: operator [] (const unsigned index)
{
	if (index < 0 || index >= this->numOfEl)
		throw "Out of Indexing Operator Bounds";

	return *this->arr[index];
};

template <typename T>
void dynaArr<T>:: operator = (const dynaArr& obj)
{
	for (size_t i = 0; i < this->numOfEl; i++)
	{
		delete this->arr[i];
	}

	delete[] arr;

	this->initialCap = obj.initialCap;
	this->cap = obj.cap;
	this->numOfEl = obj.numOfEl;

	this->arr = new T * [this->cap];

	for (size_t i = 0; i < this->numOfEl; i++)
	{
		this->arr[i] = new T(*obj.arr[i]);
	}

	this->initialize(0);
};

template <typename T>
void dynaArr<T>::expand()
{
	this->cap *= 2;
	T** tempArr = new T * [this->cap];

	for (size_t i = 0; i < this->numOfEl; i++)
	{
		tempArr[i] = this->arr[i];
	}
	delete[]arr;

	this->arr = tempArr;
	this->initialize(this->numOfEl);
};


template <typename T>
void dynaArr<T>::initialize(unsigned from)
{
	for (size_t i = from; i < this->cap; i++)
	{
		this->arr[i] = nullptr;
	}
};

template <typename T>
unsigned dynaArr<T>::max_cap()
{
	return this->cap;
};


template <typename T>
unsigned dynaArr<T>::size()
{
	return this->numOfEl;
};

template <typename T>
void dynaArr<T>::push(const T element)
{
	if (this->numOfEl >= this->cap)
		this->expand();

	this->arr[this->numOfEl++] = new T(element);
};

template <typename T>
void dynaArr<T>::remove(const unsigned index, bool ordered)
{
	if (index < 0 || index >= this->numOfEl)
		throw "Out of Bounds Remove";

	if (ordered)
	{
		delete this->arr[index];
		for (size_t i = 0; i < this->numOfEl - 1; i++)
		{
			this->arr[i] = this->arr[i + 1];
		}
		
		this->arr[--this->numOfEl] = nullptr;
	}

	else
	{
		delete this->arr[index];

		this->arr[index] = this->arr[this->numOfEl - 1];

		this->arr[--this->numOfEl] = nullptr;
	}
}

template <typename T>
void dynaArr<T>::clear()
{
	for (size_t i = 0; i < this->numOfEl; i++)
	{
		delete this->arr[i];
	}

	this->numOfEl = 0;
};


