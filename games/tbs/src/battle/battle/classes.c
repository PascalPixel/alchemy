/// @file
/// The enemies' battle sprite table, gBattleSprites.
// Part of battle.c.

#include "../battle_internal.h"

/// Returns the class's defeat-sound kind, 0 to 4, or -1 for none; a fallen
/// enemy's sound is SE_ENEMY_CRY plus it.
/// Called by BattlePlayback_Run as an enemy falls. Reads any index
/// unchecked.
u32 BattleSprite_GetDefeatSound(s32 index) {
  u32 kind = gBattleSprites[index].defeatSound;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

/// The last class with a row in gBattleSprites (battle_sprites.c).
#define ENEMY_CLASS_LAST 171

/// Returns the class's battle sprite, row 0's for a class out of range.
/// Called by BattleObject_Make and EnemySprite_LoadIntoSlots.
s32 BattleSprite_GetSprite(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return gBattleSprites[0].animation;
  return gBattleSprites[index].animation;
}

// XXX (no evidence): whether returning the sprite is a slip copied from
// BattleSprite_GetSprite, as no other getter here returns another column for a
// bad index; only the author's intent would say, since the code behaves the
// same either way for every class in range.
/// Returns the class's sprite palette. For a class out of range it returns row
/// 0's sprite, not a palette.
/// Called by BattleObject_Make and EnemySprite_LoadIntoSlots.
s32 BattleSprite_GetPalette(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return gBattleSprites[0].animation;
  return gBattleSprites[index].palette;
}

/// Returns 1 for a small class, which takes one enemy sprite slot instead of
/// two and stands closer to its neighbours; 0 out of range.
/// Called by the formation and placement code (BattleFormation_BuildEnemyList,
/// BattlePlacement_LayoutEnemies), the sprite slots (Enemy_HasSpriteRoom,
/// EnemySprite_LoadIntoSlots) and motion.c.
s32 BattleSprite_IsSmall(s32 index) {
  s32 result;

  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  result = 0;
  if (gBattleSprites[index].isSmall)
    result = 1;
  return result;
}

/// Returns the animation a class's plain attack shows, 1 for none or out of
/// range.
/// Called by BattleTurn_Build, which stages it by id range
/// (ANIM_BY_ID_RANGE).
u32 BattleSprite_GetAttackAnim(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 1;
  value = gBattleSprites[index].attackAnim;
  return value ? value : 1;
}

/// Returns how high a class hops as it runs up to attack, in pixels; 0 out
/// of range.
/// Called by BattleStaging_RunAttack, which passes it to
/// BattleMotion_ApproachTarget, and reached through FarCall_BattleTable.
u32 BattleSprite_GetAttackHop(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 0;
  value = gBattleSprites[index].attackHop;
  return value ? value : 0;
}

/// Returns 1 for a class whose blow lands with SE_HEAVY_IMPACT instead of
/// SE_IMPACT; 0 out of range. Only the Venus Lighthouse aerie bosses set it in
/// battle_sprites.c: ENEMY_SATUROS_VENUS_LIGHTHOUSE, ENEMY_MENARDI and
/// ENEMY_FUSION_DRAGON.
/// Called by the beam animations (beams.c) through its far stub.
s32 BattleSprite_HasHeavyImpact(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  return gBattleSprites[index].heavyImpact;
}

/// Returns the height an animation is drawn at over a class, in pixels, or 0
/// for its size's default; 0 out of range.
/// Called by BattleUnit_GetEffectHeight, and reached through
/// FarCall_BattleTable.
s32 BattleSprite_GetAnimHeight(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  return gBattleSprites[index].effectHeight;
}
