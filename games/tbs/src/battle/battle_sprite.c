/// @file
/// Reading a unit's row of gBattleSprites by its id (Unit.id): an enemy's
/// sprite, palette, size, attack and defeat sound.
// Part of battle.c.

#include "battle_internal.h"

/// Returns the enemy's defeat-sound kind, 0 to 4, or -1 for none; a fallen
/// enemy's sound is SE_ENEMY_CRY plus it. Reads any index unchecked.
u32 BattleSprite_GetDefeatSound(s32 index) {
  u32 kind = gBattleSprites[index].defeatSound;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

/// The last row of gBattleSprites (battle_sprites.c).
#define BATTLE_SPRITE_ROW_LAST 171

/// Returns the enemy's battle sprite, row 0's for an index out of range.
s32 BattleSprite_GetSprite(s32 index) {
  if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
    return gBattleSprites[0].animation;
  return gBattleSprites[index].animation;
}

// XXX (no evidence): whether returning the sprite for a bad index is a slip
// copied from BattleSprite_GetSprite.
/// Returns the enemy's sprite palette. For an index out of range it returns
/// row 0's sprite, not a palette.
s32 BattleSprite_GetPalette(s32 index) {
  if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
    return gBattleSprites[0].animation;
  return gBattleSprites[index].palette;
}

/// Returns 1 for a small enemy, which takes one enemy sprite slot instead of
/// two and stands closer to its neighbours; 0 out of range.
s32 BattleSprite_IsSmall(s32 index) {
  s32 result;

  if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
    return 0;
  result = 0;
  if (gBattleSprites[index].isSmall)
    result = 1;
  return result;
}

/// Returns the beam an enemy's plain attack shows, 1 for none or out of range.
u32 BattleSprite_GetAttackEffect(u32 index) {
  u16 value;

  if (index > BATTLE_SPRITE_ROW_LAST)
    return 1;
  value = gBattleSprites[index].attackEffect;
  return value ? value : 1;
}

/// Returns how high an enemy hops as it runs up to attack, in pixels; 0 out of
/// range.
u32 BattleSprite_GetAttackHop(u32 index) {
  u16 value;

  if (index > BATTLE_SPRITE_ROW_LAST)
    return 0;
  value = gBattleSprites[index].attackHop;
  return value ? value : 0;
}

/// Returns 1 for an enemy whose blow lands with SE_HEAVY_IMPACT, the Venus
/// Lighthouse aerie's bosses.
s32 BattleSprite_HasHeavyImpact(s32 index) {
  if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
    return 0;
  return gBattleSprites[index].heavyImpact;
}

/// Returns the height a battle effect is drawn at over an enemy, in pixels, or
/// 0 for its size's default; 0 out of range.
s32 BattleSprite_GetEffectHeight(s32 index) {
  if (index < 0 || index > BATTLE_SPRITE_ROW_LAST)
    return 0;
  return gBattleSprites[index].effectHeight;
}
