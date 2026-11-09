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
  u16 value;

  if (index > 386)
    return 1;
  value = gSummon_EntryTable[index].spriteBits1;
  return value ? value : 1;
}

u32 BattleClass_GetAttackHop(u32 index) {
  u16 value;

  if (index > 386)
    return 0;
  value = gSummon_EntryTable[index].spriteBits5;
  return value ? value : 0;
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
  offset = index * 8;
  offset += 4;
  return table[offset];
}
