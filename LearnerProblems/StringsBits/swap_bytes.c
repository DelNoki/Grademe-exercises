unsigned char swap_bits(unsigned char byte)
{
	int i = 0;
	unsigned char swap;
	unsigned char tab[8] = {0};
	unsigned char b = byte;
	while (i < 8)
	{
		tab[7 - i] = b % 2;
		b = (b - (b % 2)) / 2;
		i++;
	}
	i = 0;
	while (i < 4)
	{
		swap = tab[i];
		tab[i] = tab[4 + i];
		tab[4 + i] = swap;
		i++;
	}
	b = 0;
	i = 0;
	while (i < 8)
	{
		b = b * 2 + tab[i];
		i++;
	}
	return (b);
}

/*
#include <stdio.h>
int main(void)
{
	printf("%u\n", swap_bits(0xF0));
	printf("%u\n", swap_bits(0xAB));
	return 0;
}
*/