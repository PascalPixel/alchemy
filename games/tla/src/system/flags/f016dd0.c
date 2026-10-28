#include "types.h"

extern u8 gGameFlagBytes[];

void Func_08016dd0(s32 flag, s32 value) {
  s32 fieldMask = 0xF;
  s32 shift = 4 & flag;
  s32 mask = fieldMask << shift;
  u8 *bytes = gGameFlagBytes;

  flag = ((u32)flag << 20) >> 23;
  bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}
