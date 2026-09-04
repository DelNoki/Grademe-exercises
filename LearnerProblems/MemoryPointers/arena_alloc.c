/*
Write arena_alloc and arena_reset, a bump allocator that serves 8-aligned 
slices out of one 4096-byte static pool and never calls malloc.
*/

#include <stddef.h>

static _Alignas(8) unsigned char	pool[4096];
static size_t						offset = 0;

void	*arena_alloc(size_t n)
{
	size_t	rounded;
	void	*p;

	if (n == 0 || n > sizeof(pool))
		return (NULL);
	rounded = (n + 7) & ~(size_t)7;
	if (rounded > sizeof(pool) - offset)
		return (NULL);
	p = &pool[offset];
	offset += rounded;
	return (p);
}

void	arena_reset(void)
{
	offset = 0;
}

/*
Explications concernant le (n + 7) & ~(size_t)7 :

~(size_t)7 inverse les bits de 7 vu comme un size_t en binaire, i.e:
 7 = 0000...0111
~7 = 1111...1000
Ce masque ~7 a donc tous les bits à 1, sauf les 3 derniers bits (qui valent 000). 
C'est un masque qui, appliqué avec un &, va effacer les 3 derniers bits de n'importe quel nombre
c'est-à-dire le forcer à être un multiple de 8 
(puisqu'un multiple de 8 s'écrit toujours avec ses 3 derniers bits à 0).

n + 7:
Mais si on applique directement n & ~7, on arrondit vers le bas (on tronque),
pas vers le haut. Par exemple 9 & ~7 donnerait 8, alors qu'on veut 16.
L'astuce classique pour arrondir vers le haut consiste à d'abord ajouter 7 (c'est-à-dire 8 - 1) 
avant de tronquer. Ça a pour effet de "pousser" n dans la tranche 
supérieure seulement s'il n'était pas déjà un multiple de 8.
*/