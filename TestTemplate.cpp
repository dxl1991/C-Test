#include "TestTemplate.h"


template<class T> Stack<T>::Stack() {
	m_maxSize = 100;
	m_size = 0;
	m_pT = new T[m_maxSize];
}

template <class T>  Stack<T>::~Stack() {
	delete[] m_pT;
}

template <class T> void Stack<T>::push(T t) {
	m_size++;
	m_pT[m_size - 1] = t;

}
template <class T> T Stack<T>::pop() {
	T t = m_pT[m_size - 1];
	m_size--;
	return t;
}
template <class T> bool Stack<T>::isEmpty() {
	return m_size == 0;
}