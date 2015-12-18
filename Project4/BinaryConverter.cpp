#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    
    string binary = "", result = "";
    
    for(int k = 0; k < numbers.size(); k++)
        binary += convertNumberToBitString(numbers[k]);
    
    for(int k = 0; k < binary.length(); k++)
    {
        if(binary[k] == '0')
            result += " ";
        else if (binary[k] == '1')
            result += "\t";
    }
    
    
	return result;
}

#include <iostream>
bool BinaryConverter::decode(const string& bitString, vector<unsigned short>& numbers)
{
    if(bitString.size() % 16 != 0)
        return false;
    
    for(int k = 0; k < bitString.size(); k++)
        if(bitString[k] != ' ' && bitString[k] != '\t')
            return false;

    for(int j = 0; j < (bitString.size() / 16); j++)
    {
        string str;
        unsigned short num;
        for(int k = j*16; k < j*16 + 16; k++)
        {
            if(bitString[k] == ' ')
                str += "0";
            else if (bitString[k] == '\t')
                str += "1";
        }
        convertBitStringToNumber(str, num);
        numbers.push_back(num);
    }
	return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}




