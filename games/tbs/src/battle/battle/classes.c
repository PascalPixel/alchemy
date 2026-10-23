/// @file
/// The enemy classes' battle sprite table, gSummon_EntryTable.
// Part of battle.c.

#include "../battle_internal.h"

/// Returns the class's defeat-sound kind, 0 to 4, or -1 for none; a fallen
/// enemy's sound is SE_146 plus it.
/// Called by BattleEvent_Playback as an enemy falls. Reads any index
/// unchecked.
u32 BattleClass_GetDefeatSoundKind(s32 index) {
  u32 kind = gSummon_EntryTable[index].layoutKind;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

/// The last class with a row in gSummon_EntryTable (sprites.tsv).
#define ENEMY_CLASS_LAST 171

/// Returns the class's battle sprite, row 0's for a class out of range.
/// Called by BattleObject_Make and SummonSlot_RegisterActorSprites.
s32 BattleClass_GetSprite(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].animation;
}

/// Returns the class's sprite palette.
/// For a class out of range it returns row 0's sprite, not a palette.
/// XXX: likely a slip copied from BattleClass_GetSprite, because no other
/// getter here returns another column for a bad index. Called by
/// BattleObject_Make and SummonSlot_RegisterActorSprites.
s32 BattleClass_GetPalette(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].layoutBits1;
}

/// Returns 1 for a small class, which takes one enemy sprite slot instead of
/// two and stands closer to its neighbours; 0 out of range.
/// Called by the formation and placement code (BattleFormation_BuildEnemyList,
/// BattlePlacement_LayoutEnemies), the sprite slots (Enemy_CheckRoom,
/// SummonSlot_RegisterActorSprites) and motion.c.
s32 BattleClass_IsSmall(s32 index) {
  s32 result;

  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  result = 0;
  if (gSummon_EntryTable[index].spriteBit0)
    result = 1;
  return result;
}

/// Returns the effect a class's plain attack shows, 1 for none or out of
/// range.
/// Called by BattleCommand_BuildPlan, which stages it by id range
/// (STAGING_BY_ID_RANGE).
u32 BattleClass_GetAttackEffect(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 1;
  value = gSummon_EntryTable[index].spriteBits1;
  return value ? value : 1;
}

/// Returns how high a class hops as it runs up to attack, in pixels; 0 out
/// of range.
/// Called by BattlePresentation_RunAttack, which passes it to
/// BattleMotion_ApproachTarget, and reached through FarCall_BattleTable.
u32 BattleClass_GetAttackHop(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 0;
  value = gSummon_EntryTable[index].spriteBits5;
  return value ? value : 0;
}

/// Returns 1 for a class whose blow lands with SE_HEAVY_IMPACT instead of
/// SE_IMPACT; 0 out of range. Only the Mars Lighthouse bosses set it in
/// sprites.tsv: ENEMY_SATUROS_2, ENEMY_MENARDI and ENEMY_FUSION_DRAGON.
/// Called by the beam effects (beams.c) through its far stub.
s32 BattleClass_HasHeavyImpact(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  return gSummon_EntryTable[index].heavyImpact;
}

/// Returns the height an effect is drawn at over a class, in pixels, or 0
/// for its size's default; 0 out of range.
/// Called by BattleUnit_GetEffectHeight, and reached through
/// FarCall_BattleTable.
s32 BattleClass_GetEffectHeight(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  return gSummon_EntryTable[index].effectHeight;
}
