/*
Write a function that computes the least common multiple of two unsigned integers.
*/

unsigned int lcm(unsigned int a, unsigned int b)
{
	if (a == 0 || b == 0)
		return (0);
	unsigned int n = 1;
	while (n <= 4294967294 && !(n % a == 0 && n % b == 0))
		n++;
	return (n);
}

/*
Alternative : (avec pgcd = gcd)
unsigned int lcm(unsigned int a, unsigned int b)
{
	if (a == 0 || b == 0)
		return (0);
	if (a == b)
		return (a);
	unsigned int n = 2;
	unsigned int gcd = 2;
	while (n < a && n < b)
	{
		if (a % n == 0 && b % n == 0)
			gcd = n;
		n++;
	}
	if (a % gcd != 0 || b % gcd != 0)
		return (a * b);
	else 
		return ((a * b) / gcd);
}
*/