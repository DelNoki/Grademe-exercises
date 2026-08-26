#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("wrong number of arguments\n");
		return (0);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int n = 2;
	if (a == 0 || b == 0)
		return (0);
	if (a == b)
		return (a);
	int gcd = 1;
	while (n < a && n < b)
	{
		if (a % n == 0 && b % n == 0)
			gcd = n;
		n++;
	}
	printf("%d\n", gcd);
	return (0);
}