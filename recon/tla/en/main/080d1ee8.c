// Near miss, differ 2 of 15: the native loads the table address before it
// builds the loop's -1 (ldr r3; movs r1, #1); here sched2 ranks the two
// equal-priority insns by dependants and builds the -1 first. A loop with
// returns keeps the native's top-tested, unrotated loop with the -1 hoisted;
// a goto loop (differ 6) and the for (;;) with breaks (differ 15) do not.
// permute (3 min) found nothing closer.
#include "types.h"

extern const s32 gData_080f3228[];

s32 Func_080d1ee8(s32 key) {
  const s32 *entry = gData_080f3228;
  s32 value;

  while (1) {
    value = entry[0];
    if (value == -1)
      return value;
    if (value == key)
      return entry[1];
    entry += 2;
  }
}
