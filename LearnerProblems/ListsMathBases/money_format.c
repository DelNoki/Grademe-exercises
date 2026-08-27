int nb_digits(unsigned long n)
{
    unsigned long nb = n;
    int i = 0;
    while (nb > 0)
    {
        nb /= 10;
        i++;
    }
    return (i);
}

int	money_format(long cents, char *out)
{
	int l = 0;
	int nd;
	unsigned long nb;
	int sign = 0;

	/* Gérer le signe */
	if (cents < 0)
	{
		*(out + l) = '-';
		l++;
		sign = 1;
		nb = -cents;
	}
	else
		nb = cents;

	/* Gérer les centaines */
	if (nb <= 99)
	{
		l = 0;
		*(out + l + sign) = '0';
		l++;
		*(out + l + sign) = '.';
		l++;
		*(out + l + sign) = '0' + nb / 10;
		l++;
		*(out + l + sign) = '0' + nb % 10;
		l++;
	}
	else
	{
		l = 0;
	    nd = nb_digits(nb);
	    *(out + nd + sign - l) = '0' + nb % 10;
	    l++;
		nb /= 10;
	    *(out + nd + sign - l) = '0' + nb % 10;
	    l++;
		nb /= 10;
	    *(out + nd + sign - l) = '.';
	    l++;
		while (nb > 0)
		{
			*(out + nd + sign - l) = '0' + nb % 10;
			nb = nb / 10;
			l++;
		}
	}
	*(out + sign + l) = 0;
	return (l + sign);
}

/*
#include <stdio.h>
int main(void)
{
    long cents = -1;
	char out[26];
	printf("Valeur fct : %d, et chaîne obtenue : %s\n", money_format(cents, out), out);
}

Alternative : 
static void ft_add_nbr(unsigned long nbr, char *out, int *index, int round)
{
	if (round == 0 && nbr < 10)
		out[(*index)++] = '0';
	if (nbr >= 10)
		ft_add_nbr(nbr / 10, out, index, 1);
	out[(*index)++] = "0123456789"[nbr % 10];
}

int	money_format(long cents, char *out)
{
	int index = 0;
	unsigned long cent;

	if (cents < 0)
	{
		out[index++] = '-';
		cent = -(unsigned long)cents;
	}
	else
		cent = (unsigned long)cents;

	unsigned long n1 = cent / 100;
	unsigned long n2 = cent % 100;

	ft_add_nbr(n1, out, &index, 1);
	out[index++] = '.';
	ft_add_nbr(n2, out, &index, 0);
	out[index] = '\0';
	return (index);
}
*/