// Draft of overlay 6ae's Func_0200508c, the twin of overlay 6b0's
// Func_020001d4: its draft (resource_6b0_c_020001d4.c) with this overlay's
// symbols: differ 10 of 15. Left: what that draft's header records.
#include "types.h"

s32 Func_0200508c(u16 *list) {
  s32 count;

  for (count = 0; list[count] != 0xffff; count++)
    ;
  count--;
  return count;
}
