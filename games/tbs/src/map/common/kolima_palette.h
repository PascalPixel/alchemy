#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "script_layer.h"
#include "io_reg.h"
#include "battle_effect_runtime.h"

// Kolima Forest, while Tret's curse lasts (FLAG_KOLIMA_CURSE_LIFTED clear):
// the forest's colours turned redder and darker, and both sets of colours kept
// for the scene where the curse lifts.

// Forward declarations.
static u16 KolimaPalette_AdjustColorChannels(u16 color, s32 adj);
static void KolimaPalette_SaveFirst(void);
static void KolimaPalette_SaveSecond(void);
static void KolimaPalette_Capture(void);

/// Keeps the healthy colours, curses the background palettes 0 to 13 (all but
/// colours 1 to 7 of palette 1 and 1 to 8 of palette 12) by strength, keeps
/// the cursed colours too, and shows them.
void KolimaPalette_Curse(s32 strength) {
  u16 i;

  KolimaPalette_SaveFirst();
  for (i = 0; i < 224; i++) {
    if ((i < 17 || i > 23) && (i < 193 || i > 200)) {
      u16 *pal = (u16 *)(0x5000000 + i * 2);
      *pal = KolimaPalette_AdjustColorChannels(*pal, strength);
    }
  }
  KolimaPalette_Capture();
  KolimaPalette_SaveSecond();
  Engine_ColorBufferApplyTarget(0x10000, 0);
}

/// One cursed colour: red gains a 4 * adj'th of itself (up to 31), and green
/// and blue lose an adj'th.
static u16 KolimaPalette_AdjustColorChannels(u16 color, s32 adj) {
  s16 green = ((color >> 5) & 31);
  s16 red = (color & 31);
  s16 blue = ((color >> 10) & 31);
  u32 packed;

  red = (red + Math_Divide(red, (adj << 2)));
  green = (green - Math_Divide(green, adj));
  blue = (blue - Math_Divide(blue, adj));

  if (red > 31)
    red = 31;

  packed = red;
  packed |= (blue << 10) | (green << 5);
  return packed;
}

/// Keeps the healthy colours: the palette fade's colours into
/// gKolimaPalette_First.
static void KolimaPalette_SaveFirst(void) {
  Dma_Set(gPaletteFadeWork->palette, gKolimaPalette_First,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0xe0, REG_DMA3);
}

/// Keeps the cursed colours: the palette fade's colours into
/// gKolimaPalette_Second.
static void KolimaPalette_SaveSecond(void) {
  Dma_Set(gPaletteFadeWork->palette, gKolimaPalette_Second,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0xe0, REG_DMA3);
}

/// Copies the palettes on the screen, background and sprites, into the palette
/// fade's buffer as the colours to fade from.
static void KolimaPalette_Capture(void) {
  u16 *buf = gPaletteFadeWork->palette;

  Dma_Set((const void *)BG_PLTT, buf, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, buf + 0xe0,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  Engine_ColorBufferApplySource(0x10000, 0);
}

/// Makes the healthy colours (second 0) or the cursed ones (second 1) the
/// colours to fade to; the scene then fades with Engine_ColorBufferInterpolate.
void KolimaPalette_Restore(s32 second) {
  u16 *buf = gPaletteFadeWork->palette;

  if (second)
    Dma_Set(gKolimaPalette_Second, buf, (DMA_ENABLE | DMA_32BIT) << 16 | 0xe0,
            REG_DMA3);
  else
    Dma_Set(gKolimaPalette_First, buf, (DMA_ENABLE | DMA_32BIT) << 16 | 0xe0,
            REG_DMA3);
  Engine_ColorBufferApplyTarget(0x10000, 0);
  KolimaPalette_Capture();
}
