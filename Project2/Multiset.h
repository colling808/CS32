
#ifndef __Project_2__Multiset__
#define __Project_2__Multiset__

#include <iostream>
#include <string>

typedef std::string ItemType;

class Multiset
{
public:
    Multiset();                                 //constructor
    ~Multiset();                                //destructor
    Multiset(const Multiset& ms);               //copy constructor
    Multiset &operator=(const Multiset& ms);    //assignment operator
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
    
    void dump();         // Testing print function

private:
    
    struct Node
    {
        Node* prev;
        Node* next;
        ItemType value;
        int tally;
    };
    
    Node* head;
    int total;
};


// Non-member functions for Multiset
void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);



#endif /* defined(__Project_2__Multiset__) */
