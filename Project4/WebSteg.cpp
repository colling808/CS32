#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    string s;
    if(!HTTP().get(url, s))
        return false;
    Steg::hide(s, msg, host);

    return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string s;
    if(!HTTP().get(url, s))
        return false;
    
    if(!Steg::reveal(s, msg))
        return false;
    
	return true;
}
