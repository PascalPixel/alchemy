/// @file
/// Mercury Lighthouse and its entrance: the shimmer of the pool once it is
/// woken (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED).

/// The background palette, and its colours, the shimmer runs through.
#define SHIMMER_PALETTE 6
#define SHIMMER_FIRST_COLOR 9
#define SHIMMER_LAST_COLOR 15
/// The top colours, which take the red level below them dimmed.
#define SHIMMER_DIMMED_COLORS 3

/// A task: every fifth frame SHIMMER_FIRST_COLOR's red level steps on by 4,
/// and each colour above it takes the red level of the colour below it (the
/// top SHIMMER_DIMMED_COLORS dimmed by four tenths), so the shimmer runs up the
/// ramp; green and blue stay those of gPaletteCycleGreen and
/// gPaletteCycleBlue.
static void MercuryLighthouse_CyclePalette(void) {
  u16 below;
  u32 i;
  s32 red;
  s32 greenAndBlue;
  volatile u16 *color;

  /* FAKEMATCH(dead): below cleared through its own address, before anything
     reads it; as below = 0. */
  *&below = 0;
  if (gFrameCount % 5 != 0)
    return;
  gPaletteCycleRed = (gPaletteCycleRed + 4) & RGB_LEVEL_MAX;
  for (i = 0; i < SHIMMER_LAST_COLOR - SHIMMER_FIRST_COLOR; i++) {
    below = RGB_RED(BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_LAST_COLOR - 1 - i));
    red = below;
    if (i < SHIMMER_DIMMED_COLORS)
      red -= red * 4 / 10;
    color = &BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_LAST_COLOR - i);
    greenAndBlue = RGB(0, gPaletteCycleGreen, gPaletteCycleBlue);
    *color = red | greenAndBlue;
  }
  BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_FIRST_COLOR) =
      gPaletteCycleRed | greenAndBlue;
}

/// While the pool sleeps: the shimmer's colours are black.
static void MercuryLighthouse_ClearPalette(void) {
  u32 i;

  for (i = 0; i <= SHIMMER_LAST_COLOR - SHIMMER_FIRST_COLOR; i++)
    BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_LAST_COLOR - i) = 0;
}
