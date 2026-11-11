#include "types.h"

extern u8 gGameFlagBytes[];

/// Sets the four bits `flag` is in.
void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 fieldMask = 0xF;
  s32 shift = 4 & flag;
  s32 mask = fieldMask << shift;
  u8 *bytes = gGameFlagBytes;

  flag = ((u32)flag << 20) >> 23;
  bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}
