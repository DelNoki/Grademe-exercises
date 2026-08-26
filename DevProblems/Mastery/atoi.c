int atoi(const char *str)
{
	int i = 0;
	int minus = 0;
	int	tot = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus++;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		tot = tot * 10 + str[i] - '0';
		i++;
	}
	if (minus % 2 != 0)
		tot *= -1;
	return (tot);
}
