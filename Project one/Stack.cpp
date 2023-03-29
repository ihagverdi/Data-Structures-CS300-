/*
Implementation of the Stack class.
Written by Hagverdi Ibrahimli
27. March. 2023
*/

#include<iostream>


template<class Object>
class Stack {
public:
	Stack();
	Stack(const Stack& rhs);
	~Stack();

	void push(const Object& obj);
	void pop();

	const Object& topAndPop();

	const int getSize() const;
	const Object& top() const;
	bool isEmpty() const;
	void makeEmpty();

	const Stack& operator= (const Stack& rhs);

private:
	struct ListNode {
		Object obj;
		ListNode* next;

		ListNode(const Object& o, ListNode* n) :
			obj(o), next(n) {}
	};
	ListNode* topOfStack;
	int size;
};

template<class Object> Stack<Object>::Stack() {
	this->topOfStack = NULL;
}

template<class Object> Stack<Object>::Stack(const Stack& rhs) {
	*this = rhs;
}

template<class Object> void Stack<Object>::push(const Object& obj){
	this->topOfStack = new ListNode(obj, this->topOfStack);
}

template<class Object> void Stack<Object>::pop() {
	ListNode* dummy = this->topOfStack;
	this->topOfStack = this->topOfStack->next;
	delete dummy;
}

template<class Object> const Object& Stack<Object>::top() const {
	return this->topOfStack->obj;
}

template<class Object> bool Stack<Object>::isEmpty() const {
	return this->topOfStack==NULL;
}

template<class Object> void Stack<Object>::makeEmpty() {
	while (!this->isEmpty()) {
		this->pop();
	}
	//topOfStack is NULL atp
}

template<class Object> const Stack<Object>& Stack<Object>::operator= (const Stack& rhs) {
	//deep copy
	if (*this == rhs)
		return *this;

	this->makeEmpty();

	if (rhs.isEmpty())
		return *this;

	topOfStack = new ListNode(rhs.topOfStack->obj);
	ListNode* ptr1 = topOfStack;
	ListNode* ptr2 = rhs.topOfStack->next;
	
	while(ptr2 != NULL){
		ptr1->next = ListNode(ptr2->obj);
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	return *this;
}		

template<class Object> Stack<Object>::~Stack() {
	this->makeEmpty();
}

template<class Object> const Object& Stack<Object>::topAndPop() {
	Object obj = this->top();
	this->pop();
	return obj;
}

template<class Object> const int Stack<Object>::getSize() const {
	return this->size;
}
