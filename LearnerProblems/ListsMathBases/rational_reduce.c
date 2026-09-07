/*
Reduce the fraction held in r in place by dividing num and den by their 
gcd, with the sign carried by num so den ends up strictly positive.
*/

typedef struct rational
{
	int	num;
	int	den;
} rational;

int	ft_gcd(int a, int b)
{
	int gcd = 1;
	int n = 2;
	if (a == 0 || b == 0)
		return (0);
	if (a == b)
		return (a);
	while (n <= a && n <= b)
	{
		if (a % n == 0 && b % n == 0)
			gcd = n;
		n++;
	}
	return (gcd);
}

void	rational_reduce(rational *r)
{
	int gcd;
	int sign;
	int a;
	int b;

	if (r->den == 0)
		return;
	sign = ((r->num > 0 && r->den < 0) || (r->num < 0 && r->den > 0)) ? -1 : 1;
	a = (r->num < 0) ? -(r->num) : r->num;
	b = (r->den < 0) ? -(r->den) : r->den;
	gcd = ft_gcd(a, b);
	r->num = (gcd == 0) ? 0 : sign * a / gcd;
	r->den = (gcd == 0) ? 1 : b / gcd;
}
