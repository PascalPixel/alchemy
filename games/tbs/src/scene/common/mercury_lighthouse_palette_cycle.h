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

static void MercuryLighthouse_CyclePalette(void) {
  /* FAKEMATCH: volatile restores the form the bytes prove: cur lives at sp+2,
   * stored with strh and loaded again with ldrh for v (a 4-byte frame and r7
   * for none). Without it differ 38, 55 instructions for 65. */
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

static void MercuryLighthouse_ClearPalette(void) {
  u32 i;

  for (i = 0; i < 7; i++)
    BG_PLTT_COLOR(6, 15 - i) = 0;
}
