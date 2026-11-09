#include "types.h"

s32 Func_020001d4(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}
