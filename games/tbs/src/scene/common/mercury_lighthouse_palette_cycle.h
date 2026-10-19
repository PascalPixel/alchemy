#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "frame.h"
#include "io_reg.h"

// Mercury Lighthouse and its entrance: the shimmer of the pool once it is
// woken (flag 0x875), in colours 9 to 15 of background palette 6.

// A scene callback: every fifth frame colour 9's red level steps on by 4, and
// each of colours 10 to 15 takes the red level of the colour below it (13 to
// 15 dimmed by four tenths), so the shimmer runs up the ramp; green and blue
// stay those of gPaletteCycleGreen and gPaletteCycleBlue.
static void MercuryLighthouse_CyclePalette(void) {
  /* FAKEMATCH(volatile): cur lives at sp+2, stored with strh and loaded again
   * with ldrh for v, as the bytes show; as a plain u16, 22 of 65 instructions
   * differ in tbs-en. */
  volatile u16 cur;
  u32 i;
  s32 v;
  s32 bits;
  u16 *dst;

  cur = 0;
  if (gFrameCount % 5 != 0)
    return;
  gPaletteCycleRed = (gPaletteCycleRed + 4) & 31;
  for (i = 0; i < 6; i++) {
    cur = BG_PLTT_COLOR(6, 14 - i) & 31;
    v = cur;
    if (i <= 2)
      v -= v * 4 / 10;
    dst = (u16 *)&BG_PLTT_COLOR(6, 15 - i);
    bits = (gPaletteCycleBlue << 10) | (gPaletteCycleGreen << 5);
    *dst = v | bits;
  }
  BG_PLTT_COLOR(6, 9) = gPaletteCycleRed | bits;
}

// While the pool sleeps: colours 9 to 15 of background palette 6 are black.
static void MercuryLighthouse_ClearPalette(void) {
  u32 i;

  for (i = 0; i < 7; i++)
    BG_PLTT_COLOR(6, 15 - i) = 0;
}
