/*
Implement four bit flag macros in flags.h, then use them in update_flag.
En gros beaucoup de booléens...
*/

#ifndef FLAGS_H
# define FLAGS_H

/*
FLAG(bit) construit un masque n'ayant que le bit voulu à 1 (ex : bit 3 → 00001000).
*/
# define FLAG(bit) (1u << (bit))

/*
(value) & FLAG(bit) fait un ET bit à bit entre value et ce masque. 
Résultat :
- si le bit était à 1 dans value → le résultat vaut FLAG(bit) (donc non nul)
- si le bit était à 0 dans value → le résultat vaut 0

!= 0 transforme ça en un booléen propre (1 ou 0) plutôt que de renvoyer une valeur arbitraire (genre 8)

Exemple : value = 0b1010, bit = 1 → FLAG(1) = 0b0010 → 0b1010 & 0b0010 = 0b0010 (non nul) → HAS_FLAG renvoie 1.
*/
# define HAS_FLAG(value, bit) (((value) & FLAG(bit)) != 0)

/*
((value) | FLAG(bit)) fait un OU bit à bit avec le masque et force le bit voulu à 1, sans toucher aux autres bits (car OU avec 0 ne change rien).

Exemple : value = 0b1010, bit = 0 → FLAG(0) = 0b0001 → 0b1010 | 0b0001 = 0b1011.
*/
# define SET_FLAG(value, bit) ((value) | FLAG(bit))

/*
~FLAG(bit) inverse tous les bits du masque : le bit ciblé devient 0 et tous les autres deviennent 1 (ex : bit 1 → FLAG(1) = 0b0010 → ~FLAG(1) = ...11111101).

Faire un ET avec ce masque inversé force le bit voulu à 0 tout en préservant les autres bits (car ET avec 1 ne change rien).

Exemple : value = 0b1011, bit = 1 → ~FLAG(1) = ...11111101 → 0b1011 & ...1101 = 0b1001.
*/
# define CLEAR_FLAG(value, bit) ((value) & ~FLAG(bit))

unsigned int	update_flag(unsigned int value, unsigned int bit, int enabled);

#endif

//#include "flags.h"

unsigned int	update_flag(unsigned int value, unsigned int bit, int enabled)
{
	if (enabled)
		return(SET_FLAG(value, bit));
	return(CLEAR_FLAG(value, bit));
}