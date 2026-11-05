#include "types.h"
#include "call.h"

s32 Func_020001d4(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}
