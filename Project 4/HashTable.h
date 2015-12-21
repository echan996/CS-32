#include <iostream>
using namespace std;
template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity){
		cur_cap = 0;
		m_capacity = capacity;
		m_numBuckets = numBuckets;
		list = new Node*[numBuckets];
		for (int i = 0; i < numBuckets; i++)
			list[i] = nullptr;
		h_start = nullptr;
		h_end = nullptr;
	};
	~HashTable(){
		for (int i = 0; i < m_numBuckets; i++)
			for (Node* p = list[i]; p != nullptr;){
			Node* temp = p;
			p = p->next;
			delete temp;
			//cout<< "deleted";
			}
	};
	bool isFull()	const{ return cur_cap == m_capacity; };
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
	bool get(const KeyType& key,  ValueType& value)	const;
	bool touch(const KeyType& key);
	bool discard(KeyType& key, ValueType& value);
	
private:
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	struct Node{
		Node(){
			h_prev = nullptr;
			h_next = nullptr;
		}
		ValueType value;
		KeyType key;
		Node* next;
		Node* prev;
		Node* h_prev;
		Node* h_next;
		bool permanent = true;
	};
	int m_capacity, m_numBuckets, cur_cap;
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
	Node** list;
	unsigned int getBucketForKey(const KeyType& key) const
	{
		// The computeHash function must be defined
		// for each type of key that we use in some
		// hash table.
		unsigned int computeHash(KeyType); // prototype
		unsigned int result = computeHash(key);
		unsigned int bucketNum =
			determineBucketFromHash(result);
		return bucketNum;
	}
	unsigned int determineBucketFromHash(unsigned int a) const{
		return a%m_numBuckets;
	}
	Node* h_start;
	Node* h_end;
};

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent){
//case: same object
	
	unsigned int i = getBucketForKey(key);
	Node* ptr = list[i];
	if (ptr == nullptr && !isFull()){		//if its the first pointer, add it to the spot 
		Node* p = new Node();
		p->value = value;
		p->key = key;
		p->next = nullptr;
		p->prev = nullptr;
		list[i] = p;
		cur_cap++;
		
		if (!permanent){					//if it is permanent, add it to the history linked list
			if (h_start == nullptr)
				h_start = p;
			if (h_end == nullptr){
				h_end = p;
				return true;
			}
			p->h_prev = nullptr;
			p->h_next = h_end;
			h_end->h_prev = p;
			h_end = p;
			p->permanent = false;
		}
		
		return true;
	}
	for (;ptr!=nullptr;ptr = ptr->next){
		if (ptr->key == key){		//if the desired input is found, touch it and change the value
			ptr->value = value;
			touch(ptr->key);
			return true;
		}
		if (ptr->next == nullptr){	//else if the next object is a nullptr, break out of the iteration
			break;
		}
	}
	if (isFull()){
		return false;
	}
	Node* p = new Node();	//asign new node to the ptr->next spot
	p->value = value;
	p->key = key;
	p->next = nullptr;
	ptr->next = p;
	p->prev = ptr;
	cur_cap++;
	if (!permanent){		//if not permanent, add to the history linked list
		if (h_start == nullptr)
			h_start = p;
		if (h_end == nullptr){
			h_end = p;
			return true;
		}
		p->h_prev = nullptr;
		p->h_next = h_end;
		h_end->h_prev = p;
		h_end = p;
		p->permanent = false;
	}
	return true;
}
template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType& key,  ValueType& value) const{
	unsigned int i = getBucketForKey(key);	
	for (Node* p = list[i]; p != nullptr;){		//if its in the hashtable, change the value 
		if (p->key == key){
			value = p->value;
			return true;
		}
		p = p->next;
	}
	return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key){
	int i = getBucketForKey(key);
	for (Node* p = list[i]; p != nullptr;p = p->next)		//iterate through hash table
		if (p->key == key){	
		if (p == h_end)							//if its the last node that matches, return true no change needed
			return true;
		if (p->permanent){						//if the matching node is permanent, return false 
			return false;
		}
		if (p->h_prev != nullptr)				//else, reline in the linked list 
			p->h_prev->h_next = p->h_next;
		if (p->h_next != nullptr)
			p->h_next->h_prev = p->h_prev;
		if (p == h_start){
			h_start = p->h_prev;
		}
		p->h_prev = nullptr;
		p->h_next = h_end;
		h_end->h_prev = p;
		h_end = p;		
		return true;
		}	

	return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value){
	if (h_start == nullptr){
		return false;
	}
	int i = getBucketForKey(h_start->key);
	Node* p = list[i];
	if (p->key == h_start->key){		//if the first object is the start match, delete the first object in the array linked list
		list[i] = p->next;
		if (p->next!=nullptr)
		p->next->prev = nullptr;		//relink array linked list
		key = h_start->key;				//relink and reassign head/tail pointers for the history linked list
		value = h_start->value;
		if (p->h_prev != nullptr){
			p->h_prev->h_next = nullptr;
		}
		h_start = p->h_prev;
		if (p == h_end)
			h_end = nullptr;
		delete p;
		cur_cap--;
		return true;
	}
	for (; p != nullptr;p=p->next)		//else iterate through the linked list and find the object and perform the same relinkage 
		if (p->key == h_start->key){
		list[i] = p->next;
		if (p->next != nullptr){
			p->next->prev = nullptr;
		}
		key = h_start->key;
		value = h_start->value;
		if (p->h_prev != nullptr){
			p->h_prev->h_next = nullptr;
		}
		h_start = p->h_prev;
		if (p == h_end)
			h_end = nullptr;
		delete p;
		cur_cap--;
		return true;
		}
	return false;
}
