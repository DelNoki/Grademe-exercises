/*
Implement a function that says whether a + b would leave the range of an int, 
and says it without ever computing a + b.
*/
#include <limits.h>
int	add_overflows(int a, int b)
{
	if ((a >= 0 && b > INT_MAX - a) || (a < 0 && b < INT_MIN - a))
		return (1);
	else 
		return (0);
}