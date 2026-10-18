// Draft, differ 44 of 178: starts the scrolling credits at a page. It counts
// the line table's 255 page marks up to the page asked for (at most 500
// lines, stopping at the end mark 0), sets the scroll there, allocates and
// fills the sprite table and its tiles as Func_081a0a70 does, installs the
// two scroll callbacks and draws the first 32 lines. Left: register choice
// and order. The native forms the fill word's address once in r4 before the
// first store (here each fill stores through sp and r4 comes later), and in
// the text-sprite loop it keeps the row's 2i in r7, the y in lr and
// 0x40004000 in r10 where the draft rotates them (r7 = y, lr = i). The line
// count shares i with the loops, as the native's r6 does: with a separate
// line variable, differ 66. Tried: the tile as i * 24 or (i + i * 2) * 8
// (differ 44 each); Dma_Fill32's own words for the two fills (differ 69).
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
extern const u8 *gDisplayScroll_LineTable[];

void Func_081a0cac(void);
void Func_081a0d78(void);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

void Func_081a0e0c(s32 page) {
  u32 fill;
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

  gDisplayScroll_Objects = Runtime_BumpAllocateAlternatePool(
      128 * sizeof(struct DisplayScrollObject));
  Dma_Fill32Cell(fill, 0, (void *)0x06010000, 0x6000);
  Dma_Fill32Cell(fill, 0x11111111, (void *)0x06016000, 0x100);

  entry = (u32 *)gDisplayScroll_Objects;
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = (i << 21) | 0x80004000;
    *q = 0x300;
    entry += 2;
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = (i << 21) | 0x80004088;
    *q = 0x300;
    entry += 2;
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = (i << 21) | 0x40004098;
    *q = 0x300;
    entry += 2;
  }
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 6; j++) {
      u32 *q = entry;

      *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
      *q = (i * 3) * 8 + j * 4;
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
