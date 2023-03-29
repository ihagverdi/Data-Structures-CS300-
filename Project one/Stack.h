#pragma once
//Stack linked-list implementation
//Hagverdi Ibrahimli

template<class Object>
class Stack {
public:
	Stack() {
		this->topOfStack = NULL;
		size = 0;
	}
	Stack(const Stack& rhs) {
		*this = rhs;
	}
	~Stack() {
		this->makeEmpty();
		size = 0;
	}

	void push(const Object& obj) {
		this->topOfStack = new ListNode(obj, this->topOfStack);
		size++;
	}
	void pop() {
		ListNode* dummy = this->topOfStack;
		this->topOfStack = this->topOfStack->next;
		delete dummy;
		size--;
	}

	const Object& topAndPop() {
		Object obj = this->top();
		this->pop();
		return obj;
	}

	const int getSize() const {
		return this->size;
	}
	const Object& top() const {
		return this->topOfStack->obj;
	}
	bool isEmpty() const {
		return this->topOfStack == NULL;
	}
	void makeEmpty() {
		while (!this->isEmpty()) {
			this->pop();
		}
		//topOfStack is NULL atp
	}

	const Stack& operator= (const Stack& rhs) {
		//deep copy
		if (*this == rhs)
			return *this;

		this->makeEmpty();

		if (rhs.isEmpty())
			return *this;

		topOfStack = new ListNode(rhs.topOfStack->obj);
		ListNode* ptr1 = topOfStack;
		ListNode* ptr2 = rhs.topOfStack->next;

		while (ptr2 != NULL) {
			ptr1->next = ListNode(ptr2->obj);
			ptr1 = ptr1->next;
			ptr2 = ptr2->next;
		}
		size = rhs.size;
		return *this;
	}

private:
	struct ListNode {
		Object obj;
		ListNode* next;
		
		ListNode(const Object& o, ListNode* n):
			obj(o), next(n){}
	};
	ListNode* topOfStack;
	int size;
};
