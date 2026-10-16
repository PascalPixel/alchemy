// Near miss, differ 37 of 36 (2026-10-12; 22 before, with direct calls that the
// native does not have): the three fills go through the resident IWRAM entry,
// loaded once for the first two calls and again after the loop, as the native
// does. What is left is one choice: this draft keeps 0x100 in r5 for both the
// first size and destination += 0x40 (so the entry takes r6 and destination
// r7), where the native builds 0x100 twice (r1 for the call, r3 for the add). A
// one-pass block around the first fill still shares it.
#include "types.h"
#include "iwram_call.h"

void BattlePresentation_BuildTilemap(s32 *destination) {
  s32 entry;
  u32 index;

  Iwram_FillWords(destination, 0x100, -1);
  destination += 0x40;
  Iwram_FillWords(destination, 0x80, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 0x20;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}
