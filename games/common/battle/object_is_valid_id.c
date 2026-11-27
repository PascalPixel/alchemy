/* btObjectIsValidId, the same in both games. Both games'
   battle/battle_auto_battle.c include it where the function stands. */

/* Returns 0 for a party member's or an enemy's unit id, -1 for any other. */
Sint32 btObjectIsValidId(Uint32 unitId)
{
	if(unitId <= ALLY_MAX) {
		return 0;
	}
	if(unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
		return 0;
	}
	return -1;
}
