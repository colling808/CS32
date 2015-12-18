
#include "Multiset.h"

Multiset::Multiset()            // constructor
{
    head = nullptr;
    total = 0;
}

Multiset::~Multiset()           // deconstructor
{
    while (head != nullptr)         // traverse through each node and delete each ptr
    {
        Node *n = head->next;
        delete head;
        head = n;
    }
}

Multiset::Multiset(const Multiset& ms)     // copy constructor
{
 
    Node* ptr = ms.head;
    
    while (ptr != nullptr)
    {
        
        for(int k = 0; k < ptr->tally; k++)
            insert(ptr->value);
        
        ptr = ptr->next;
    }
    
    total = ms.total;
    
}

Multiset &Multiset::operator=(const Multiset& ms)   //assignment operator
{
    if(&ms == this)             // does nothing if assigning variable to itself
        return(*this);
    
    while (head != nullptr)
    {
        Node *n = head->next;
        delete head;
        head = n;
    }
    
    Node* ptr = ms.head;
    
    while (ptr != nullptr)          // traverse and insert each value into multiset
    {
        
        for(int k = 0; k < ptr->tally; k++)
            insert(ptr->value);
        
        ptr = ptr->next;
    }
    
    total = ms.total;       // copy total
    
    return (*this);
}

bool Multiset::empty() const
{
    return (head == nullptr);
}

int Multiset::size() const
{
    return total;                   // return the running total
}

int Multiset::uniqueSize() const
{
    int uniqueTotal = 0;
    Node* ptr = head;
    
    while(ptr != nullptr)
    {
        uniqueTotal++;              // counts the number of nodes as ptr traverses through list
        ptr = ptr->next;
    }
    
    return uniqueTotal;
}

bool Multiset::insert(const ItemType& value)
{
    if (empty())                                // CASE: if multiset is empty
    {
        Node* ptr = new Node;
        
        ptr->value = value;
        ptr->tally = 1;
        total++;
        
        ptr->next = head;
        ptr->prev = nullptr;
        head = ptr;
        
        return true;
    }
    
    if (contains(value))                        // CASE: if value is already in multiset
    {
        Node* ptr = head;
        while (ptr != nullptr)
        {
            if(ptr->value == value)
            {
                ptr->tally++;
                total++;
                return true;
            }
            ptr = ptr->next;
        }
    }
   
    Node *ptr = head;                      // CASE: value is not in an empty multiset, add to en

    while (ptr->next != nullptr)
        ptr = ptr->next;
    
    Node *n = new Node;
    n->value = value;
    n->tally = 1;
    n->next = nullptr;
    n->prev = ptr;
    ptr->next = n;
    total++;
    
    return true;
}

int Multiset::erase(const ItemType& value)
{
    if(empty() || !contains(value))             //CASE: if multiset is empty OR doesn't contain value
        return 0;
    
    
    Node* ptr = head;
    
    while (ptr->next != nullptr)
    {
        if(ptr->value == value)
            break;
        
        ptr = ptr->next;
    }
    
    if(ptr->tally == 1)                        //CASE: if multiset has multiple cases of value
    {
        total--;
        
        if(ptr->prev == nullptr)
            head = ptr->next;
        else
            ptr->prev->next = ptr->next;
        
        if(ptr->next == nullptr)
            ptr->prev = nullptr;
        else
            ptr->next->prev = ptr->prev;
        
        delete ptr;
        
        return 1;
        
    }
    
    if(ptr->tally > 1)                         //CASE: if multiset only has one case of value
    {
        ptr->tally--;
        total--;
    }
    
    return 1;
}

int Multiset::eraseAll(const ItemType& value)
{
    int totalErased = 0;
    
    if(empty() || !contains(value))             //CASE: if multiset is empty OR doesn't contain value
        return totalErased;
    
    Node* ptr = head;
    
    while (ptr->next != nullptr)
    {
        if(ptr->value == value)
            break;
        
        ptr = ptr->next;
    }
    
    totalErased = ptr->tally;                   //CASE: if multiset contains value
    
    if(ptr->prev == nullptr)
        head = ptr->next;
    else
        ptr->prev->next = ptr->next;
    
    if(ptr->next == nullptr)
        ptr->prev = nullptr;
    else
        ptr->next->prev = ptr->prev;
    
    delete ptr;
    
    total -= totalErased;
    
    return totalErased;
    
}

bool Multiset::contains(const ItemType& value) const
{
    
    if (empty())                // if list is empty, then it will not contain anything, therefore return 0.
        return 0;
    
    Node* ptr = head;

    while (ptr != nullptr)                              // traverse to find value
    {
        if(ptr->value == value && ptr->tally > 0)
            return true;
        
        ptr = ptr->next;
    }
    return false;
}

int Multiset::count(const ItemType& value) const
{
    if(!contains(value))        // if value isn't in list, return 0
        return 0;
    
    
    Node* ptr = head;
    while(ptr->next != nullptr) // traverse through list to find node with value
    {
        if(ptr->value == value)
            break;
        
        ptr = ptr->next;
    }
    return (ptr->tally);        // return tally of node

}

int Multiset::get(int i, ItemType& value) const
{
    Node* ptr = head;
    if (i >= 0 && i < uniqueSize())     // make sure i is within the bounds of linked list, else return 0
    {
        for(int k = 0; k < i; k++)      // traverse to the ith node of linked list
        {
            ptr = ptr->next;
        }
        
        value = ptr->value;             // set value to value at ith node
        return ptr->tally;              // return tally of ith node
    }
    
    return 0;
}


void Multiset::swap(Multiset& other)        // Switch the head pointers and totals~ easy! :D
{
    Node* temp;
    int sub;
    
    sub = other.total;
    other.total = total;
    total = sub;
    
    temp = other.head;
    other.head = head;
    head = temp;
    
}


void Multiset::dump()                   // test function that outputs each value in linked list
{
    std::cerr << "---\n";
    Node* p = head;
    while (p != nullptr)
    {
        for(int k=0; k < p->tally; k++)
            std::cerr << p->value << std::endl;
        
        p = p->next;
    }
    
    std::cerr << "---\n";
}




void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    Multiset temp;      // use a temporary variable to prevent aliasing
    
    for(int i = 0;i < ms1.uniqueSize(); i++)    // loop through each node in ms1
    {
        ItemType x;
        ms1.get(i, x);                          // retrieve each item in ms1
        for(int j = 0; j < ms1.count(x); j++)
            temp.insert(x);                     // insert into temp "count" times
    }
    
    
    for(int i = 0;i < ms2.uniqueSize(); i++)    // loop through each node in ms2
    {
        ItemType x;
        ms2.get(i, x);                          // retrieve each item in ms2
        for(int j = 0; j < ms2.count(x); j++)
            temp.insert(x);                     //insert into temp "count" times
    }
    
    result = temp;
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    Multiset temp;  // use a temporary variable to prevent aliasing
    
    for(int i = 0;i < ms1.uniqueSize(); i++)
    {
        ItemType x;
        ms1.get(i, x);
        for(int j = 0; j < ms1.count(x); j++)
            temp.insert(x);
    }
    
    // similar to combine function, but second parameter is erased instead of inserted
    
    for(int i = 0;i < ms2.uniqueSize(); i++)
    {
        ItemType x;
        ms2.get(i, x);
        for(int j = 0; j < ms2.count(x); j++)
            temp.erase(x);
    }
    
    result = temp;
    
}



