/*
Write a two-function allocator over a 4096-byte static pool: 
heap_alloc hands out 8-aligned blocks from a free list, 
heap_free gives one back and merges it with a free neighbour.
*/

#include <stddef.h>

#define POOL_SIZE 4096

typedef struct s_tag
{
	size_t	size;   /* taille utile du bloc, toujours multiple de 8 */
	size_t	free;   /* 1 si libre, 0 si occupé */
}	t_tag;

#define TAG_SIZE sizeof(t_tag)

static _Alignas(8) unsigned char	pool[POOL_SIZE];
static int							initialized = 0;

/*
Un header au tout début du pool (pool[0]), qui déclare une taille utile 
de POOL_SIZE - 2 * TAG_SIZE = 4096 - 32 = 4064 octets. 
On soustrait deux fois TAG_SIZE parce qu'il faut réserver la place 
pour le header et le footer à l'intérieur des 4096 octets physiques.

Un footer, à l'adresse pool + TAG_SIZE + h->size, 
c'est-à-dire juste après les 4064 octets de payload. 
Il porte exactement les mêmes valeurs que le header.
*/
static void	init_pool(void)
{
	t_tag	*h;
	t_tag	*f;

	h = (t_tag *)pool;
	h->size = POOL_SIZE - 2 * TAG_SIZE;
	h->free = 1;
	f = (t_tag *)(pool + TAG_SIZE + h->size);
	f->size = h->size;
	f->free = 1;
	initialized = 1;
}

/* Retrouver le footer d'un bloc à partir de son header:
Le cast en unsigned char * sert à faire de l'arithmétique de pointeur en octets 
— si on avait fait h + TAG_SIZE directement sur un t_tag *, 
le compilateur aurait avancé de TAG_SIZE * sizeof(t_tag) octets, 
ce qui n'est pas du tout ce qu'on veut.
*/
static t_tag	*get_footer(t_tag *h)
{
	return ((t_tag *)((unsigned char *)h + TAG_SIZE + h->size));
}

/*
Le bloc suivant commence juste après le footer du bloc courant. 
On calcule cette adresse, puis on vérifie qu'elle est encore strictement 
à l'intérieur du pool (n < pool + POOL_SIZE). 
Si on est arrivé exactement à la fin du pool, ça veut dire que le bloc courant 
était le dernier — on renvoie NULL pour signaler qu'il n'y a rien après.

C'est cette fonction qui sert de « curseur d'avancement » 
dans la boucle de recherche de heap_alloc.
*/
static t_tag	*get_next(t_tag *h)
{
	unsigned char	*n;

	n = (unsigned char *)get_footer(h) + TAG_SIZE;
	if (n >= pool + POOL_SIZE)
		return (NULL);
	return ((t_tag *)n);
}

/* C'est le symmétrique de get_footer en sens inverse:
C'est ici que la magie des boundary tags opère vraiment. 
Pour trouver le bloc précédent, il faut d'abord trouver son footer, 
qui se trouve juste avant le header courant (h - TAG_SIZE). 
On vérifie qu'on ne sort pas du pool par la gauche (pf_addr < pool).

Une fois qu'on a pf (previous footer), on lit pf->size 
— c'est-à-dire la taille du bloc précédent. Qu'il ait été libre ou occupé, 
cette info a toujours été maintenue à jour. 
On peut alors reculer de TAG_SIZE + pf->size octets supplémentaires 
pour atterrir exactement sur le header du bloc précédent.
*/
static t_tag	*get_prev(t_tag *h)
{
	unsigned char	*pf_addr;
	t_tag			*pf;

	pf_addr = (unsigned char *)h - TAG_SIZE;
	if (pf_addr < pool)
		return (NULL);
	pf = (t_tag *)pf_addr;
	return ((t_tag *)(pf_addr - TAG_SIZE - pf->size));
}


