// Near miss of Func_020001d4 (15 native instructions, 7 differ): the native
// loads the 0xffff terminator from the literal pool and compares in r1 on both
// tests. With the terminator in a u32 local the loop's registers match (as a
// literal it is copied to ip: 10); what is left is the pool load itself, which
// a split constant never becomes in this compiler, and the branch over the pool
// that follows from it. A signed list compared with -1, a u16 local, casts of
// either side and the loop spelled with gotos measured the same or worse.
#include "types.h"

s32 Func_020001d4(u16 *list) {
  s32 count = 0;
  u32 end = 0xffff;

  while (*list != end) {
    list++;
    count++;
  }
  count--;
  return count;
}
