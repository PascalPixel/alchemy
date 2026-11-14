/// Steps a palette cycle that fades as it slows.
void PaletteCycle_Fade(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gPaletteCycleTimer != 0) {
    if (--gPaletteCycleTimer == 0) {
      step = 1;
      gPaletteCycleTimer = gPaletteCycleDelay;
      if (gPaletteCycleDelay > 2)
        gPaletteCycleDelay--;
    }
  }
  if (step == 0)
    return;
  src = gPaletteCycleColors;
  for (i = 0; i < gPaletteCycleLength; i++) {
    dst = &BG_PLTT[gPaletteCycleSlots[gPaletteCycleIndex * gPaletteCycleLength +
                                      i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gPaletteSlots[gPaletteCycleLength]];
  if (gPaletteCycleBright != 0)
    src = gPaletteColors + gPaletteCycleLength * 3;
  else
    src = gPaletteCycleColors + gPaletteCycleLength * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gPaletteCycleBright ^= 1;
  if (++gPaletteCycleIndex == gPaletteCycleLength)
    gPaletteCycleIndex = 0;
}
