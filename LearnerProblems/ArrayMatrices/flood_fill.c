/*
Implement a flood-fill algorithm that propagates through a 2D character grid 
from a given starting cell, marking every reachable connected cell with the character F.
*/

typedef struct Point
{
	int x;
	int y;
} Point;

int shall_put(char **tab, Point size, unsigned char c, int x, int y)
{
	if (x < 0 || x >= size.x || y < 0 || y >= size.y)
		return (0);
	return (c == (unsigned char)(tab[y][x]));
}

void	flood_fill(char **tab, Point size, Point begin)
{
	unsigned char c;

	if (!tab || begin.x < 0 || begin.x >= size.x
		|| begin.y < 0 || begin.y >= size.y)
		return;

	c = (unsigned char)(tab[begin.y][begin.x]);
	tab[begin.y][begin.x] = 'F';

	if (shall_put(tab, size, c, begin.x - 1, begin.y))
	{
		begin.x -= 1;
		flood_fill(tab, size, begin);
		begin.x += 1;
	}
	if (shall_put(tab, size, c, begin.x + 1, begin.y))
	{
		begin.x += 1;
		flood_fill(tab, size, begin);
		begin.x -= 1;
	}
	if (shall_put(tab, size, c, begin.x, begin.y - 1))
	{
		begin.y -= 1;
		flood_fill(tab, size, begin);
		begin.y += 1;
	}
	if (shall_put(tab, size, c, begin.x, begin.y + 1))
	{
		begin.y += 1;
		flood_fill(tab, size, begin);
	}
}