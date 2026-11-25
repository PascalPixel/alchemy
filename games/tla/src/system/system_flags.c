/// @file
/// Unit_Get and the game flags: bits, bytes and nibbles.
// Part of system.c.

#include "system_internal.h"
#include "battle/battle_unit.h"
#include "game/class_djinn.h"
#include "system/ram_buffer.h"
#include "game/game_flags.h"

/// Returns the unit of unitId: ISAAC to PIERS the characters, the
/// ENEMY_BATTLE_SIZE from FIRST_ENEMY_UNIT the enemies of the current battle;
/// NULL for anything else.
struct Unit *Unit_Get(u32 unitId) {
  struct Unit *states;
  struct Unit *unit;

  if (unitId <= PIERS) {
    states = gCharacterUnits;
    unit = &states[unitId];
    return unit;
  }
  if (unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE &&
      (states = Ram_HeapSlots->enemyUnits) != NULL) {
    unit = &states[unitId - FIRST_ENEMY_UNIT];
    return unit;
  }
  return NULL;
}

// The game flags: 4096 bits in gGameFlagBytes. A flag number's low three bits
// pick the bit and the next nine the byte; byte and nibble flags use the same
// numbering. Bits above the twelfth are dropped (GAME_FLAG_BYTE), so a number
// past 4095 wraps onto a flag below it rather than reaching other memory.

/// Returns 1 when `flag` is set and 0 when it is clear.
s32 GameFlag_Get(s32 flag) {
  s32 bit = flag & 7;

  flag = GAME_FLAG_BYTE(flag);
  return (gGameFlagBytes[flag] >> bit) & 1;
}

/// Sets `flag` and returns its byte number.
s32 GameFlag_Set(s32 flag) {
  s32 bit = GAME_FLAG_BIT(flag);
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  flags[flag] |= bit;
  return flag;
}

/// Clears `flag`.
void GameFlag_Clear(s32 flag) {
  s32 bit = GAME_FLAG_BIT(flag);
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  flags[flag] &= ~bit;
}

/// Unused: flips `flag` and returns its new value.
u32 GameFlag_Toggle(s32 flag) {
  s32 bit = GAME_FLAG_BIT(flag);
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  flags[flag] ^= bit;
  if (flags[flag] & bit)
    return 1;
  return 0;
}

/// Returns the whole byte `flag` is in, a counter kept among the flags.
s32 GameFlag_GetByte(s32 flag) {
  flag = GAME_FLAG_BYTE(flag);
  return gGameFlagBytes[flag];
}

/// Sets the whole byte `flag` is in.
u32 GameFlag_SetByte(u32 flag, u8 value) {
  flag = GAME_FLAG_BYTE(flag);
  gGameFlagBytes[flag] = value;
  return flag;
}

/// The most a counter byte among the flags holds.
#define GAME_FLAG_BYTE_MAX 255

/// Unused: adds 1 to the byte `flag` is in, stopping at GAME_FLAG_BYTE_MAX, and
/// returns it.
u8 GameFlag_IncrementByte(s32 flag) {
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  if (flags[flag] < GAME_FLAG_BYTE_MAX)
    flags[flag]++;
  return flags[flag];
}

/// Unused: takes 1 from the byte `flag` is in, stopping at 0, and returns it.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  if (flags[flag] != 0)
    flags[flag]--;
  return flags[flag];
}

/// The bits of a nibble flag, low or high in its byte by its number's bit 2.
#define GAME_FLAG_NIBBLE_MASK 0xf

/// Unused: returns the four bits a flag is in, 0 to 15.
s32 GameFlag_GetNibble(u32 flag) {
  s32 shift = flag & 4;
  s32 mask = GAME_FLAG_NIBBLE_MASK << shift;
  flag = GAME_FLAG_BYTE(flag);

  return (gGameFlagBytes[flag] & mask) >> shift;
}

/// Unused: sets the four bits `flag` is in.
void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 nibble = GAME_FLAG_NIBBLE_MASK;
  s32 shift = flag & 4;
  s32 mask = nibble << shift;
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  flags[flag] = (flags[flag] & ~mask) | ((value & nibble) << shift);
}
