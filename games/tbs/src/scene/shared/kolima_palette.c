#include "system/dma.h"

// Kolima Forest, while Tret's curse lasts (FLAG_KOLIMA_CURSE_LIFTED clear):
// the forest's colours turned redder and darker, and both sets of colours kept
// for the event where the curse lifts.

/// Background colour `color` of palette `palette`, counted from colour 0.
#define BG_COLOR_INDEX(palette, color) ((palette) * PALETTE_COLORS + (color))

/// The size of a kept set of colours: the palette fade's background and sprite
/// colours.
#define KOLIMA_PALETTE_WORDS (2 * PALETTE_FADE_BG_COLORS * sizeof(u16) / 4)

/// The healthy colours, kept by KolimaPalette_SaveHealthy; each scene defines
/// the 1792 bytes.
extern u8 gKolimaPalette_Healthy[];
/// The cursed colours, kept by KolimaPalette_SaveCursed.
extern u8 gKolimaPalette_Cursed[];

static u16 KolimaPalette_CurseColor(u16 color, s32 divisor);
static void KolimaPalette_SaveHealthy(void);
static void KolimaPalette_SaveCursed(void);
static void KolimaPalette_Capture(void);

/// Keeps the healthy colours, curses the background palettes 0 to 13 (all but
/// colours 1 to 7 of palette 1 and 1 to 8 of palette 12) by divisor, keeps
/// the cursed colours too, and shows them.
void KolimaPalette_Curse(s32 divisor) {
  u16 i;

  KolimaPalette_SaveHealthy();
  for (i = 0; i < PALETTE_FADE_BG_COLORS; i++) {
    if ((i < BG_COLOR_INDEX(1, 1) || i > BG_COLOR_INDEX(1, 7)) &&
        (i < BG_COLOR_INDEX(12, 1) || i > BG_COLOR_INDEX(12, 8))) {
      u16 *pal = (u16 *)&BG_PLTT[i];
      *pal = KolimaPalette_CurseColor(*pal, divisor);
    }
  }
  KolimaPalette_Capture();
  KolimaPalette_SaveCursed();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
}

/// One cursed colour: red gains 1 / (4 * divisor) of itself, up to
/// RGB_LEVEL_MAX, and green and blue lose 1 / divisor.
static u16 KolimaPalette_CurseColor(u16 color, s32 divisor) {
  s16 green = RGB_GREEN(color);
  s16 red = RGB_RED(color);
  s16 blue = RGB_BLUE(color);
  u32 packed;

  red = (red + Math_Divide(red, (divisor << 2)));
  green = (green - Math_Divide(green, divisor));
  blue = (blue - Math_Divide(blue, divisor));

  if (red > RGB_LEVEL_MAX)
    red = RGB_LEVEL_MAX;

  packed = red | ((blue << BLUE_SHIFT) | (green << GREEN_SHIFT));
  return packed;
}

/// Keeps the healthy colours: the palette fade's colours into
/// gKolimaPalette_Healthy.
static void KolimaPalette_SaveHealthy(void) {
  Dma_Set(gPaletteFadeWork->palette, gKolimaPalette_Healthy,
          DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS), REG_DMA3);
}

/// Keeps the cursed colours: the palette fade's colours into
/// gKolimaPalette_Cursed.
static void KolimaPalette_SaveCursed(void) {
  Dma_Set(gPaletteFadeWork->palette, gKolimaPalette_Cursed,
          DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS), REG_DMA3);
}

/// Copies the palettes on the screen, background and sprites, into the palette
/// fade's buffer as the colours to fade from.
static void KolimaPalette_Capture(void) {
  u16 *buf = gPaletteFadeWork->palette;

  Dma_Set((const void *)BG_PLTT, buf,
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * sizeof(u16) / 4), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, buf + PALETTE_FADE_BG_COLORS,
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * sizeof(u16) / 4), REG_DMA3);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
}

/// Makes the healthy colours (second 0) or the cursed ones (second 1) the
/// colours to fade to; the event then fades with
/// PaletteFade_StartInterpolation.
void KolimaPalette_Restore(s32 second) {
  u16 *buf = gPaletteFadeWork->palette;

  if (second)
    Dma_Set(gKolimaPalette_Cursed, buf, DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS),
            REG_DMA3);
  else
    Dma_Set(gKolimaPalette_Healthy, buf, DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS),
            REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  KolimaPalette_Capture();
}

#undef BG_COLOR_INDEX
#undef KOLIMA_PALETTE_WORDS
