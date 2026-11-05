// Draft: matched under -O2, back on its scaffold when the overlays moved to -Os (2026-11-02).
#include "types.h"

extern u16 gData_02001a14, gData_02001a1c, gData_02001a20, gData_02001a30,
    gData_02001a34;
extern u16 *gData_02001a18, *gData_02001a24, *gData_02001a28, *Data_02001a2c;

void Func_020001f8(void) {
  u16 *colour;
  u16 *slot;
  s32 due = 0;
  u32 i;
  s32 r, g, b;
  s32 index;
  u16 **list;

  if (gData_02001a1c != 0 && --gData_02001a1c == 0) {
    due = 1;
    gData_02001a1c = gData_02001a20;
    if (gData_02001a20 > 2)
      gData_02001a20--;
  }
  if (!due)
    return;
  colour = Data_02001a2c;
  i = 0;
  if (i < gData_02001a30) {
    u16 *table;
    u16 *step;
    u16 *count;

    /* FAKEMATCH(one-pass): the block ends CSE's run, so count is copied from
     * the test's address here and not above the test; as plain statements,
     * 20 of 120 instructions differ in tla-en. */
    do {
      table = gData_02001a18;
      step = &gData_02001a34;
    } while (0);
    count = &gData_02001a30;
    do {
      slot = (u16 *)0x05000000 + table[*step * gData_02001a30 + i];
      r = *colour++;
      g = *colour++;
      b = *colour++;
      /* FAKEMATCH(one-pass): the colour's build and store in a once-only
       * block, so the third step stays above it and i++ below; as a plain
       * statement, 6 of 120 instructions differ in tla-en. */
      do {
        *slot = b << 10 | g << 5 | r;
      } while (0);
      i++;
    } while (i < *count);
  }
  /* FAKEMATCH(barrier): the memory clobber keeps GCSE from carrying the
   * address of gData_02001a30 past the loop, so it reloads here; as no asm, 21
   * of 120 instructions differ in tla-en. */
  asm("" ::: "memory");
  slot = (u16 *)0x05000000 + gData_02001a24[gData_02001a30];
  index = gData_02001a30 * 3;
  list = gData_02001a14 ? &gData_02001a28 : &Data_02001a2c;
  /* FAKEMATCH(one-pass): the shift in a once-only block, so it stays ahead of
   * the list's load; as a plain statement, 2 of 120 instructions differ in
   * tla-en. */
  do {
    index *= 2;
  } while (0);
  colour = (u16 *)((u8 *)*list + index);
  g = *colour++;
  r = *colour++;
  b = *colour;
  /* FAKEMATCH(one-pass): as in the loop; as a plain statement, 2 of 120
   * instructions differ in tla-en. */
  do {
    *slot = b << 10 | g << 5 | r;
  } while (0);
  gData_02001a14 ^= 1;
  /* FAKEMATCH(volatile): the volatile read issues after the left half of the
   * step's zero extension; as a plain read, 2 of 120 instructions differ in
   * tla-en. */
  if (++gData_02001a34 == gData_02001a30) {
    gData_02001a34 = 0;
    /* FAKEMATCH(barrier): the two uses and the longer life set the slot's
     * allocation priority between i's and r's, so it takes r4 and i r5; as no
     * asm, 18 of 120 instructions differ in tla-en. */
    asm("" : : "r"(slot), "r"(slot));
  }
}