/* Centre du programme:
- Initialisation paresseuse au premier appel.
- Garde-fou en deux temps : n == 0 renvoie NULL directement. 
n > POOL_SIZE est testé avant tout calcul, ce qui protège contre l'overflow: 
si n = SIZE_MAX, on sort ici, avant même de calculer rounded, 
qui aurait sinon débordé silencieusement vers une petite valeur.
- rounded : arrondi de n au multiple de 8 supérieur.
- On parcourt la liste chaînée implicite des blocs, en partant du tout début du pool, 
via get_next, jusqu'à tomber sur NULL (fin du pool) ou trouver un bloc libre assez grand.
- Notez qu'il n'y a pas de structure de liste chaînée explicite 
(pas de pointeurs next/prev stockés) : la « liste » est entièrement portée par 
la disposition physique des blocs dans le tableau, et on la traverse avec get_next. 
Autrement dit: « la forme de la free list est libre » — ici, c'est une liste implicite dérivée des tailles.
- Cas A (découper le bloc):
Si le bloc trouvé est beaucoup plus grand que ce dont on a besoin, 
ça ne sert à rien de le donner en entier — on le coupe en deux : 
la première partie devient le bloc alloué, la deuxième reste libre pour de futures requêtes.
Condition du split : on ne le fait que si remaining >= 2 * TAG_SIZE, 
c'est-à-dire s'il reste au moins la place de loger un header et un footer 
pour le morceau restant.
- Cas B (le bloc est donné en entier):
Si le surplus est trop petit pour former un bloc viable, 
on donne tout le bloc tel quel : on marque juste free = 0 
sur le header et le footer existants, sans toucher à size.
Dans les deux cas, on renvoie l'adresse juste après le header, 
c'est-à-dire le début du payload utilisable — c'est ce pointeur-là 
que l'utilisateur va manipuler (et plus tard repasser à heap_free).
- Si le bloc courant ne convenait pas (occupé, ou trop petit), 
on passe au suivant. Si on épuise toute la liste sans rien trouver, on renvoie NULL.
*/
void	*heap_alloc(size_t n)
{
	t_tag	*h;
	t_tag	*f;
	size_t	rounded;
	size_t	remaining;
	t_tag	*newh; /* fait office de swap pour écrire les footers/headers */

	if (!initialized)
		init_pool();
	if (n == 0 || n > POOL_SIZE)
		return (NULL);
	rounded = (n + 7) & ~(size_t)7;
	h = (t_tag *)pool;
	while (h)
	{
		if (h->free && h->size >= rounded)
		{
			remaining = h->size - rounded;
			if (remaining >= 2 * TAG_SIZE)
			{
				h->size = rounded;
				h->free = 0;
				f = get_footer(h);
				f->size = rounded;
				f->free = 0;
				newh = (t_tag *)((unsigned char *)f + TAG_SIZE);
				newh->size = remaining - 2 * TAG_SIZE;
				newh->free = 1;
				f = get_footer(newh);
				f->size = newh->size;
				f->free = 1;
			}
			else
			{
				h->free = 0;
				f = get_footer(h);
				f->free = 0;
			}
			return ((void *)((unsigned char *)h + TAG_SIZE));
		}
		h = get_next(h);
	}
	return (NULL);
}

/*
- Initialisation paresseuse par sécurité (au cas où heap_free serait appelée avant 
tout heap_alloc). Puis le cas heap_free(NULL).
- À partir du pointeur utilisateur p, on retrouve le header en reculant de TAG_SIZE octets 
(symétrique de ce qu'on faisait à la fin de heap_alloc). On marque le bloc comme libre, header et footer.
- (Fusion avec le voisin suivant)
On regarde ce qu'il y a juste après notre bloc. S'il existe (next != NULL) et qu'il est libre, 
on fusionne : la nouvelle taille du bloc h devient sa taille actuelle, plus 2 * TAG_SIZE 
(le header et le footer de next qui disparaissent, absorbés dans le payload), plus la taille de next. 
On recalcule ensuite le footer à sa nouvelle position (via get_footer(h), qui utilise la nouvelle h->size) 
— ce nouveau footer est en réalité l'ancien footer de next, mais peu importe : on écrase son contenu avec les bonnes valeurs.
- (Fusion avec le voisin précédent)
Même logique, mais vers l'arrière. On cherche le bloc précédent. S'il existe et est libre, 
on absorbe notre bloc h (qui, à ce stade, a peut-être déjà été fusionné avec next !) dans prev. 
Le nouveau bloc résultant a pour header celui de prev, et pour footer celui qu'on vient de recalculer.
*/
void	heap_free(void *p)
{
	t_tag	*h;
	t_tag	*f;
	t_tag	*next;
	t_tag	*prev;

	if (!initialized)
		init_pool();
	if (!p)
		return ;
	h = (t_tag *)((unsigned char *)p - TAG_SIZE);
	h->free = 1;
	f = get_footer(h);
	f->free = 1;
	next = get_next(h);
	if (next && next->free)
	{
		h->size = h->size + 2 * TAG_SIZE + next->size;
		f = get_footer(h);
		f->size = h->size;
		f->free = 1;
	}
	prev = get_prev(h);
	if (prev && prev->free)
	{
		prev->size = prev->size + 2 * TAG_SIZE + h->size;
		f = get_footer(prev);
		f->size = prev->size;
		f->free = 1;
	}
}