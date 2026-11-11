#include "types.h"

extern const s16 gData_0810c3f4[][33];

/// Copies the shop's ordinary stock, up to 24 items, to `output` with a 0
/// after, and returns how many. Called by Func_081082c4 for Buy.
s32 ShopStock_CopyItems(s32 stockRow, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;

  count = 0;
  if (gData_0810c3f4[stockRow][0] != 0) {
    dst = output;
    src = gData_0810c3f4[stockRow];
    do {
      // The row's halfword read through a volatile pointer, a load apart from
      // the end test's; as *src, 41 of 28 instructions differ in tla-en.
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
