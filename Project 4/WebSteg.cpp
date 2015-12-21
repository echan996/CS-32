#include "provided.h"
#include <string>
#include "http.h"
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	string urlStore;
	if (!HTTP().get(url, urlStore))
		return false;  
	if (!Steg::hide(urlStore, msg, host))
		return false;
	return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	string urlStore;
	if (!HTTP().get(url, urlStore))
		return false;
	if (!Steg::reveal(urlStore, msg))
		return false;
	return true;  // This compiles, but may not be correct
}

unsigned int computeHash(string a){
	int count = 3;
	for (int i = 0; i < a.size(); i++)
		count *= (static_cast<int>(a[i]) + i);
	return count;
}
unsigned int computeHash(unsigned short a){
	return a*a / 2 * a / 3 * a % 4;
}