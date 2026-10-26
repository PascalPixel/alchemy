#include "types.h"
#include "dma.h"

struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

extern u16 gDisplayScroll_Position;
extern s16 gDisplayScroll_Busy;
extern s16 gDisplayScroll_LastPosition;
extern struct DisplayScrollObject *gDisplayScroll_Objects;
extern u32 gFrameTick;
extern const void *gDisplayScroll_LineTable[];

s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

void Func_081a0cac(void) {
  u32 frame = gDisplayScroll_Position;
  u32 fine = frame & 7;
  s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
  u32 *entry = (u32 *)(gDisplayScroll_Objects + 24);
  s32 row;

  for (row = 0; row <= 15; row++) {
    s32 col;

    for (col = 0; col < 6; col++) {
      u32 *q = entry;

      *q++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
      entry += 2;
      /* FAKEMATCH: a one-pass block keeps the entry step ahead of the tile
       * store; as plain statements, in either order, the step falls behind the
       * tile step (2 differ). */
      do {
      } while (0);
      *q = tile;
      tile += 4;
      if (tile == 0x300)
        tile = 0;
    }
  }
  Dma_Set(gDisplayScroll_Objects, (void *)0x07000000, 0x84000100,
          (volatile u32 *)0x040000d4);
  if (gFrameTick % 3 == 0)
    gDisplayScroll_Position++;
}

/// A scheduler callback added by Func_081a0e0c: draws each line entering the
/// credits sixteen rows ahead of the visible top. A line table value of 255
/// ends the text, and blank rows follow it.
void Func_081a0d78(void) {
  s32 position;
  u32 current;
  s32 line;
  const u8 *text;

  position = (s16)gDisplayScroll_Position;
  current = gDisplayScroll_Position;
  line = position / 8;
  if (gDisplayScroll_Busy != 0) {
    gDisplayScroll_Busy++;
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0],
                           ((position / 8 + 16) & 31) * 24, 1);
  } else if (position / 8 != gDisplayScroll_LastPosition / 8) {
    gDisplayScroll_LastPosition = current;
    text = gDisplayScroll_LineTable[line];
    if (text == (const u8 *)255) {
      gDisplayScroll_Busy = 1;
      DisplayScroll_DrawLine(gDisplayScroll_LineTable[0],
                             ((line + 16) & 31) * 24, 1);
    } else {
      gDisplayScroll_Busy =
          DisplayScroll_DrawLine(text, ((line + 16) & 31) * 24, 1);
    }
  }
}
