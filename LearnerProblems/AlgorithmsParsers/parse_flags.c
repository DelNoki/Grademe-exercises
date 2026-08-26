#include <unistd.h>

static int	ft_strlen(const char *s)
{
	int	l = 0;

	while (s[l] != '\0')
		l++;
	return (l);
}

static void	ft_putstr(int fd, const char *s)
{
	write(fd, s, ft_strlen(s));
}

static int	ft_streq(const char *a, const char *b)
{
	int	i = 0;

	while (a[i] != '\0' && b[i] != '\0')
	{
		if (a[i] != b[i])
			return (0);
		i++;
	}
	return (a[i] == '\0' && b[i] == '\0');
}

int	main(int argc, char **argv)
{
	int	i = 1;

	while (i < argc)
	{
		if (argv[i][0] != '-' || ft_streq(argv[i], "--"))
			break ;
		if (ft_streq(argv[i], "-l"))
			ft_putstr(1, "Option l\n");
		else if (ft_streq(argv[i], "-c"))
			ft_putstr(1, "Option c\n");
		else if (ft_streq(argv[i], "-p"))
		{
			if (i + 1 < argc)
			{
				ft_putstr(1, "Option p with arg ");
				ft_putstr(1, argv[i + 1]);
				ft_putstr(1, "\n");
				i++;
			}
		}
		i++;
	}
	return (0);
}