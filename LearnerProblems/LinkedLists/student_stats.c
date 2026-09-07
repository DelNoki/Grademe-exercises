/*
Implement student_stats, which reduces an array of student records into one stats struct 
holding the lowest score, the highest score, the integer mean, and the name of the top scorer.
*/

#include <stddef.h>

typedef struct s_student
{
	const char	*name;
	int		score;
} student;

typedef struct s_stats
{
	int		min;
	int		max;
	int		mean;
	const char	*best;
} stats;

void	student_stats(const student *a, size_t n, stats *out)
{
	size_t i;
	long long m = 0;
	
	i = 0;
	if (n == 0)
	{
		out->min = 0;
		out->max = 0;
		out->mean = 0;
		out->best = NULL;
		return;
	}
	else
	{
		out->min = (a[i]).score;
		out->max = (a[i]).score;
		m = 0;
		out->best = (a[i]).name;
	}
	while (i < n)
	{
		m += (a[i]).score;
		if ((a[i]).score < out->min)
			out->min = (a[i]).score;
		else if ((a[i]).score > out->max)
		{
			out->max = (a[i]).score;
			out->best = (a[i]).name;
		}
		i++;
	}
	out->mean = (m < 0) ? (int)(-(-m / i)) : (int)(m / i);
}
