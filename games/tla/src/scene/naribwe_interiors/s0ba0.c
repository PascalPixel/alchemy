#include "types.h"
#include "event_inline.h"

// Lists the flags still unset for a group: from 7 up to the limit, skipping 7
// in group 0 and 9 and 10 in group 1, each flag group * 20 + i + 48 not yet set
// goes to the output; returns how many.
s32 Func_02000ba0(s32 limit, s32 group, s32 *out) {
  s32 count = 0;
  s32 i = 7;

  if (i < limit) {
    s32 *cursor = out;

    do {
      s32 flag;

      if (group == 0 && i == 7)
        goto next;
      if (group == 1) {
        if (i == 9)
          goto next;
        if (i == 10)
          goto next;
      }
      flag = group * 20 + i + 48;
      if (GameFlag_Test(flag) == 0) {
        *cursor++ = flag;
        count++;
      }
    next:
      i++;
    } while (i < limit);
  }
  return count;
}
