#include "Queue.h"
#include <iostream>
using namespace std;

template<typename T>
T* Queue<T>::removeBack()
{
	count--;
	if(front == back)
	{
		T* temp = front->value;
		front = NULL;
		back = NULL;
		return temp;
	}
	else
	{
		Node<T>* current = front;
		while(current->next->next)
		{
			current = current->next;
		}
		T* temp = current->next->value;
		current->next = NULL;
		return temp;
	}
}

template<typename T>
int Queue<T>::indexOf(T* val)
{
	int count = 0;
	for(Node<T>* current = front; current; current = current->next)
	{
		if(*current->value == *val) return count;
		count++;
	}
	return -1;
}

template<typename T>
bool Queue<T>::contains(T* val)
{
	for(Node<T>* current = front; current; current = current->next)
	{
		if(*current->value == *val) return true;
	}
	return false;
}

template<typename T>
bool Queue<T>::empty()
{
	return front == NULL;
}

template<typename T>
Queue<T>::Queue()
{
	front = NULL;
	back = NULL;
	count = 0;
}

template<typename T>
void Queue<T>::enqueue(T* obj)
{
	count++;
	Node<T> * n = new Node<T>(obj);
	
	if(front == NULL)
	{
		front = n;
		back = n;
	}
	else
	{
		back->next = n;
		back = n;
	}
}

template<typename T>
T* Queue<T>::dequeue()
{
	count--;
	Node<T> * temp = front;
	front = front->next;
	return temp->value;
}

template<typename T>
T* Queue<T>::find(string name)
{
	for( Node<T> * current = front; current; current = current->next)
	{
		if(current->value->name.compare(name) == 0) return current->value;
	}
	return NULL;
}
