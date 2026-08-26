#include <unistd.h>

int ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

void ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		char *msg = "wrong number of arguments\n";
		ft_putstr(msg);
		return (0);
	}
	int i = 0;
	int j = 0;
	while (argv[2][i])
	{
		while (argv[2][i + j] == argv[1][j] && argv[1][j])
			j++;
		if (argv[1][j] == 0)
		{
			char *msg = "yes\n";
			ft_putstr(msg);
			return (0);
		}
		i++;
	}
	char *msg = "no\n";
	ft_putstr(msg);
	return (0);
}