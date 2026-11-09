#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "ram_buffer.h"

extern struct BattleUnit gOwnerStates[];

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

s32 GameFlag_Test(s32 flag) {
  s32 bit;
  u32 index;

  index = (u32)flag << 20;
  bit = flag & 7;
  flag = index >> 23;
  return (gGameFlagBytes[flag] >> bit) & 1;
}

s32 GameFlag_SetBit(s32 flag) {
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

void GameFlag_ClearBit(s32 flag) {
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
