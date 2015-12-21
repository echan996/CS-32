// Multiset.h

#ifndef MULTISET_INCLUDED
#define MULTISET_INCLUDED
#include <string>
using namespace std;
// Later in the course, we'll see that templates provide a much nicer
// way of enabling us to have Multisets of different types.  For now, we'll
// use a typedef.

typedef string ItemType;

class Multiset
{
public:
	Multiset();     
	~Multiset();
	Multiset& operator= (const Multiset &ms);
	Multiset(const Multiset &ms);
	bool empty() const;  
	int size() const;
	int uniqueSize() const;
	bool insert(const ItemType& value);
	int erase(const ItemType& value);
	int eraseAll(const ItemType& value);
	bool contains(const ItemType& value) const;
	int count(const ItemType& value) const;
	int get(int i, ItemType& value) const;
	void swap(Multiset& other);
	void dump();

private:

	struct Node
	{
		ItemType m_value;
		int      m_count;
		Node* back;
		Node* front;
	};
	Node* head;  // the items in the multiset, with counts
	Node* end;
	int  m_uniqueSize;               // how many distinct items in the multiset
	int  m_size;                     // total number of items in the multiset

};

inline
int Multiset::size() const
{
	return m_size;
}

inline
int Multiset::uniqueSize() const
{
	return m_uniqueSize;
}

inline
bool Multiset::empty() const
{
	return size() == 0;
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);
void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);
#endif // MULTISET_INCLUDED
