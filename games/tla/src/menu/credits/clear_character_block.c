/// @file
/// Tiles: clear half of the BG character and palette storage.

#include "dma.h"
#include "io_reg.h"
#include "types.h"

/// Fills 30K bytes of 256-colour characters with colour 1 and clears 128
/// palette colours.
/// alt: 0 selects the first half (VRAM 0x06000000, palette colours 0-127);
///      1 selects the second (VRAM 0x06008000, colours 128-255, fill colour
///      129).
void Graphics_ClearCharacterBlockAndPalette(s32 alt) {
  u32 v;
  u32 vram;
  u32 pal;
  volatile u32 fill;

  if (alt == 0) {
    v = 0x01010101;
    vram = 0x06000000;
    pal = 0x05000000;
  } else {
    v = 0x81818181;
    vram = 0x06008000;
    pal = 0x05000100;
  }
  fill = v;
  Dma_Set((const void *)&fill, (void *)vram,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 7680, REG_DMA3);
  fill = 0;
  Dma_Set((const void *)&fill, (void *)pal,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 64, REG_DMA3);
}
