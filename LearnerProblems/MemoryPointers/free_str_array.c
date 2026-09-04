/*
Write free_str_array, which releases every string of a NULL-terminated char **, 
then releases the array of pointers itself, and returns how many strings it held.
*/

#include <stdlib.h>
#include <stddef.h>

size_t	free_str_array(char **arr)
{
	size_t i = 0;
	if (!arr)
		return (0);
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (i);
}
