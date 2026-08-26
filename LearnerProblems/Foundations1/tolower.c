int	tolower(int c)
{
	int nc = c;
	if (c >= 'A' && c <= 'Z')
		nc += 32;
	return (nc);
}