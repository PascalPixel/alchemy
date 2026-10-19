#include "battle_summon.h"
#include "types.h"

u32 BattleClass_GetDefeatSoundKind(s32 index) {
  u32 kind = gSummon_EntryTable[index].layoutKind;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

s32 BattleClass_GetSprite(s32 index) {
  if ((u32)index > 386)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].animation;
}

s32 BattleClass_GetPalette(s32 index) {
  if ((u32)index > 386)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].layoutBits1;
}

s32 BattleClass_IsSmall(s32 index) {

  s32 result;
  u32 flags;

  if ((u32)index > 386)
    return 0;
  result = 0;
  flags = gSummon_EntryTable[index].spriteBit0;
  if (flags)
    result = 1;
  return result;
}

u32 BattleClass_GetAttackEffect(u32 index) {
  /* FAKEMATCH: the result selection preserves the separate extract and
   * return-move order. */
  u32 value;
  u32 result;

  if (index > 386)
    return 1;
  value = gSummon_EntryTable[index].spriteBits1;
  if (value != 0)
    result = value;
  else
    result = 1;
  return result;
}

u32 Battle_GetSpriteFlagHighBits(u32 index) {
  /* FAKEMATCH: zero-result selection preserves the compare and return move. */
  u32 value;
  u32 result;

  if (index > 386)
    return 0;
  value = gSummon_EntryTable[index].spriteBits5;
  if (value != 0)
    result = value;
  else
    result = 0;
  return result;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index) {
  if ((u32)index > 386)
    return 0;
  return gSummon_EntryTable[index].layoutBit0;
}

u32 BattleClass_GetEffectHeight(u32 index) {
  u8 *table;
  u32 offset;

  if (index > 386)
    return 0;
  table = (u8 *)gSummon_EntryTable;
  /* FAKEMATCH: ⚓️ loads the table before scaling the index */
  asm volatile("");
  offset = index * 8;
  offset += 4;
  return table[offset];
}
