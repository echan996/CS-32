#include "Multiset.h"
#include <iostream>
using namespace std;
Multiset::Multiset(){
	head = nullptr;
	end = nullptr;
	m_uniqueSize = 0;
	m_size = 0;
}
Multiset::~Multiset(){
	Node* ptr = head;
	while (ptr!=nullptr){
		Node* temp = ptr->front;
		delete ptr;
		ptr = temp;
	}
	m_uniqueSize = 0;
	m_size = 0;
}
Multiset::Multiset(const Multiset& ms){
	head = nullptr;
	end = nullptr;
	m_uniqueSize = ms.m_uniqueSize;
	m_size = ms.m_size;
	if (ms.uniqueSize() == 0)
		return;
	
	head = new Node;						//set head
	head->back = nullptr;
	head->front = nullptr;
	head->m_count = ms.head->m_count;
	head->m_value = ms.head->m_value;

	Node* temp1 = head;
	Node* temp2 = ms.head->front;
	while (temp2 != nullptr){				//set all pointers
		Node* assign = new Node;
		assign->back = temp1;
		assign->front = nullptr;
		assign->m_count = temp2->m_count;
		assign->m_value = temp2->m_value;
		temp1->front = assign;
		temp1 = assign;
		temp2 = temp2->front;
	}
	end = temp1;							//set end pointer
	end->front = nullptr;
}
Multiset& Multiset::operator= (const Multiset &ms){
	if (this == &ms)						//avoid aliasing
		return *this;

	Node* ptr = head;						//delete
	while (ptr != nullptr){
		Node* temp = ptr->front;
		delete ptr;
		ptr = temp;
	}

	m_uniqueSize = ms.m_uniqueSize;
	m_size = ms.m_size;
	head = nullptr;
	end = nullptr;
	if (ms.uniqueSize() == 0)
		return *this;

	head = new Node;						//set head
	head->back = nullptr;
	head->front = nullptr;
	head->m_count = ms.head->m_count;
	head->m_value = ms.head->m_value;

	Node* temp1 = head;
	Node* temp2 = ms.head->front;
	while (temp2 != nullptr){				//set values
		Node* assign = new Node;
		assign->back = temp1;
		assign->front = nullptr;
		assign->m_count = temp2->m_count;
		assign->m_value = temp2->m_value;
		temp1->front = assign;
		temp1 = assign;
		temp2 = temp2->front;
	}
	end = temp1;
	end->front = nullptr;
	return *this;
}
 
bool Multiset::insert(const ItemType& value){
	if (head == nullptr){				//if its empty,assign head
		head = new Node;
		head->back = nullptr;
		head->front = nullptr;
		head->m_value = value;
		head->m_count = 1;
		m_size++;
		m_uniqueSize++;
		end = head;
		return true;
	}
	Node* ptr = head;
	for (ptr; ptr != nullptr; ptr = ptr->front){		//if it exists, add one to the count
		if (ptr->m_value == value){
			ptr->m_count++;
			m_size++;
			return true;
		}
	}
	Node* ptr2 = new Node;							//add it to the end
	ptr2->back = end;
	end->front = ptr2;
	ptr2->front = nullptr;
	end = ptr2;
	ptr2->m_count = 1;
	ptr2->m_value = value;
	m_uniqueSize++;
	m_size++;
	return true;
}

void Multiset::dump(){
	for (Node* ptr = head; ptr != nullptr; ptr = ptr->front)
		cout << ptr->m_value << "  " << ptr->m_count<<endl;
	
}

int Multiset::eraseAll(const ItemType& value){
	if (count(value) == 0)
		return 0;
	int count = 0;
	Node* ptr = head;
	if (ptr->m_value == value){			//if its the head, delete it and change the head pointer
		if (ptr->front != nullptr){
			ptr->front->back = nullptr;
			head = ptr->front;
		}
		count = ptr->m_count;
		delete ptr;
		m_size -= count;
		m_uniqueSize--;
		if (m_uniqueSize == 0){
			head = end = nullptr;
		}
		return count;
	}
	for (; ptr->front!= nullptr; ptr = ptr->front){		//if its in the middle, delete it and relink the surrounding objects
		if (ptr->m_value == value){
			ptr->back->front = ptr->front;
			ptr->front->back = ptr->back;
			count = ptr->m_count;
			delete ptr;
			m_size-=count;
			m_uniqueSize--;
			return count;
		}
	}
	ptr = end;
	if (ptr->m_value == value){							//if its the end, delete and set the second last pointer to the new end
		ptr->back->front = nullptr;
		end = ptr->back;
		count = ptr->m_count;
		delete ptr;
		m_size -= count;
		m_uniqueSize--;
		return count;
	}
	return 0;
}

int Multiset::erase(const ItemType& value){
	if (count(value) == 1 || count(value) == 0)
		return eraseAll(value);										//send to eraseAll
	for (Node* ptr = head; ptr != nullptr;ptr = ptr->front)			//decrement by one
		if (ptr->m_value == value){
		ptr->m_count--;
		return 1;
		}

	return 0;
}

int Multiset::count(const ItemType& value) const{
	for (Node* ptr = head; ptr != nullptr; ptr = ptr->front)
		if (ptr->m_value == value)
		return ptr->m_count;
	return 0;
}

bool Multiset::contains(const ItemType& value) const{
	return count(value) > 0;
}

int Multiset::get(int i, ItemType& value) const{
	Node* ptr = head;
	if (i >= uniqueSize()||i<0)
		return 0;
	for (int a = 0; ptr != nullptr;ptr = ptr->front,a++)
		if (a == i){
			value = ptr->m_value;
			return ptr->m_count;
		}
	return 0;
}

void Multiset::swap(Multiset& other){
	Node* tHead = head;								//swap the pointers and all the data
	Node* tTail = end;
	int t1 = m_uniqueSize;
	int t2 = m_size;
	head = other.head;
	end = other.end;
	m_uniqueSize = other.m_uniqueSize;
	m_size = other.m_size;
	other.head = tHead;
	other.end = tTail;
	other.m_uniqueSize = t1;
	other.m_size = t2;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result){
	Multiset temp1 = ms1;					//temp arrays
	Multiset temp2 = ms2;
	Multiset tResult;
	ItemType b;
	for (int i = 0; i < temp1.uniqueSize(); i++){					//reassign values to new array
		for (int a = 0; a < temp1.get(i, b); a++)
			tResult.insert(b);
	}
	for (int i = 0; i < temp2.uniqueSize(); i++){
		for (int a = 0; a < temp2.get(i, b); a++)
			tResult.insert(b);
	}
	result = tResult;
}
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result){
	Multiset temp1 = ms1;						//temp arrays
	Multiset temp2 = ms2;
	ItemType b;
	for (int i = 0; i < temp2.uniqueSize(); i++){		//reassign to new array
		for (int a = 0; a < temp2.get(i, b); a++)
			temp1.erase(b);		
	}
	result = temp1;
}