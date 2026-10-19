// Draft, differ 94 of 236: runs one step of the title sequence's vertical picture scroll. The
// position moves from pos toward limit by step, waiting frames between moves
// (1 when a key cuts the wait short, else 0 with pos updated). Scrolling
// down, each 8 lines of scroll DMA the next 30 map rows' worth of tiles
// (0x780 bytes) and number the next map row, wrapping the tiles to block 0
// and the row cursor to 0x0600f800 after 32 rows; scrolling up undoes a row
// when the counter runs out. Two countdowns in the heap slot at 0xac start
// a fade to white or back when they reach 1. Left: the shared return-1
// block, which the native keeps after the first loop and the draft after
// the second (so most branches differ); the step's second, u16 read at
// entry (here zero-extended in the second loop instead); and the rows
// countdown's test, which ands with 0xffff here.
#include "types.h"
#include "dma.h"
#include "bg_scroll.h"
#include "palbuf.h"
#include "ram_buffer.h"

struct TitleScroll {
  s16 pos;
  s16 limit;
  s16 step;
  u8 *src;
  u8 *dst;
  u16 *cursor;
  u16 tile;
  s16 scroll;
  u16 rows;
  u32 frames;
};

struct TitleFadeTimers {
  u8 unk00[10];
  s16 white;
  s16 back;
};

s32 Title_WaitFramesOrPress(u32 frames);

s32 Func_081a68b4(struct TitleScroll *s) {
  struct TitleFadeTimers *timers = Ram_HeapSlots->unk84[10];
  s32 i;
  s32 j;
  u16 step = s->step;

  if (s->step > 0) {
    for (i = s->pos; i < s->limit; i += s->step) {
      if (Title_WaitFramesOrPress(s->frames))
        return 1;
      if (s->scroll > 0) {
        Dma_Set(s->src, s->dst, 0x840001e0, REG_DMA3);
        s->src += 0x780;
        s->dst += 0x780;
        for (j = 29; j >= 0; j--)
          *s->cursor++ = s->tile++;
        *s->cursor++ = 0x1ff;
        *s->cursor++ = 0x1ff;
        if (++s->rows == 32) {
          s->dst = (u8 *)0x06000000;
          s->rows = 0;
          s->tile = 0;
          s->cursor = (u16 *)0x0600f800;
        }
        s->scroll -= 8;
      }
      s->scroll += s->step;
      gBgScroll[1].y += s->step;
      if (timers->white != 0 && --timers->white == 1) {
        PaletteFade_SetTarget(0x7fff, 0);
        PaletteFade_Start(10);
      }
      if (timers->back != 0 && --timers->back == 1) {
        PaletteFade_SetTarget(0x10000, 0);
        PaletteFade_Start(10);
      }
    }
  } else {
    for (i = s->pos; i > s->limit; i += (s16)step) {
      gBgScroll[1].y += step;
      s->scroll -= step;
      if (s->scroll > 8) {
        if ((s16)--s->rows == 0) {
          s->rows = 1;
          Dma_Set(s->src, s->dst, 0x840001e0, REG_DMA3);
          s->src -= 0x780;
          s->dst -= 0x780;
          for (j = 29; j >= 0; j--)
            *s->cursor++ = s->tile++;
          s->cursor -= 30;
          s->tile -= 30;
        }
        s->scroll -= 8;
      }
      if (Title_WaitFramesOrPress(s->frames))
        return 1;
      step = s->step;
    }
  }
  s->pos = i;
  return 0;
}
