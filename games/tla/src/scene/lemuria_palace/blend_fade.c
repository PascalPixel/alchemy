#include "types.h"
#include "io_reg.h"

extern u8 gData_0200308c;

void wait(s32 frames);

static __inline__ void Blend_WriteAlpha(s32 value) {
  // The s32 parameter keeps each constant a word built with shifts; a u16 store
  // loads it from the literal pool.
  REG_BLDALPHA = value;
}

/* FAKEMATCH: each alpha write is a one-pass block; with the plain inline the
 * fades differ 4 and 2. */
#define BLEND_SET_ALPHA(value)                                                 \
  do {                                                                         \
    Blend_WriteAlpha(value);                                                   \
  } while (0)

/// Fades the second target out and the first in over 16 steps of four frames.
void Func_02000510(void) {
  s32 i;

  /* FAKEMATCH: the alpha writes are one-pass blocks (BLEND_SET_ALPHA); a for,
   * do-while or while (1) loop differs 39, as the loop pass hoists the register
   * address and 16 out of it. */
  BLEND_SET_ALPHA(0x1000);
  gData_0200308c = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA(((16 - i) << 8) | i);
  wait(4);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(16);
  gData_0200308c = 1;
}

/// The reverse fade, in steps of three frames.
void Func_02000550(void) {
  s32 i;

  /* FAKEMATCH: the alpha writes are one-pass blocks (BLEND_SET_ALPHA), as in
   * Func_02000510. */
  BLEND_SET_ALPHA(16);
  gData_0200308c = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA((i << 8) | (16 - i));
  wait(3);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(0x1000);
}
