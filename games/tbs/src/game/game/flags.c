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
  return (gGameFlagBytes[GAME_FLAG_BYTE(flag)] & GAME_FLAG_BIT(flag)) != 0;
}

/// Sets `flag` and returns its byte number. Called from every bank, as
/// GameFlag_SetFar, and by the events directly or through the event
/// layer.
s32 GameFlag_Set(s32 flag) {
  u32 byte = GAME_FLAG_BYTE(flag);

  gGameFlagBytes[byte] |= GAME_FLAG_BIT(flag);
  return byte;
}

/// Clears `flag`. Called from every bank, as GameFlag_ClearFar, and by the
/// events directly or through event.h.
void GameFlag_Clear(s32 flag) {
  gGameFlagBytes[GAME_FLAG_BYTE(flag)] &= ~GAME_FLAG_BIT(flag);
}

/// Flips `flag` and returns its new value. Only GAME.S's bare veneer reaches
/// it; nothing calls it by name.
u32 GameFlag_Toggle(s32 flag) {
  u32 byte = GAME_FLAG_BYTE(flag);

  gGameFlagBytes[byte] ^= GAME_FLAG_BIT(flag);
  return (gGameFlagBytes[byte] & GAME_FLAG_BIT(flag)) != 0;
}

/// Returns the whole byte `flag` is in, a counter kept among the flags. Called
/// by the battle encounter code and, through FieldGameFlag_GetByte, by the
/// Colosso scenes and the Battle Arena.
s32 GameFlag_GetByte(s32 flag) { return gGameFlagBytes[GAME_FLAG_BYTE(flag)]; }

/// Sets the whole byte `flag` is in. Reached through FieldGameFlag_SetByte by
/// the Colosso maps, the world map and the Battle Arena's win counts.
void GameFlag_SetByte(s32 flag, s32 value) {
  gGameFlagBytes[GAME_FLAG_BYTE(flag)] = value;
}

/// Adds 1 to the byte `flag` is in, stopping at 255, and returns it. Only
/// GAME.S's bare veneer reaches it; nothing calls it by name.
u8 GameFlag_IncrementByte(s32 flag) {
  u8 *counter = &gGameFlagBytes[GAME_FLAG_BYTE(flag)];

  if (*counter < 255)
    (*counter)++;
  return *counter;
}

/// Takes 1 from the byte `flag` is in, stopping at 0, and returns it. Only
/// GAME.S's bare veneer reaches it; nothing calls it by name.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *counter = &gGameFlagBytes[GAME_FLAG_BYTE(flag)];

  if (*counter != 0)
    (*counter)--;
  return *counter;
}

/// Returns the four bits `flag` is in, 0 to 15 (bit 2 of the number picks the
/// half: flags n to n + 3, with n a multiple of 4, are one nibble). Only
/// GAME.S's bare veneer reaches it; nothing calls it by name.
s32 GameFlag_GetNibble(u32 flag) {
  s32 shift = flag & 4;

  return (gGameFlagBytes[GAME_FLAG_BYTE(flag)] >> shift) & 15;
}

/// Sets the four bits `flag` is in. Only GAME.S's bare veneer reaches it;
/// nothing calls it by name.
void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 shift = flag & 4;
  u8 *bits = &gGameFlagBytes[GAME_FLAG_BYTE(flag)];

  *bits = (*bits & ~(15 << shift)) | ((value & 15) << shift);
}
