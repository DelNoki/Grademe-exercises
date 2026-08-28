/*
Implement a function that recurses from n down to 1, 
reporting every level to on_enter on the way down and to on_leave on the way back up.
*/

#include <stddef.h>
void	descend(int n, void (*on_enter)(int), void (*on_leave)(int))
{
	if (n <= 0)
		return ;
	int ok1 = (on_enter == NULL) ? 1 : 0;
	int ok2 = (on_leave == NULL) ? 1 : 0;
	int i = 0;
	while (i <= n - 1 && ok1 == 0)
	{
		on_enter(n - i);
		i++;
	}
	i = n;
	while (i > 0 && ok2 == 0)
	{
		on_leave(n - i + 1);
		i--;
	}
}

/*
Alternative : 
void	descend(int n, void (*on_enter)(int), void (*on_leave)(int))
{
	if (n <= 0)
		return ;
	if (on_enter)
		on_enter(n);
	descend(n - 1, on_enter, on_leave);
	if (on_leave)
		on_leave(n);
}
*/