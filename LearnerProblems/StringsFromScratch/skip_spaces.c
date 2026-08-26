const char	*skip_spaces(const char *s)
{
	if (!*s)
		return (s);
	int i = 0;
	while ((s[i] == 9 || (s[i] >= 11 && s[i] <= 13) || s[i] == 32) && s[i])
		i++;
	return (s + i);
}