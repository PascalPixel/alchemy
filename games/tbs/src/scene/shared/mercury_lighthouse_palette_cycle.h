#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "frame.h"
#include "io_reg.h"

// Mercury Lighthouse and its entrance: the shimmer of the pool once it is
// woken (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), in colours 9 to 15 of
// background palette 6.

/// A task: every fifth frame colour 9's red level steps on by 4, and
/// each of colours 10 to 15 takes the red level of the colour below it (13 to
/// 15 dimmed by four tenths), so the shimmer runs up the ramp; green and blue
/// stay those of gPaletteCycleGreen and gPaletteCycleBlue.
static void MercuryLighthouse_CyclePalette(void) {
  /* FAKEMATCH(volatile): the red level below passes through a halfword on
   * the stack; as a plain local, 22 of 65 instructions differ in tbs-en. */
  volatile u16 below;
  u32 i;
  s32 red;
  s32 greenAndBlue;
  volatile u16 *color;

  below = 0;
  if (gFrameCount % 5 != 0)
    return;
  gPaletteCycleRed = (gPaletteCycleRed + 4) & 31;
  for (i = 0; i < 6; i++) {
    below = BG_PLTT_COLOR(6, 14 - i) & 31;
    red = below;
    if (i <= 2)
      red -= red * 4 / 10;
    color = &BG_PLTT_COLOR(6, 15 - i);
    greenAndBlue = (gPaletteCycleBlue << 10) | (gPaletteCycleGreen << 5);
    *color = red | greenAndBlue;
  }
  BG_PLTT_COLOR(6, 9) = gPaletteCycleRed | greenAndBlue;
}

/// While the pool sleeps: colours 9 to 15 of background palette 6 are black.
static void MercuryLighthouse_ClearPalette(void) {
  u32 i;

  for (i = 0; i < 7; i++)
    BG_PLTT_COLOR(6, 15 - i) = 0;
}
