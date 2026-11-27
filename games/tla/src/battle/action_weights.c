/* The enemies' action weights. */

#include "types.h"

/* btAIChooseAction's rolls for the first three action strategies, a row each:
   the chance in 256 of each place in the enemy's actions. */
Uint8 BattleAIActionWeights[3][8] = {
	{32, 32, 32, 32, 32, 32, 32, 32},
	{53, 47, 41, 35, 29, 23, 17, 11},
	{113, 56, 31, 20, 14, 10, 7, 5},
};
