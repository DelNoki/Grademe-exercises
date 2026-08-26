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

static int	is_open(char c)
{
	return (c == '(' || c == '[' || c == '{');
}

static int	is_close(char c)
{
	return (c == ')' || c == ']' || c == '}');
}

static int	matches(char open, char close)
{
	if (open == '(' && close == ')')
		return (1);
	if (open == '[' && close == ']')
		return (1);
	if (open == '{' && close == '}')
		return (1);
	return (0);
}

static int	is_balanced(const char *s)
{
	char	stack[1024];
	int		top;
	int		i;

	top = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (is_open(s[i]))
		{
			if (top >= 1024)
				return (0);
			stack[top++] = s[i];
		}
		else if (is_close(s[i]))
		{
			if (top == 0 || !matches(stack[top - 1], s[i]))
				return (0);
			top--;
		}
		i++;
	}
	return (top == 0);
}

int	main(int argc, char **argv)
{
	int	i;

	if (argc < 2)
	{
		ft_putstr(1, "wrong number of arguments\n");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (is_balanced(argv[i]))
			ft_putstr(1, "OK\n");
		else
			ft_putstr(1, "Error\n");
		i++;
	}
	return (0);
}