// Part of battle.c, the battle object, compiled only through it.
// The enemy classes' battle sprite table, gSummon_EntryTable.

#include "../battle_internal.h"

// Returns the enemy sprite's defeat-sound layout kind; values above four become
// -1.
u32 BattleClass_GetDefeatSoundKind(s32 index) {
  u32 kind = gSummon_EntryTable[index].layoutKind;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

// The last class with a row in gSummon_EntryTable.
#define ENEMY_CLASS_LAST 171

// Returns the enemy-class sprite animation, using row zero for an invalid
// class.
s32 BattleClass_GetSprite(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].animation;
}

// Returns the enemy-class palette/layout bits. An invalid class returns row
// zero's animation field.
s32 BattleClass_GetPalette(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].layoutBits1;
}

// Returns whether the enemy class uses a single sprite slot; invalid classes
// return zero.
s32 BattleClass_IsSmall(s32 index) {
  s32 result;

  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  result = 0;
  if (gSummon_EntryTable[index].spriteBit0)
    result = 1;
  return result;
}

// Returns the class's attack-effect kind, substituting one for zero or an
// invalid class.
u32 Battle_GetSpriteFlagNibble(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 1;
  value = gSummon_EntryTable[index].spriteBits1;
  return value ? value : 1;
}

// Returns the class's attack-hop height, or zero for an invalid class.
u32 Battle_GetSpriteFlagHighBits(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 0;
  value = gSummon_EntryTable[index].spriteBits5;
  return value ? value : 0;
}

// Returns layout bit zero for an enemy class, or zero for an invalid class.
s32 Summon_IsEntrySecondaryFlagged(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  return gSummon_EntryTable[index].layoutBit0;
}

// Returns the class's effect-height byte, or zero for an invalid class.
s32 BattleClass_GetEffectHeight(s32 index) {
  if (index < 0 || index > ENEMY_CLASS_LAST)
    return 0;
  return gSummon_EntryTable[index].unk04[0];
}
