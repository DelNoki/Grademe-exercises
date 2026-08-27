/*
Write a function that fills an output array with the sum 
of each row of a 2D grid whose rows are exactly four integers wide.
*/

#include <stddef.h>
void	row_sums(const int (*rows)[4], size_t nrows, int *out)
{
	if (nrows == 0)
		return ;
	size_t i = 0;
	while (i < nrows)
	{
		out[i] = rows[i][0] + rows[i][1] + rows[i][2] + rows[i][3];
		i++;
	}
}
