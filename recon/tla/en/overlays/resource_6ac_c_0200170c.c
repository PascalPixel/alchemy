// Draft, differ 20 of 120 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Twin of overlay 6ae's Func_020050b0 and overlay 6aa's Func_020047b8, a
// palette cycle. The palette writes go through the volatile BG_PLTT: a volatile
// reference in the loop stops loop.c turning two of the colour pointer's three
// increments into givs, so all three stay, and the store keeps its address
// register, both as in the listing (a plain u16 pointer: differ 85).
// Left: the native reads the count's address through two registers (r6 in the
// body, a preheader copy in ip for the exit test) and rebuilds 0x5000000 in the
// body. Here cse merges the body's and the exit test's address into one pseudo,
// and the second loop pass hoists the constant into ip (T = 32, less 3 per move,
// against 24 insns). That missing `mov r2, ip` also moves i++ and the last
// src++, and leaves i in r4 and dst in r5, the native's the other way round.
// Tried: a do { } while (0) around the colours or the address splits the two
// pseudos and gives the exit test's mov, but the constant still hoists and r8
// and r10 get pushed (differ 101 at best; a one-pass block, so not kept). Goto
// and if + do/while loops hoist too little (differ 91: the table load stays in
// the body). Operand orders, a value or index local, red/green/blue types,
// block-scoped dst and colours, declaration order, and a shared offset before
// the source select gain nothing. Lead: 138's move fails only if one more
// movable moves before it in the second pass, or the count there passes 29.
#include "types.h"
#include "io_reg.h"

extern u16 gData_02003918;
extern u16 *Data_0200391c;
extern u16 gData_02003920;
extern u16 gData_02003924;
extern u16 *gData_02003928;
extern u16 *Data_0200392c;
extern u16 *Data_02003930;
extern u16 gData_02003934;
extern u16 gData_02003938;

void Func_0200170c(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02003920 != 0) {
    if (--gData_02003920 == 0) {
      step = 1;
      gData_02003920 = gData_02003924;
      if (gData_02003924 > 2)
        gData_02003924--;
    }
  }
  if (step == 0)
    return;
  src = Data_02003930;
  for (i = 0; i < gData_02003934; i++) {
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 28 of 120 instructions differ
     * in tla-en. */
    do {
      dst = &BG_PLTT[Data_0200391c[gData_02003938 * gData_02003934 + i]];
    } while (0);
    /* FAKEMATCH(barrier): the empty asm on dst changes its allocation rank; as no
     * asm, 95 of 120 instructions differ in tla-en. */
    asm("" : : "r"(dst));
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(barrier): the empty asm on blue changes its allocation rank; as no
     * asm, 30 of 120 instructions differ in tla-en. */
    asm("" : : "r"(blue));
    /* FAKEMATCH(barrier): the empty asm on blue changes its allocation rank; as no
     * asm, 32 of 120 instructions differ in tla-en. */
    asm("" : "=r"(blue) : "0"(blue));
    *dst = (blue << 10) | (green << 5) | red;
  }
  dst = &BG_PLTT[gData_02003928[gData_02003934]];
  if (gData_02003918 != 0)
    src = Data_0200392c + gData_02003934 * 3;
  else
    src = Data_02003930 + gData_02003934 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 22 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02003918 ^= 1;
  if (++gData_02003938 == gData_02003934)
    gData_02003938 = 0;
}
