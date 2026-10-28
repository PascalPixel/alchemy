#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "resource.h"
#include "ram_buffer.h"
#include "system.h"
#include "input.h"
#include "frame.h"
#include "bg_scroll.h"
#include "edition.h"

extern char ResourceId_Data018;
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define DATA018_TILE_BYTES 0xa000
#else
#define DATA018_TILE_BYTES 0x9c00
#endif

/// Blanks the display, loads resource 0x18's palette into the BG palette with
/// colour 0 black, and unpacks its tiles through the map cell buffer into BG
/// character block 0.
void Func_081a6030(void) {
  u8 *source;
  u8 *cells;

  REG_DISPCNT = 0;
  source = Resource_GetTableEntry((s32)&ResourceId_Data018);
  Dma_Set(source, (void *)BG_PLTT, DMA_COPY_WORDS(0x200 / 4), REG_DMA3);
  BG_PLTT[0] = 0;
  /* FAKEMATCH: a one-pass block loads the cell buffer's address before 0x200 is
   * built for the source offset; as an argument the constant comes first
   * (differ 2). */
  do {
    cells = Ram_MapCellBuffer;
  } while (0);
  Resource_DecodeByteLzInRam(source + 0x200, cells);
  Dma_Set(cells, (void *)0x06000000, DMA_COPY_WORDS(DATA018_TILE_BYTES / 4),
          REG_DMA3);
}

s32 Title_WaitOrPress(u32 frames) {
  u32 i;

  for (i = 0; i < frames; i++) {
    wait(1);
    Random16();
    if (gInput.pressed != 0)
      return 1;
  }
  return 0;
}

struct TitleScrollStep {
  s32 frames;
  s32 speed[4];
  u32 mask[4];
};

s32 Title_RunScrollStep(struct TitleScrollStep *step) {
  s32 i;

  for (i = 0; i < step->frames; i++) {
    if (step->mask[0] == 0 || (gFrameCount & step->mask[0]) == 0)
      gBgScroll[1].y += step->speed[0];
    if (step->mask[1] == 0 || (gFrameCount & step->mask[1]) == 0)
      gBgScroll[1].x += step->speed[1];
    if (step->mask[2] == 0 || (gFrameCount & step->mask[2]) == 0)
      gBgScroll[2].y += step->speed[2];
    if (step->mask[3] == 0 || (gFrameCount & step->mask[3]) == 0)
      gBgScroll[2].x += step->speed[3];
    if (Title_WaitOrPress(1))
      return 0;
  }
  return 1;
}
