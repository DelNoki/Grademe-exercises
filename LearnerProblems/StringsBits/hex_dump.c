/*
Implement a function that writes a memory region as a classic 
hexdump-style listing of 16 bytes per row, followed by an ASCII gutter.
*/

#include <unistd.h>

void	*hex_dump(void *addr, unsigned int size)
{
	unsigned char *p = addr;
	char *hexa = "0123456789abcdef";
	int count = 0;
	unsigned int i = 0;
	if (size == 0)
		return (addr);
	while (i < size)
	{
		count = 0;
		while (count < 16)
		{
			if (count + i < size)
			{
				write(1, &hexa[*(p + i + count) / 16], 1);
				write(1, &hexa[*(p + i + count) % 16], 1);
			}
			else
				write(1, "  ", 2);
			write(1, " ", 1);
			count++;
		}
		write(1, " ", 1);
		count = 0;
		while (count < 16 && i < size)
		{
			if (*(p + i) >= 32 && *(p + i) <= 126)
			{
				write(1, p + i, 1);
			}
			else
				write(1, ".", 1);
			i++;
			count++;
		}
		write(1, "\n", 1);
	}
	return (addr);
}

/*
N.B : un gars sur le site a fait des trucs bizarres, voici des notions inhabituelles.
- ptr[j] >> 4 : on prend la valeur située à l'index j du pointeur ptr, puis que l'on décale ses bits de 4 positions vers la droite
- ptr[j] & 0x0F : effectue une opération de masquage binaire pour extraire uniquement les 4 bits les plus à droite de l'élément situé à l'index j
*/