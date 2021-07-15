#include "LinkedList.h"


template <typename T>
LinkedList<T>::LinkedList() {
	this->head = this->tail = nullptr;
}

template <typename T>
LinkedList<T>::~LinkedList() {
	for (NodePtr it = this->heads; it != nullptr;) {
		this->head = this->head->next;
		delete it;
		it = this->head;
	}
}
template <typename T>
T* LinkedList<T>::Begin(void) {
	return this->head;
}
template <typename T>
T* LinkedList<T>::End(void) {
	return this->tail;
}

template<typename T>
void LinkedList<T>::popback(void)
{
	NodePtr newtail;
	for (NodePtr temp = this->head;; temp = temp->next) {
		if (temp->next == this->tail) {
			newtail = temp;
			break;
		}
	}
	delete this->tail;
	this->tail = newtail;
	newtail->next = nullptr;
}

template<typename T>
void LinkedList<T>::popfront(void)
{
	NodePtr temp = this->head->next;
	delete this->head;
	this->head = temp;
}

template<typename T>
void LinkedList<T>::insert(T& obj, unsigned pos)
{
	if (pos > this->Length()) {
		throw "position to insert is more than maximum length";
		return;
	}
	if (pos == 0) {
		this->pushfront(obj);
		return;
	}
	if (pos == this->Length()) {
		this->pushback(obj);
		return;
	}
	NodePtr temp_prev;
	int i = 1;
	for (NodePtr temp = this->head;temp != nullptr, i < pos;temp = temp->next) {
		if (i == pos) {
			obj->next = temp->next;
			temp->next = obj;
			return;
		}
	}
}

template<typename T>
int LinkedList<T>::Length(void)
{
	if (this->head == nullptr) {
		return 0;
	}
	int i = 0;
	for (NodePtr temp = this->head; temp != nullptr; temp = temp->next) {
		++i;
	}
	return i;
}

template<typename T>
T LinkedList<T>::operator[](unsigned num)
{
	if (num < 0 || num > this->Length()) {
		throw "index out of range";
		return nullptr;
	}
	unsigned count = 0;
	for (NodePtr temp = this->head; temp != nullptr; temp = temp->next) {
		if (count == num) {
			return *temp;
		}
		++count;
	}
	return ;
}

template <typename T>
void LinkedList<T>::pushback(T& obj) {
	if (this->head == nullptr) {
		this->head = this->tail = obj;
		return;
	}
	this->tail->next = obj;
	this->tail = this->tail->next;
}

template <typename T>
void LinkedList<T>::pushfront(T& obj) {
	if (this->head == nullptr) {
		this->head = this->tail = obj;
		return;
	}
	obj->next = this->head;
	this->head = obj;
}
