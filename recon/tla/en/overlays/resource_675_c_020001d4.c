// Draft, differ 3 of 15 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 3 (tla-en); notes below may predate it.
// Draft, differ 13 of 15: the native loads 0xffff from the literal pool before
// the loop and jumps over the pool to the count - 1 tail; this form builds
// 0xffff in three instructions and falls through.
#include "types.h"
#include "call.h"

s32 Func_020001d4(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  return count - 1;
}
