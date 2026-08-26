/*
Write a program that receives a single string argument 
and prints its first whitespace-delimited token, followed by a newline.
*/

#include <unistd.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	char *s = argv[1];
	while (*s == 9 || (*s >= 11 && *s <= 13) || *s == 32)
		s++;
	while (*s && !(*s == 9 || (*s >= 11 && *s <= 13) || *s == 32))
	{
		write(1, s, 1);
		s++;
	}
	write(1, "\n", 1);
	return (0);
}