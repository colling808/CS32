#ifndef COMPRESSOR_INCLUDED
#define COMPRESSOR_INCLUDED

#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>

template<typename KeyType, typename ValueType>
class HashTable;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    
    unsigned short C = static_cast<unsigned short>(s.length()/2) + 512;
    if(C > 16384)
        C = 16384;
    
    HashTable<string , unsigned short> H(C*2, C);
    
    for(int j = 0; j < 256; j++)
    {
        string str(1, static_cast<char>(j));
        H.set(str, j, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    vector<unsigned short> V;
    
    for(int k = 0; k < s.size(); k++)
    {
        string c (1,s[k]), expandedRun = runSoFar + c;
        unsigned short x, cv;
        if(H.get(expandedRun, x))
        {
            runSoFar = expandedRun;
            continue;
        }
        
        H.get(runSoFar, x);
        V.push_back(x);
        H.touch(runSoFar);
        runSoFar = "";
        H.get(c, cv);
        V.push_back(cv);
        if(!H.isFull())
        {
            H.set(expandedRun, nextFreeID);
            nextFreeID++;
        }
        else
        {
            string str;
            H.discard(str, x);
            H.set(expandedRun, x);
        }
    }
    if(runSoFar.length() != 0)
    {
        unsigned short x;
        H.get(runSoFar, x);
        V.push_back(x);
    }
    
    V.push_back(C);
    numbers = V;
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    if(numbers.empty())
        return false;
    
    int C = numbers[numbers.size()-1];
    
    HashTable<unsigned short, string> H(C*2,C);
    
    for(int j = 0; j < 256; j++)
    {
        string str(1, static_cast<char>(j));
        H.set(j, str, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "", output = "";
    
    for(int k = 0; k < numbers.size() -1; k++)
    {
        unsigned short us = numbers[k];
        string str;
        if(us <= 255)
        {
            H.get(us, str);
            output += str;
            if(runSoFar == "")
            {
                runSoFar = string(1,static_cast<char>(us));
                continue;
            }
            
            string expandedRun = runSoFar + string(1,static_cast<char>(us));
            if(!H.isFull())
            {
                H.set(nextFreeID, expandedRun);
                nextFreeID++;
            }
            else
            {
                unsigned short x;
                str = "";
                H.discard(x, str);
                H.set(x, expandedRun);
            }
            
            runSoFar = "";
            continue;
        }
        else
        {
            if(!H.get(us, str))
                return false;
            H.touch(us);
            H.get(us, str);
            output += str;
            runSoFar = str;
        
        }
    }
    
    s = output;
    return true;

}

unsigned int computeHash(string key)
{
    unsigned int hash = 0;
    for(int k = 0; k < key.size(); k++)
        hash = hash + key[k];
    
    return hash;
}


unsigned int computeHash(unsigned short key)
{
    return key;
}



#endif