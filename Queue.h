#include <iostream>
#include "Node.h"
using namespace std;

#ifndef QUEUE_H
#define QUEUE_H
template<typename T>
class Queue
{
	public:
		Node<T>* front;
		Node<T>* back;
		int count;

		Queue<T>();
		void enqueue(T*);
		T* dequeue();
		T* removeBack();
		T* find(string);
		bool empty();
		bool contains(T*);
		int indexOf(T*);
};

#include "Queue.tpp"
#endif
