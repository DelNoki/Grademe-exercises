char *find_any_char(const char *text, const char *needles)
{
	if (!needles || !text)
		return ((void *)0);
	int i = 0;
	int j;
	char *p = (char *)text;
	while (text[i])
	{
		j = 0;
		while (needles[j])
		{
			if (needles[j] == text[i])
				return (p + i);
			j++;
		}
		i++;
	}
	return ((void *)0);
}