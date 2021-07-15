#pragma once
template <typename T>
class LinkedList
{
private:

	class Node {
	public:
		T data;
		Node* next;
	};
	typedef Node* NodePtr;
	NodePtr head, tail;
public:
	LinkedList();
	~LinkedList();
	void pushback(T& obj);
	void pushfront(T& obj);
	T* Begin(void);
	T* End(void);
	void popback(void);
	void popfront(void);
	void insert(T& obj, unsigned pos);
	int Length(void);
	T operator[](unsigned num);
};
