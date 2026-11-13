/// @file
/// The game flags: bits, bytes and nibbles.
// Part of game.c.

#include "../game_internal.h"

// The game flags: 4096 bits in gGameFlagBytes. A flag number's low three bits
// pick the bit and the next nine the byte; byte and nibble flags use the same
// numbering. Bits above the twelfth are dropped (GAME_FLAG_BYTE), so a number
// past 4095 wraps onto a flag below it rather than reaching other memory.

/// Returns 1 when `flag` is set. Every bank tests flags through it, as
/// GameFlag_GetFar, and the events directly or through event.h.
s32 GameFlag_Get(s32 flag) {
  s32 bit = GAME_FLAG_BIT(flag);

  flag = GAME_FLAG_BYTE(flag);
  return NONZERO(gGameFlagBytes[flag] & bit);
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

/// Flips `flag` and returns its new value. Only GAME.S's bare veneer reaches
/// it; nothing calls it by name.
u32 GameFlag_Toggle(s32 flag) {
  s32 bit = GAME_FLAG_BIT(flag);
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  flags[flag] ^= bit;
  return NONZERO(flags[flag] & bit);
}

/// Returns the whole byte `flag` is in, a counter kept among the flags.
s32 GameFlag_GetByte(s32 flag) {
  flag = GAME_FLAG_BYTE(flag);

  return gGameFlagBytes[flag];
}

/// Sets the whole byte `flag` is in. Reached through FieldGameFlag_SetByte by
/// the Colosso maps, the world map and the Battle Arena's win counts.
void GameFlag_SetByte(s32 flag, s32 value) {
  flag = GAME_FLAG_BYTE(flag);

  gGameFlagBytes[flag] = value;
}

/// Adds 1 to the byte `flag` is in, stopping at 255, and returns it. Only
/// GAME.S's bare veneer reaches it; nothing calls it by name.
u8 GameFlag_IncrementByte(s32 flag) {
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  if (flags[flag] < 255)
    flags[flag]++;
  return flags[flag];
}

/// Takes 1 from the byte `flag` is in, stopping at 0, and returns it. Only
/// GAME.S's bare veneer reaches it; nothing calls it by name.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  if (flags[flag] != 0)
    flags[flag]--;
  return flags[flag];
}

/// Returns the four bits `flag` is in, 0 to 15 (bit 2 of the number picks the
/// half: flags n to n + 3, with n a multiple of 4, are one nibble). Only
/// GAME.S's bare veneer reaches it; nothing calls it by name.
s32 GameFlag_GetNibble(u32 flag) {
  s32 shift = flag & 4;
  s32 mask = 15 << shift;
  flag = GAME_FLAG_BYTE(flag);

  return (gGameFlagBytes[flag] & mask) >> shift;
}

/// Sets the four bits `flag` is in. Only GAME.S's bare veneer reaches it;
/// nothing calls it by name.
void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 nibble = 15;
  s32 shift = flag & 4;
  s32 mask = nibble << shift;
  u8 *flags = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);

  flags[flag] = (flags[flag] & ~mask) | ((value & nibble) << shift);
}
