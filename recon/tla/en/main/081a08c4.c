// Draft, differ 2 of 131, skeleton gap 1 (tla-en, 2026-11-01)
// Twin of The Broken Seal's Credits_UpdateTextSprites (menu/credits.c): the
// credits' per-frame update, with the same 16 rows of six text sprites over
// the same 32-row tile ring, DMA to OAM and fourth-frame scroll; only the end
// differs: here A or B ends a page pause, and B held scrolls every frame. The
// twin's loop forms took the draft from differ 12 to 2.
// Left: one scheduling pair: the native steps the pointer (adds #8) between
// the two stores; here it lands after the tile step. Tried without gain:
// words[0]/[1] with a late step (differ 89), the step between the stores
// through a second pointer or object++, object->attr01/attr2 with object++ in
// either store, a value temporary, the tile step and wrap respelled, a u32
// column (18), words declared first (38).
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
          (volatile u32 *)0x040000d4);

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
