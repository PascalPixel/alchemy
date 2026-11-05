// Draft: overlay 6ac's copy of resource_6aa_c_02004794.c, with this overlay's symbols.
#include "types.h"

s32 Func_020016e8(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}
