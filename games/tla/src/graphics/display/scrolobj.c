#include "types.h"
#include "mem.h"
#include "dma.h"
#include "callback_scheduler.h"

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

/// Starts the scrolling credits at a page, as Func_081a0a70 starts them at the
/// top: counts the line table's 255 page marks up to the page asked for (at
/// most 500 lines, stopping at the end mark 0) and sets the scroll there, then
/// builds the sprite table and its tiles, adds Func_081a0cac and Func_081a0d78
/// to the scheduler and draws the first 32 lines blank.
void Func_081a0e0c(s32 page) {
  /* FAKEMATCH(volatile): the DMA fill word volatile; as a plain u32, 6 of 178
   * instructions differ in tla-en. */
  volatile u32 fill;
  s32 marks;
  u32 *entry;
  u32 i;
  u32 j;

  marks = 0;
  i = 0;
  if (page != 0 && gDisplayScroll_LineTable[0] != 0) {
    do {
      if ((u32)gDisplayScroll_LineTable[i] == 255)
        marks++;
      i++;
      if (i == 500 || marks == page)
        break;
    } while (gDisplayScroll_LineTable[i] != 0);
  }
  gDisplayScroll_Position = i * 8;

  {
    void *objects = Runtime_BumpAllocateAlternatePool(
        128 * sizeof(struct DisplayScrollObject));
    void **cell = (void **)&gDisplayScroll_Objects;

    *cell = objects;
    fill = 0;
    Dma_Set((void *)&fill, (void *)0x06010000, 0x85001800,
            (volatile u32 *)0x040000d4);
    fill = 0x11111111;
    Dma_Set((void *)&fill, (void *)0x06016000, 0x85000040,
            (volatile u32 *)0x040000d4);
    entry = *(u32 **)cell;
  }
  {
    u32 attr = 0x80004000;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x80004088;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x40004098;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 6; j++) {
      u32 *q = entry;
      u32 tile = (i * 3) * 8 + j * 4;

      *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
      *q = tile;
      entry += 2;
    }
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = 0x00c000c0;
    *q = 0x300;
    entry += 2;
  }
  gDisplayScroll_LastPosition = 0;
  gDisplayScroll_Busy = 0;
  Scheduler_AddOrUpdateCallback((s32)Func_081a0cac, 0xc80);
  Scheduler_AddOrUpdateCallback((s32)Func_081a0d78, 0x480);
  for (i = 0; i < 32; i++)
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
}
