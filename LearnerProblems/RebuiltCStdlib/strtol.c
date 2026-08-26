/*
#include <ctype.h>
#include <errno.h>
#include <limits.h>
*/
#include <stddef.h>

long	strtol(const char *str, char **endptr, int base)
{
	const char		*start;
	long			result;
	int				sign;
	int				digit;
	int				converted;
	unsigned long	uresult;

	start = str;
	uresult = 0;
	sign = 1;
	converted = 0;

	while (((unsigned char)*str >= 9 && (unsigned char)*str <= 13) || (unsigned char)*str == 32)
		str++;

	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}

	if ((base == 0 || base == 16) && str[0] == '0'
		&& (str[1] == 'x' || str[1] == 'X'))
	{
		str += 2;
		base = 16;
	}
	else if (base == 0 && str[0] == '0')
	{
		base = 8;
	}
	else if (base == 0)
	{
		base = 10;
	}

	while (*str)
	{
		if ((unsigned char)*str >= '0' && (unsigned char)*str <= '9')
			digit = *str - '0';
		else if (((unsigned char)*str >= 'A' && (unsigned char)*str <= 'Z') || ((unsigned char)*str >= 'a' && (unsigned char)*str <= 'z'))
		{
			if ((unsigned char)*str >= 'A' && (unsigned char)*str <= 'Z')
				digit = (unsigned char)*str - 'A' + 10;
			else
				digit = (unsigned char)*str - 'a' + 10;
		}
		else
			break;

		if (digit >= base)
			break;

        /* Gestion de l'overflow
		if (uresult > (ULONG_MAX - digit) / base)
		{
			errno = ERANGE;
			if (endptr)
				*endptr = (char *)str;
			return (sign == 1 ? LONG_MAX : LONG_MIN);
		}
        */

		uresult = uresult * base + digit;
		str++;
		converted = 1;
	}

	if (!converted)
	{
		if (endptr)
			*endptr = (char *)start;
		return (0);
	}

	if (endptr)
		*endptr = (char *)str;

	result = (long)uresult * sign;
	return (result);
}


/* PREMIER ATTEMPT

#include <stddef.h>

long	strtol(const char *str, char **endptr, int base)
{
	int sign = 1;
	int i = 0;
	int j;
	long res = 0;
	char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if ((base == 0 || base == 16) && str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
	{
		i = i + 2;
		base = 16;
	}
	else if (base == 0 && str[i] == '0')
		base = 8;
	else if (base == 0)
		base = 10;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			j = 0;
			while (str[i] + 32 != digits[j])
				j++;
			if (j >= base)
    			break;
			res = res * (long)base + (long)j;
		}
		else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9'))
		{
			j = 0;
			while (str[i] != digits[j])
				j++;
			if (j >= base)
    			break;
			res = res * (long)base + (long)j;
		}
		else
			break;
		i++;
	}
	if (endptr)
		*endptr = (char *)(str + i);
	if (sign == -1)
		res = -res;
	return (res);
}
*/