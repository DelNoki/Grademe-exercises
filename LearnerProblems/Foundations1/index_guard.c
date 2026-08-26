/* Vérifier que l'index i est valide pour la liste a de longueur n */
#include <stddef.h>
int	at_or_default(const int *a, size_t n, long i, int fallback)
{
	if (n == 0 || !a)
		return (fallback);
	int	value;
	if (i >= 0 && (size_t)i < n)
	{
		value = a[i];
		return (value);
	}
	else
		return (fallback);
}