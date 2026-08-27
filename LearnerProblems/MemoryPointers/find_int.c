/*
Write a function that returns a pointer to the first int 
equal to value inside the half-open range [first, last), 
and last itself when the value never shows up.
*/
int	*find_int(int *first, int *last, int value)
{
	if (first == last)
		return (last);
	int i = 0;
	while (*(first + i) != *last)
	{
		if (*(first + i) == value)
			return (first + i);
		i++;
	}
	return (last);
}