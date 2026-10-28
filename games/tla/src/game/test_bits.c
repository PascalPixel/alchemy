#include "types.h"

s32 FlagBits_Test(u8 *bytes, s32 flag) {
  s32 bit;
  u32 index;

  bit = flag & 7;
  index = (u32)flag << 20;
  flag = index >> 23;
  return (bytes[flag] >> bit) & 1;
}
