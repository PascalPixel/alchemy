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

void *UnitGet(s32 owner) {
  struct BattleUnit *states;
  struct BattleUnit *unit;

  if ((u32)owner < 8) {
    /* FAKEMATCH: the record size as a forced temporary is built first, and a
     * one-pass block loads the table before the size is scaled; indexed
     * straight the scaling comes before the load (differ 2), and either device
     * alone leaves the same two rows apart. */
    s32 offset = sizeof(struct BattleUnit);

    do {
      states = gOwnerStates;
    } while (0);
    offset *= owner;
    unit = (struct BattleUnit *)((u8 *)states + offset);
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
  /* FAKEMATCH: Both ordinary forms load the flag base after mask setup; keep
   * that load first. */
  __asm__("" : : "r"(bit), "r"(bytes));
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
  /* FAKEMATCH: Both ordinary forms load the flag base after mask setup; keep
   * that load first. */
  __asm__("" : : "r"(bit), "r"(bytes));
  mask = 1 << bit;
  flag = ((u32)flag << 20) >> 23;
  bytes[flag] &= ~mask;
}

u32 GameFlag_Toggle(s32 flag) {
  s32 bit;
  s32 mask;
  u8 *bytes;
  s32 value;

  bit = 7;
  bytes = gGameFlagBytes;
  /* FAKEMATCH: Without it the flag base loads after the mask is built (differ
   * 2); keep that load before the and. */
  __asm__("" : : "r"(bytes));
  bit &= flag;
  mask = 1;
  /* FAKEMATCH: Keeps the 1 after the and; naming the base again keeps it in r2
   * and the mask in r1 (differ 16 without). */
  __asm__("" : : "r"(mask), "r"(bytes));
  mask <<= bit;
  flag = ((u32)flag << 20) >> 23;
  bytes[flag] ^= mask;
  value = bytes[flag] & mask;
  return (u32)((0 - value) | value) >> 31;
}
