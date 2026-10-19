/// @file
/// Part of game.c, the game object, compiled only through it.
/// The game flags: bits, bytes and nibbles.

#include "../game_internal.h"

// The game flags: 4096 bits in gGameFlagBytes. A flag number's low three bits
// pick the bit and the next nine the byte; byte and nibble flags use the same
// numbering.

/// Returns 1 when `flag` is set.
s32 GameFlag_Test(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;
  s32 flagValue;

  flagMask = 1 << (7 & flag);
  flag = GAME_FLAG_BYTE(flag);
  flagBytes = gGameFlagBytes;
  flagValue = flagBytes[flag] & flagMask;
  return (u32)((0 - flagValue) | flagValue) >> 0x1F;
}

/// Sets `flag`. Returns its byte number.
s32 GameFlag_SetBit(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;

  flagMask = 1 << (7 & flag);
  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  flagBytes[flag] |= flagMask;
  return flag;
}

/// Clears `flag`.
void GameFlag_ClearBit(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;

  flagMask = ~(1 << (7 & flag));
  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  flagBytes[flag] &= flagMask;
}

/// Flips `flag` and returns its new value.
u32 GameFlag_Toggle(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;
  s32 flagValue;

  flagMask = 1 << (7 & flag);
  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  flagBytes[flag] ^= flagMask;
  flagValue = flagBytes[flag] & flagMask;
  return (u32)((0 - flagValue) | flagValue) >> 31;
}

/// Returns the whole byte `flag` is in.
s32 GameFlag_GetByte(s32 flag) {
  u32 byteIndex;

  byteIndex = (u32)flag << 20;
  flag = byteIndex >> 23;
  return gGameFlagBytes[flag];
}

/// Sets the whole byte `flag` is in.
void GameFlag_SetByte(s32 flag, s32 value) {
  u32 shifted = (u32)flag << 20;
  flag = shifted >> 23;
  gGameFlagBytes[flag] = value;
}

/// Adds 1 to the byte `flag` is in, stopping at 255, and returns it.
u8 GameFlag_IncrementByte(s32 flag) {
  u8 *flagBytes;

  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  if (flagBytes[flag] <= 0xFE) {
    flagBytes[flag]++;
  }
  return flagBytes[flag];
}

/// Takes 1 from the byte `flag` is in, stopping at 0, and returns it.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *flagBytes;

  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  if (flagBytes[flag] != 0) {
    flagBytes[flag] += 0xFF;
  }
  return flagBytes[flag];
}

/// Returns the four bits `flag` is in (bit 2 of the number picks the half).
s32 GameFlag_GetNibble(u32 flag) {
  u32 shifted = (u32)flag << 20;
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
