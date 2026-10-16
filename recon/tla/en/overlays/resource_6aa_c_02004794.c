// Near miss, differ 13: the listing loads 0xffff from the pool (this source
// builds it as a split constant, so the native compare was not SImode), jumps
// from the loop to the exit across that pool, and leaves count - 1 in r2 before
// it copies it to r0. while, for, do-while and a u16 end value were tried.
#include "types.h"

s32 Func_02004794(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  return count - 1;
}
