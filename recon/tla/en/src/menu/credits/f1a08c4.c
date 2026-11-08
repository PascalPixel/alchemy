#include "types.h"
#include "dma.h"
#include "input.h"

struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

extern u16 gDisplayScroll_Position;
extern s16 gDisplayScroll_Busy;
extern struct DisplayScrollObject *gDisplayScroll_Objects;
extern s16 Data_02007500;
extern u32 gVBlankCount;

void Func_081a08c4(void) {
  u32 fine = gDisplayScroll_Position & 7;
  u32 tile = (((s16)gDisplayScroll_Position / 8) & 31) * 24;
  u32 *words = (u32 *)(gDisplayScroll_Objects + 24);
  s32 row;

  for (row = 0; row < 16; row++) {
    s32 column;

    for (column = 0; column < 6; column++) {
      *words++ =
          (row * 8 + 16 - fine) | ((column * 32 + 24) << 16) | 0x40004000;
      *words++ = tile;
      tile += 4;
      if (tile == 768)
        tile = 0;
    }
  }
  Dma_Set(gDisplayScroll_Objects, (void *)0x07000000, 0x84000100,
          (u32 *)SPLIT_CONSTANT(0x4000000, 212));

  if (gDisplayScroll_Busy == 1 && (gInput.held & 3)) {
    gDisplayScroll_Busy = 0;
    Data_02007500++;
  }
  if (gDisplayScroll_Busy == 3 && (gInput.held & 3))
    gDisplayScroll_Busy = 0;
  if (gDisplayScroll_Busy == 0) {
    if ((gVBlankCount & 3) == 0)
      gDisplayScroll_Position++;
    if ((gInput.held & 2) && (gVBlankCount & 3))
      gDisplayScroll_Position++;
  }
}
