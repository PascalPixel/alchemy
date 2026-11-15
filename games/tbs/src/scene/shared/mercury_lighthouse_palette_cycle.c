
// Mercury Lighthouse and its entrance: the shimmer of the pool once it is
// woken (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), in colours 9 to 15 of
// background palette 6.

/// A task: every fifth frame colour 9's red level steps on by 4, and
/// each of colours 10 to 15 takes the red level of the colour below it (13 to
/// 15 dimmed by four tenths), so the shimmer runs up the ramp; green and blue
/// stay those of gPaletteCycleGreen and gPaletteCycleBlue.
static void MercuryLighthouse_CyclePalette(void) {
  /* FAKEMATCH(volatile): the red level below passes through a halfword on the
   * stack. */
  volatile u16 below;
  u32 i;
  s32 red;
  s32 greenAndBlue;
  volatile u16 *color;

  below = 0;
  if (gFrameCount % 5 != 0)
    return;
  gPaletteCycleRed = (gPaletteCycleRed + 4) & RGB_LEVEL_MAX;
  for (i = 0; i < 6; i++) {
    below = RGB_RED(BG_PLTT_COLOR(6, 14 - i));
    red = below;
    if (i <= 2)
      red -= red * 4 / 10;
    color = &BG_PLTT_COLOR(6, 15 - i);
    greenAndBlue = RGB(0, gPaletteCycleGreen, gPaletteCycleBlue);
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
