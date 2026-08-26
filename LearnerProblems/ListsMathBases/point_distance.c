typedef struct point
{
	int	x;
	int	y;
} point;

long	point_distance2(point a, point b)
{
	long d = ((long)a.x - (long)b.x) * ((long)a.x - (long)b.x);
	d += ((long)a.y - (long)b.y) * ((long)a.y - (long)b.y);
	return (d);
}