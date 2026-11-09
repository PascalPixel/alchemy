#include "types.h"
#include "iwram_call.h"

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  Iwram_FillWords(destination, size, value);
}

void Func_08125bb8(s32 *destination) {
  s32 entry;
  u32 index;

  /* FAKEMATCH(wrapper): each fill forwarded through FillWords_, so its size is
   * built in r1 at the call instead of shared with destination's step; as
   * direct Iwram_FillWords calls, 37 of 36 instructions differ in tla-en. */
  FillWords_(destination, 0x100, -1);
  destination += 0x40;
  FillWords_(destination, 0x80, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 0x20;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  FillWords_(destination, 0x280, 0x03ff03ff);
}
