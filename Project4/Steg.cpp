#include "provided.h"
#include <vector>
#include <string>
using namespace std;


void removeSpace(string& s)
{
    unsigned long k = s.size() - 1;
    if(s[k] == '\r')
    {
        s.pop_back();
        k--;
    }
    while(k >= 0)
    {
        if(s[k] != ' ' && s[k] != '\t')
           break;
        else
            s.pop_back();
        k--;
    }
}

void removeLead(string& s)
{
    string str = "";
    unsigned long k = 0;
    if(s[s.size()-1] == '\r')
        s.pop_back();
    
    while(k < s.size())
    {
        if(s[k] == ' ' || s[k] == '\t')
            str += s[k];
        else
            str = "";
        k++;
    }
    s = str;
}




bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
    if(hostIn.empty())
        return false;
    
    vector<string> substrings;
    
    string substr = "";
    int k = 0;
    while(k < hostIn.size())
    {
        if(hostIn[k] != '\n')
           substr += hostIn[k];
        else if(hostIn[k] == '\n')
        {
            substrings.push_back(substr);
            substr = "";
        }
        if(k == hostIn.size() - 1 && hostIn[k] != '\n')
            substrings.push_back(substr);
        k++;
    }
    
    for(int j = 0; j < substrings.size(); j++)
        removeSpace(substrings[j]);
    
    vector<unsigned short> numbers;
    Compressor::compress(msg, numbers);
    string code = BinaryConverter::encode(numbers);
    
    int L = code.length(), N = substrings.size();
    vector <string> subcode;
    
    for(int j = 0; j < N ; j++)
    {
        if(j < (N % L))
        {
            substrings[j] += code.substr(j*(L/N+1), L/N+1);
        }
        else
        {
            substrings[j] += code.substr(j*(L/N), L/N);
        }
        
        hostOut = hostOut + substrings[j] + '\n';
    }
    return true;
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
    vector<string> substrings;
    
    string str = "";
    int k = 0;
    while(k < hostIn.size())
    {
        if(hostIn[k] != '\n')
            str += hostIn[k];
        else
        {
            substrings.push_back(str);
            str = "";
        }
        if(k == hostIn.size() - 1 && hostIn[k] != '\n')
            substrings.push_back(str);

        k++;
    }
    
    str = "";
    for(int k = 0; k < substrings.size(); k++)
    {
        removeLead(substrings[k]);
        str += substrings[k];
    }
    
    vector<unsigned short> numbers;
    if(!BinaryConverter::decode(str, numbers))
       return false;
    
    if(!Compressor::decompress(numbers, str))
        return false;
    
    msg = str;
    
	return true;  // This compiles, but may not be correct
}



















