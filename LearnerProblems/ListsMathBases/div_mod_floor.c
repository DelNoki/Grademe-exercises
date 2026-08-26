/*
Implement a function that divides a by b 
and writes the quotient and the remainder through q and r, 
choosing the pair whose remainder is never negative.
*/

void	div_mod_floor(int a, int b, int *q, int *r)
{
	if (a < 0 && b >= 0 && a % b != 0)
		*q = a / b - 1;
	else if (a < 0 && b < 0 && a % b != 0)
		*q = a / b + 1;
	else
		*q = a / b;
	*r = a - b * *q;
}

/*
Mon cauchemar :

long ft_abs(int x)
{
	long nb = x;
	if (nb < 0)
		return (-nb);
	else
		return (nb);
}

void	div_mod_floor(int a, int b, int *q, int *r)
{
	if (b == 0 || (a == -2147483648 && b == -1))
		return ;
	if (a == 0)
	{
		*q = 0;
		*r = 0;
		return;
	}
	if (b == 1)
	{
		*q = a;
		*r = 0;
		return;
	}
	if (ft_abs(a) < ft_abs(b))
	{
	    if (a < 0 && b < 0)
	        *q = 1;
		else if (a < 0 && b > 0)
			*q = -1;
		else
			*q = 0;
	}
	else if (ft_abs(a) > ft_abs(b))
	{
		if (a % b == 0)
		{
			*q = a / b;
			*r = 0;
		}
		else if (a < 0 && b < 0)
	        *q = a / b + 1;
		else if (a < 0 && b > 0)
			*q = a / b - 1;
		else
			*q = a / b;
	}
	else
	{
		if ((a > 0 && b < 0) || (a < 0 && b > 0))
	        *q = -1;
		else
			*q = 1;
	}
	*r = (int)ft_abs(ft_abs(a) - ft_abs(b) * ft_abs(*q));
}

*/