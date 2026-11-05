// Near miss, differ 16 of 36 (tla-en, 2026-11-05), with untagged empty asms, one-pass
// blocks and locals found by search; each needs its S2 tag on adoption, measured then.
// Near miss, differ 37 of 36 (2026-10-12; 22 before, with direct calls that the
// native does not have): the three fills go through the resident IWRAM entry,
// loaded once for the first two calls and again after the loop, as the native
// does. What is left is one choice: this draft keeps 0x100 in r5 for both the
// first size and destination += 0x40 (so the entry takes r6 and destination
// r7), where the native builds 0x100 twice (r1 for the call, r3 for the add). A
// one-pass block around the first fill still shares it.
#include "types.h"
#include "iwram_call.h"

void BattleTransition_BuildTilemap(s32 *destination) {
  s32 entry;
  u32 index;

  destination += 0x40;
  {
    __typeof__(destination) hoisted = destination;
    asm("");
  {
    register __typeof__(hoisted) hoisted asm("r0") = hoisted;
    Iwram_FillWords( hoisted , 0x100, -1);
  }
  }
  Iwram_FillWords(destination, 0x80, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 0x20;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  {
    register __typeof__(destination) hoisted asm("r0") = destination;
    Iwram_FillWords( hoisted , 0x280, 0x03ff03ff);
  }
}
