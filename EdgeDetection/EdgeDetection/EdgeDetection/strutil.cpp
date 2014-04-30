#include "strutil.h"

using namespace std;

string reverseString(string input)
{
	return string(input.rbegin(), input.rend());
}