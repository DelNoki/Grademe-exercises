#include <stddef.h>
void	reverse_int(int *a, size_t n)
{
	if (n == 0)
		return;
	int swap;
	size_t i = 0;
	while (i < (n / 2))
	{
		swap = *(a + i);
		*(a + i) = *(a + n - 1 - i);
		*(a + n - 1 - i) = swap;
		i++;
	}
}