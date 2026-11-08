// Func_0810a6b8: The Broken Seal's EventTable_CopyRowHeader over this game's
// gData_0810c3f4.
#include "types.h"

extern const s16 gData_0810c3f4[][33];

s32 Func_0810a6b8(s32 rowNo, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;

  count = 0;
  if (gData_0810c3f4[rowNo][0] != 0) {
    dst = output;
    src = gData_0810c3f4[rowNo];
    do {
      /* FAKEMATCH(volatile): the row's halfword read through a volatile
       * pointer, so the loop's end test reads the next one afresh; as
       * *dst = *src, 41 of 28 instructions differ in tla-en. */
      *dst = *(const volatile s16 *)src;
      count++;
      src++;
      dst++;
      if (count > 23)
        break;
    } while (*src != 0);
  }
  output[count] = 0;
  return count;
}
