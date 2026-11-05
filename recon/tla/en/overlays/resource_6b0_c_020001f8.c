// Draft, differ 29 of 120 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Overlay 6b0's Func_020001f8, a palette cycle, the twin of overlay 6ae's
// Func_020050b0. The palette store is volatile: strength_reduce leaves a biv's
// increments alone in a loop with a volatile reference, which keeps the
// colour pointer's three post-increments; the tail's *src++ gives the listing's
// last load, its dead increment folded.
// Left: the native builds 0x5000000 inside the loop. Here the first loop pass
// refuses to move it (29 insns, threshold 18 after four moves) and the second
// moves it (24 insns, threshold 30); the later register and scheduling
// differences, r12 as a second copy of &gData_02004450 among them, follow from
// that. The native behaves as if only one loop pass saw this loop. Tried with
// no effect on it: an index local, u32 address arithmetic, a shared palette
// local, while for for, inner-block locals, a u16 colour local or u16 r, g, b
// (either keeps extensions at the volatile store), volatile counters.
#include "types.h"
#include "call.h"

extern u16 gData_02004434;
extern u16 *Data_02004438;
extern u16 gData_0200443c;
extern u16 gData_02004440;
extern u16 *gData_02004444;
extern u16 *Data_02004448;
extern u16 *Data_0200444c;
extern u16 gData_02004450;
extern u16 gData_02004454;

void Func_020001f8(void) {
  s32 step;
  u32 i;
  u32 index;
  u16 *src;
  volatile u16 *dst;
  s32 r, g, b;

  step = 0;
  if (gData_0200443c != 0) {
    if (--gData_0200443c == 0) {
      step = 1;
      gData_0200443c = gData_02004440;
      if (gData_02004440 > 2)
        gData_02004440--;
    }
  }
  if (step == 0)
    return;
  src = Data_0200444c;
  for (i = 0; i < gData_02004450; i++) {
    dst = (volatile u16 *)0x5000000 +
          Data_02004438[gData_02004454 * gData_02004450 + i];
    r = *src++;
    g = *src++;
    b = *src++;
    /* FAKEMATCH(barrier): the empty asm on b changes its allocation rank; as no
     * asm, 33 of 120 instructions differ in tla-en. */
    asm("" : : "r"(b));
    *dst = (b << 10) | (g << 5) | r;
  }
  index = gData_02004444[gData_02004450];
  dst = (volatile u16 *)0x5000000 + index;
  src = (gData_02004434 != 0 ? Data_02004448 : Data_0200444c) +
        gData_02004450 * 3;
  r = *src++;
  g = *src++;
  /* FAKEMATCH(barrier): the empty asm on g changes its allocation rank; as no
   * asm, 42 of 120 instructions differ in tla-en. */
  asm("" : : "r"(g));
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 31 of 120 instructions differ
   * in tla-en. */
  do {
    b = *src++;
  } while (0);
  *dst = (b << 10) | (g << 5) | r;
  gData_02004434 ^= 1;
  if (++gData_02004454 == gData_02004450)
    gData_02004454 = 0;
}
