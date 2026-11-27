/* btSprGetDefeatSnd, the same in both games. The Broken Seal's
   battle/battle_sprite.c and The Lost Age's battle/battle_formation.c
   include it where the function stands. */

/* Returns a battle sprite's defeat-sound kind, 0 to 4, or -1 for none; a fallen
   enemy's sound is SE_ENEMY_CRY plus it. Reads any index unchecked. */
Uint32 btSprGetDefeatSnd(Sint32 idx)
{
	Uint32 kind = BattleSprites[idx].defeatSnd;

	if((Sint32)kind > 4)
		kind = -1U;
	return kind;
}
