// Near miss: differ 10 (listing 15 instructions, draft 17). The native loads
// 0xffff from the literal pool as a halfword constant and keeps it in r1
// through the loop; the draft builds it in three instructions and moves it
// to ip. The twin of overlay 6B0's Func_020001d4 (same difference).
// Tried: a pointer walk (this form), an indexed loop; a volatile read of the
// entry, to keep it a halfword register (differ 16: 0xffff is still built
// in three instructions, and the entry is copied to ip on each pass).
#include "types.h"

s32 Func_02000978(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}
