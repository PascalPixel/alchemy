/* Reading a unit's row of BattleSprites by its id (UNIT.id): an enemy's sprite,
   palette, size, attack and defeat sound. Part of battle.c. */

#include "battle_internal.h"

/* Returns the enemy's defeat-sound kind, 0 to 4, or -1 for none; a fallen
   enemy's sound is SE_ENEMY_CRY plus it. Reads any index unchecked. */
Uint32 btSprGetDefeatSnd(Sint32 idx)
{
	Uint32 kind = BattleSprites[idx].defeatSnd;

	if((Sint32)kind > 4)
		kind = -1U;
	return kind;
}

/* The last row of BattleSprites (battle_sprites.c). */
#define BATTLE_SPRITE_ROW_LAST 171

/* Returns the enemy's battle sprite, row 0's for an index out of range. */
Sint32 btSprGetSpr(Sint32 idx)
{
	if(idx < 0 || idx > BATTLE_SPRITE_ROW_LAST)
		return BattleSprites[0].anm;
	return BattleSprites[idx].anm;
}

/* XXX (no evidence): whether returning the sprite for a bad index is a slip
   copied from btSprGetSpr. Returns the enemy's sprite palette. For an index out
   of range it returns row 0's sprite, not a palette. */
Sint32 btSprGetPalette(Sint32 idx)
{
	if(idx < 0 || idx > BATTLE_SPRITE_ROW_LAST)
		return BattleSprites[0].anm;
	return BattleSprites[idx].pal;
}

/* Returns 1 for a small enemy, which takes one enemy sprite slot instead of two
   and stands closer to its neighbours; 0 out of range. */
Sint32 btSprIsSmall(Sint32 idx)
{
	Bool result;

	if(idx < 0 || idx > BATTLE_SPRITE_ROW_LAST)
		return 0;
	result = FALSE;
	if(BattleSprites[idx].isSmall)
		result = TRUE;
	return result;
}

/* Returns the beam an enemy's plain attack shows, 1 for none or out of
   range. */
Uint32 btSprGetAttackEffect(Uint32 idx)
{
	Uint16 value;

	if(idx > BATTLE_SPRITE_ROW_LAST)
		return 1;
	value = BattleSprites[idx].attackEff;
	return value ? value : 1;
}

/* Returns how high an enemy hops as it runs up to attack, in pixels; 0 out of
   range. */
Uint32 btSprGetAttackHop(Uint32 idx)
{
	Uint16 value;

	if(idx > BATTLE_SPRITE_ROW_LAST)
		return 0;
	value = BattleSprites[idx].attackHop;
	return value ? value : 0;
}

/* Returns 1 for an enemy whose blow lands with SE_HEAVY_IMPACT, the Venus
   Lighthouse aerie's bosses. */
Sint32 btSprHasHeavyImpact(Sint32 idx)
{
	if(idx < 0 || idx > BATTLE_SPRITE_ROW_LAST)
		return 0;
	return BattleSprites[idx].heavyImpact;
}

/* Returns the height a battle effect is drawn at over an enemy, in pixels, or 0
   for its size's default; 0 out of range. */
Sint32 btSprGetEffectHeight(Sint32 idx)
{
	if(idx < 0 || idx > BATTLE_SPRITE_ROW_LAST)
		return 0;
	return BattleSprites[idx].effHeight;
}
