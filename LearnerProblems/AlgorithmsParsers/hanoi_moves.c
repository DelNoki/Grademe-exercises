/*
Write a program that prints the moves solving the Tower of Hanoi 
for the disk count given as its single argument, one move per line.
*/

#include <stdlib.h>
#include <unistd.h>

/* move count disks from X to Z through Y */
void move_disk(int count, char x, char y, char z)
{
	char buf[4];

	if (count == 0)
		return;
	move_disk(count - 1, x, z, y);
	buf[0] = x;
	buf[1] = ' ';
	buf[2] = z;
	buf[3] = '\n';
	write(1, buf, 4);
	move_disk(count - 1, y, x, z);
}

int	main(int argc, char **argv)
{
	int nb;

	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}

	nb = atoi(argv[1]);
	if (nb <= 0)
		return (0);
	move_disk(nb, 'A', 'B', 'C');
	return (0);
}
