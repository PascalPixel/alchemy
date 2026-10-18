#include "types.h"

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

// BattleFx_BuildBlitter(slot, 7, 7, flags, mode) for a 128 by 128 canvas,
// with the flags and the mode packed into one word: mode << 4 | flags.
s32 BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags) {
  return Func_08196404(slot, 7, 7, modeAndFlags & 0xf, (u32)modeAndFlags >> 4);
}
