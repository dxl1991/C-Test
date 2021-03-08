#pragma once


template<class T> class Stack {
public:
	Stack();
	~Stack();
	void push(T t);
	T pop();
	bool isEmpty();
private:
	T* m_pT;
	int m_maxSize;
	int m_size;
};

template<typename T> void swap1(T& t1, T& t2)
{
	T temp;
	temp = t1;
	t1 = t2;
	t2 = temp;
}

