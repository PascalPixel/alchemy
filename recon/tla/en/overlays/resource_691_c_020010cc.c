// Draft, differ 6 of 33: the sentinel 0xffff is built movs, lsls, adds with the
// table load after lsls; the listing loads between movs and lsls. Same as
// Func_02001084; tried operand order, casts, unsigned literals.
#include "types.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

s32 Func_020010cc(s32 x, s32 z, u16 *table) {
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, x >> 20, z >> 20) >> 8;

  while (*table != 0xffff) {
    table++;
    if (code == *table) {
      result = table[1];
      break;
    }
    table++;
    result = *table++;
  }
  return result;
}
