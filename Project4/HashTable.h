
#ifndef Project_4_HashTable_h
#define Project_4_HashTable_h
#include <iostream>
using namespace std;

template <typename KeyType, typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity);
    ~HashTable();
    bool isFull() const;
    bool set(const KeyType& key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value) const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key, ValueType& value);
    
private:
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    
    struct DLL // doubly linked list
    {
        DLL() {m_prev = m_next = nullptr;};
        KeyType m_key;
        DLL *m_prev, *m_next;
    };
    struct Node
    {
        Node() { n_next = nullptr; addr = nullptr; }
        KeyType n_key;
        ValueType n_value;
        bool m_permanent;
        Node* n_next;
        DLL* addr;
    };
    void sendToBack(DLL* curr, bool isNew);
    int n_buckets; // delete later
    int m_capacity; // the max capacity
    int m_size;     // the current size
    Node** m_buckets;   // an array of pointers
    DLL *m_head, *m_tail;
};

template <typename KeyType, typename ValueType>
HashTable <KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)
{
    n_buckets = numBuckets;
    m_buckets = new Node*[n_buckets];
    m_capacity = capacity;
    m_head = m_tail = nullptr;
    m_size = 0;
    
}


template <typename KeyType, typename ValueType>
HashTable <KeyType, ValueType>::~HashTable()
{
    for(int k = 0; k < n_buckets; k++)
    {
        if(m_buckets[k] == nullptr)
            continue;
        else
        {
            Node* p = m_buckets[k];
            while(p->n_next != nullptr)
            {
                Node* killMe = p;
                p = p->n_next;
                if(killMe != nullptr)
                    delete killMe;
                if(killMe->addr != nullptr)
                    delete killMe->addr;
            }
        }
    }
    delete [] m_buckets; // this sometimes causes program to crash
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const
{

    return m_capacity == m_size;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent)
{
    if(isFull())
        return false;
    
    unsigned int computeHash(KeyType key);
    unsigned hash = computeHash(key) % n_buckets;
    Node *addMe, *p = m_buckets[hash];
    
    if(p == nullptr)  // adds to front of linked list
    {
        m_buckets[hash] = addMe = new Node;
        addMe->n_value = value;
        addMe->n_key = key;
        addMe->n_next = nullptr;
        m_size++;
    }
    
    else if(p->n_key == key) // if the key is already there, update the value
    {
        p->n_value = value;
        if(!p->m_permanent)
           sendToBack(p->addr, false);
        return true;
    }
    else                            // add to rear of linked list
    {
        while(p->n_next != nullptr)
        {
            if(p->n_key == key) // if already in DLL, update and send to back of DLL
            {
                p->n_value = value;
                if(!p->m_permanent)
                    sendToBack(p->addr, false);
                return true;
            }
            p = p->n_next;
        }

        addMe = p->n_next = new Node;
        addMe->n_key = key;
        addMe->n_value = value;
        addMe->n_next = nullptr;
        m_size++;
    
    }
    
    if(!permanent)
    {
        addMe->m_permanent = false;
        addMe->addr = new DLL;
        addMe->addr->m_key = key;
        sendToBack(addMe->addr, true);
    }
    else
        addMe->m_permanent = true;
    
return true;

}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType &key, ValueType &value) const
{
    unsigned int computeHash(KeyType key);
    unsigned int hash = computeHash(key) % n_buckets;
    Node* curr = m_buckets[hash];
    
    while(curr != nullptr)
    {
        if(curr->n_key == key)
        {
            value = curr->n_value;
            return true;
        }
        curr = curr->n_next;
    }

    return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType &key)
{
    unsigned int computeHash(KeyType key);
    unsigned int hash = computeHash(key) % n_buckets;
    Node* curr = m_buckets[hash];
    
    while(curr != nullptr)
    {
        if(curr->n_key == key && !curr->m_permanent)
        {
            sendToBack(curr->addr, false);
            return true;
        }
        curr = curr->n_next;
    }
    return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType &key, ValueType &value)
{
    
    DLL* killMe = m_tail;   // first non-permanent delete from list
    
    if(killMe == nullptr)
        return false;   // returns false if there are no non-permanent items in list
    
    if(killMe == m_head)
        m_head = nullptr;
    else
    {
        m_tail = killMe->m_prev;
        m_tail->m_next = nullptr;
    }
    key = killMe->m_key;
    delete killMe;
    
    unsigned int computeHash(KeyType key);
    unsigned int hash = computeHash(key) % n_buckets;   // next, delete from hashtable
    Node* curr = m_buckets[hash];

    if(curr!= nullptr && curr->n_key == key)
    {
        Node* die = m_buckets[hash];
        value = die->n_value;
        m_buckets[hash] = curr->n_next;
        delete die;
        m_size--;
        return true;
    }
    
    while(curr != nullptr)
    {
        if(curr->n_next != nullptr && curr->n_next->n_key == key)
            break;
        curr = curr->n_next;
    }
    
    if(curr != nullptr)
    {
        Node* die = curr->n_next;
        value = die->n_value;
        curr->n_next = die->n_next;
        delete die;
    }
    m_size--;
    return true;
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType,ValueType>::sendToBack(DLL* curr, bool isNew)
{
    if(curr== nullptr) return;
    if(m_head == nullptr)          // list is empty, add to head
    {
        m_head = m_tail = curr;
        return;
    }
    if(curr == m_tail)        // item being touched is already at the back
        return;
    if(m_head == curr)   // case when item is the head
    {
        m_head = curr->m_next;
        m_head->m_prev = nullptr;
    }
    else if(!isNew)                    // case when item is in the middle
    {
        curr->m_prev->m_next = curr->m_next;
        curr->m_next->m_prev = curr->m_prev;
    }
    
    curr->m_prev = m_tail;
    m_tail->m_next = curr;
    m_tail = curr;
    m_tail->m_next = nullptr;
    return;

}


//
//template <typename KeyType, typename ValueType>
//void HashTable<KeyType, ValueType>::print()                   // testing print() function
//{
//    //prints each hash
//    for(int k = 0; k < n_buckets; k++)
//    {
//        if(m_buckets[k] == nullptr)
//            continue;
//        else
//        {
//            Node* p = m_buckets[k];
//            while(p != nullptr)
//            {
//                cout << p->n_key << " is associated with " << p->n_value << endl;
//                p = p->n_next;
//            }
//        }
//    }
//
//    cout << "--- \n Here are the non-permanent members:\n";
//    DLL* ptr = m_head;
//    
//    //prints the DLL
//    while(ptr != nullptr)
//    {
//        cout << ptr->m_key << endl;
//        ptr = ptr->m_next;
//    }
//    cout << "---\n";
//    
//}
//




#endif
