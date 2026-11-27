// Reading a unit's row of BattleSprites by its id (UNIT.id): an enemy's sprite, palette, size,
// attack and defeat sound. Part of battle.c.

#include "battle_internal.h"

// Returns the enemy's defeat-sound kind, 0 to 4, or -1 for none; a fallen enemy's sound is
// SE_ENEMY_CRY plus it. Reads any index unchecked.
Uint32 btSpriteGetDefeatSound(Sint32 index)
{
	Uint32 kind = BattleSprites[index].defeatSound;

	if ((Sint32)kind > 4)
		kind = -1U;
	return kind;
}

// The last row of BattleSprites (battle_sprites.c).
#define BATTLE_SPRITE_ROW_LAST 171

// Returns the enemy's battle sprite, row 0's for an index out of range.
Sint32 btSpriteGetSprite(Sint32 index)
{
	if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
		return BattleSprites[0].animation;
	return BattleSprites[index].animation;
}

// XXX (no evidence): whether returning the sprite for a bad index is a slip copied from
// btSpriteGetSprite. Returns the enemy's sprite palette. For an index out of range it returns row
// 0's sprite, not a palette.
Sint32 btSpriteGetPalette(Sint32 index)
{
	if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
		return BattleSprites[0].animation;
	return BattleSprites[index].palette;
}

// Returns 1 for a small enemy, which takes one enemy sprite slot instead of two and stands closer
// to its neighbours; 0 out of range.
Sint32 btSpriteIsSmall(Sint32 index)
{
	Bool result;

	if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
		return 0;
	result = FALSE;
	if (BattleSprites[index].isSmall)
		result = TRUE;
	return result;
}

// Returns the beam an enemy's plain attack shows, 1 for none or out of range.
Uint32 btSpriteGetAttackEffect(Uint32 index)
{
	Uint16 value;

	if (index > BATTLE_SPRITE_ROW_LAST)
		return 1;
	value = BattleSprites[index].attackEffect;
	return value ? value : 1;
}

// Returns how high an enemy hops as it runs up to attack, in pixels; 0 out of range.
Uint32 btSpriteGetAttackHop(Uint32 index)
{
	Uint16 value;

	if (index > BATTLE_SPRITE_ROW_LAST)
		return 0;
	value = BattleSprites[index].attackHop;
	return value ? value : 0;
}

// Returns 1 for an enemy whose blow lands with SE_HEAVY_IMPACT, the Venus Lighthouse aerie's
// bosses.
Sint32 btSpriteHasHeavyImpact(Sint32 index)
{
	if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
		return 0;
	return BattleSprites[index].heavyImpact;
}

// Returns the height a battle effect is drawn at over an enemy, in pixels, or 0 for its size's
// default; 0 out of range.
Sint32 btSpriteGetEffectHeight(Sint32 index)
{
	if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
		return 0;
	return BattleSprites[index].effectHeight;
}
