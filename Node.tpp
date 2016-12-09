#include "Node.h"
#include <iostream>
using namespace std;

template<typename T>
Node<T>::Node(T* val)
{
	value=val;
	next = NULL;
}
