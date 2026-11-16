/// @file
/// Unit_Get and the game flags: bits, bytes and nibbles.
// Part of system.c.

#include "system_internal.h"
#include "battle/battle_unit.h"
#include "game/owner_state.h"
#include "system/ram_buffer.h"
#include "game/game_flags.h"

/// Returns the unit of unitId: ISAAC to PIERS the characters, the six from
/// FIRST_ENEMY_UNIT the enemies of the current battle; NULL for anything else.
struct Unit *Unit_Get(u32 unitId) {
  struct Unit *states;
  struct Unit *unit;

  if (unitId <= PIERS) {
    states = gCharacterUnits;
    unit = &states[unitId];
    return unit;
  }
  if (unitId - FIRST_ENEMY_UNIT < 6 &&
      (states = Ram_HeapSlots->enemyUnits) != NULL) {
    unit = &states[unitId - FIRST_ENEMY_UNIT];
    return unit;
  }
  return NULL;
}

/// Returns 1 when `flag` is set and 0 when it is clear.
s32 GameFlag_Get(s32 flag) {
  s32 bit;
  u32 index;

  index = (u32)flag << 20;
  bit = flag & 7;
  flag = index >> 23;
  return (gGameFlagBytes[flag] >> bit) & 1;
}

/// Sets `flag` and returns its byte number.
s32 GameFlag_Set(s32 flag) {
  s32 bit;
  s32 mask;
  u8 *bytes;

  bit = flag & 7;
  bytes = gGameFlagBytes;
  mask = 1 << bit;
  flag = GAME_FLAG_BYTE(flag);
  bytes[flag] |= mask;
  return flag;
}

/// Clears `flag`.
void GameFlag_Clear(s32 flag) {
  s32 bit;
  s32 mask;
  u8 *bytes;

  bit = flag & 7;
  bytes = gGameFlagBytes;
  mask = 1 << bit;
  flag = GAME_FLAG_BYTE(flag);
  bytes[flag] &= ~mask;
}

/// Flips `flag` and returns its new value. Nothing calls it or its far-call
/// export.
u32 GameFlag_Toggle(s32 flag) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & flag);
  bytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  bytes[flag] ^= mask;
  value = bytes[flag] & mask;
  return (u32)((0 - value) | value) >> 31;
}

/// Returns the whole byte `flag` is in, a counter kept among the flags.
s32 GameFlag_GetByte(s32 flag) {
  flag = GAME_FLAG_BYTE(flag);
  return gGameFlagBytes[flag];
}

/// Sets the whole byte `flag` is in.
u32 GameFlag_SetByte(u32 flag, u8 value) {
  u32 shifted = flag << 20;
  flag = shifted >> 23;
  gGameFlagBytes[flag] = value;
  return flag;
}

/// Increments the byte holding eight flags, saturating at 255, and returns
/// it.
u8 GameFlag_IncrementByte(s32 flag) {
  u8 *bytes = gGameFlagBytes;

  flag = GAME_FLAG_BYTE(flag);
  if (bytes[flag] < 255)
    bytes[flag]++;
  return bytes[flag];
}

/// Decrements the byte holding eight flags, saturating at 0, and returns it.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *bytes = gGameFlagBytes;

  flag = GAME_FLAG_BYTE(flag);
  if (bytes[flag] != 0)
    bytes[flag]--;
  return bytes[flag];
}

/// Returns the four bits a flag is in, 0 to 15.
s32 GameFlag_GetNibble(u32 flag) {
  u32 shifted = flag << 20;
  u32 shift;
  u32 mask;
  s32 result;

  shift = flag & 4;
  mask = 15;
  flag = shifted >> 23;
  result = gGameFlagBytes[flag];
  result &= mask << shift;
  return result >> shift;
}

/// Sets the four bits `flag` is in.
void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 fieldMask = 0xF;
  s32 shift = 4 & flag;
  s32 mask = fieldMask << shift;
  u8 *bytes = gGameFlagBytes;

  flag = GAME_FLAG_BYTE(flag);
  bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}
