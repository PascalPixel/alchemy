/// @file
/// Unit_Get and the game flags: bits, bytes and nibbles.
// Part of system.c.

#include "../system_internal.h"
#include "battle/battle_unit.h"
#include "owner_state.h"
#include "system/ram_buffer.h"

void *Unit_Get(s32 owner) {
  struct BattleUnit *states;
  struct BattleUnit *unit;

  if ((u32)owner < 8) {
    states = gOwnerStates;
    unit = &states[owner];
    return unit;
  }
  if ((u32)owner - 0x80 < 6 && (states = Ram_HeapSlots->unkd0[2]) != NULL) {
    unit = &states[owner - 0x80];
    return unit;
  }
  return NULL;
}

s32 GameFlag_Get(s32 flag) {
  s32 bit;
  u32 index;

  index = (u32)flag << 20;
  bit = flag & 7;
  flag = index >> 23;
  return (gGameFlagBytes[flag] >> bit) & 1;
}

s32 GameFlag_Set(s32 flag) {
  s32 bit;
  s32 mask;
  u8 *bytes;

  bit = flag & 7;
  bytes = gGameFlagBytes;
  mask = 1 << bit;
  flag = ((u32)flag << 20) >> 23;
  bytes[flag] |= mask;
  return flag;
}

void GameFlag_Clear(s32 flag) {
  s32 bit;
  s32 mask;
  u8 *bytes;

  bit = flag & 7;
  bytes = gGameFlagBytes;
  mask = 1 << bit;
  flag = ((u32)flag << 20) >> 23;
  bytes[flag] &= ~mask;
}

u32 GameFlag_Toggle(s32 flag) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & flag);
  bytes = gGameFlagBytes;
  flag = ((u32)flag << 20) >> 23;
  bytes[flag] ^= mask;
  value = bytes[flag] & mask;
  return (u32)((0 - value) | value) >> 31;
}

u8 GameFlag_GetByte(s32 flag) {
  flag = ((u32)flag << 20) >> 23;
  return gGameFlagBytes[flag];
}

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

  flag = ((u32)flag << 20) >> 23;
  if (bytes[flag] < 255)
    bytes[flag]++;
  return bytes[flag];
}

/// Decrements the byte holding eight flags, saturating at 0, and returns it.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *bytes = gGameFlagBytes;

  flag = ((u32)flag << 20) >> 23;
  if (bytes[flag] != 0)
    bytes[flag]--;
  return bytes[flag];
}

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

  flag = ((u32)flag << 20) >> 23;
  bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}
