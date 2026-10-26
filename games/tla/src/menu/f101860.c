#include "types.h"
#include "owner_state.h"

#define ACTION_MASK 0x3fff
#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000

s32 Func_08101860(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *firstCount, s32 *secondCount) {
  s32 i;
  s32 j;
  s32 total;
  s32 first;
  s32 second;

  total = 0;
  first = 0;
  second = 0;
  for (i = 0; i < 32 && b[i].encodedAction != 0; i++) {
    out[total] = b[i].encodedAction & ACTION_MASK;
    total++;
    for (j = 0; j < 32; j++) {
      if (((b[i].encodedAction ^ a[j].encodedAction) & ACTION_MASK) == 0)
        break;
    }
    if (j == 32) {
      first++;
      out[total - 1] |= FLAG_FIRST;
    }
  }
  for (i = 0; i < 32 && a[i].encodedAction != 0; i++) {
    for (j = 0; j < 32; j++) {
      if (((a[i].encodedAction ^ b[j].encodedAction) & ACTION_MASK) == 0)
        break;
    }
    if (j == 32) {
      second++;
      out[total] = (a[i].encodedAction & ACTION_MASK) | FLAG_SECOND;
      total++;
    }
  }
  *firstCount = first;
  *secondCount = second;
  return total;
}
