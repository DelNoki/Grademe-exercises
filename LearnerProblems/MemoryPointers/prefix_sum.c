/* Buffer out remembers all the partial sums of {*a, *(a + 1), ..., *(a + n - 1)} */
#include <stddef.h>
void	prefix_sum(const int *a, size_t n, long *out)
{
	if (n == 0)
		return ;
	size_t i = 1;
	*out = *a;
	while (i < n)
	{
		*(out + i) = *(out + i - 1) + *(a + i);
		i++;
	}
}