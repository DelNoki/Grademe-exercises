/*
Write a program that reads two integers and an operator 
from the command line, then prints the computed result.
*/

#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("wrong number of arguments\n");
		return (0);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[3]);
	if (argv[2][0] == '/' && b != 0)
		printf("%d", a / b);
	else if (argv[2][0] == '+')
		printf("%d", a + b);
	else if (argv[2][0] == '-')
		printf("%d", a - b);
	else if (argv[2][0] == '*')
		printf("%d", a * b);
	else if (argv[2][0] == '%')
		printf("%d", a % b);
	printf("\n");
	return (0);
}
