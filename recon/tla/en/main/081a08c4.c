// Draft, differ 12 of 131: the credits' per-frame update. It lays the 16 rows
// of six text sprites at the scroll's fine offset, copies the sprite table to
// OAM by DMA, ends a page pause on A or B, and advances the scroll every
// fourth frame, every frame while B is held. Left: instruction order only:
// the native builds the wrap limit 768 after 16 - fine in the preheader, and
// steps the object pointer between the two stores where the draft steps it
// at the loop's end (also with object++ written between the stores).
// Tried: a y variable stepped by 8 per row (differ 62).
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
extern u32 gFrameTick;

void Func_081a08c4(void) {
  struct DisplayScrollObject *object;
  u32 fine;
  u32 tile;
  u32 x;
  s32 row;
  s32 column;

  fine = gDisplayScroll_Position & 7;
  tile = (((s16)gDisplayScroll_Position / 8) & 31) * 24;
  object = gDisplayScroll_Objects + 24;
  for (row = 0; row < 16; row++) {
    x = 24 << 16;
    for (column = 0; column < 6; column++) {
      u32 *q = (u32 *)object;

      *q++ = (row * 8 + 16 - fine) | x | 0x40004000;
      *q = tile;
      tile += 4;
      if (tile == 768)
        tile = 0;
      object++;
      x += 32 << 16;
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
    if ((gFrameTick & 3) == 0)
      gDisplayScroll_Position++;
    if ((gInput.held & 2) && (gFrameTick & 3))
      gDisplayScroll_Position++;
  }
}
