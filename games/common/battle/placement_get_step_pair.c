/* btPlacementGetStepPair, the same in both games. Both games'
   battle/battle_placement.c include it where the function stands. */

/* Unused: reads the battlefield position of a party place. */
void btPlacementGetStepPair(Sint32 partyPlace, Sint32 *x, Sint32 *y)
{
	partyPlace *= 2;
	*x = BattlePlacementSpots[partyPlace];
	*y = BattlePlacementSpots[partyPlace + 1];
}
