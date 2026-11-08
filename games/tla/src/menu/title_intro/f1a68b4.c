/// @file
/// Scrolls the intro's tall picture from one position to another: each step
/// moves it by the scroll's step and waits its frames, streams in the next map
/// rows' tiles when scrolling down and takes them back when scrolling up, and
/// starts the fade to white or back when its countdown reaches 1. A key press
/// cuts the scroll short.

#include "types.h"
#include "dma.h"
#include "bg_scroll.h"
#include "palette_buffers.h"
#include "ram_buffer.h"

struct TitleScroll {
  s16 pos;
  s16 limit;
  u16 step;
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

s32 Title_WaitOrPress(u32 frames);

s32 Func_081a68b4(struct TitleScroll *s) {
  struct TitleFadeTimers *timers = Ram_HeapSlots->unk84[10];
  s32 i;
  s32 j;
  s32 step;

  step = s->step;
  if ((s16)step > 0) {
    for (i = s->pos; i < s->limit; i += (s16)s->step) {
      if (Title_WaitOrPress(s->frames))
        goto fail;
      if (s->scroll > 0) {
        Dma_Set(s->src, s->dst, 0x840001e0, REG_DMA3);
        s->src += 0x780;
        s->dst += 0x780;
        for (j = 0; j < 30; j++)
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
    goto done;
  fail:
    return 1;
  } else {
    for (i = s->pos; i > s->limit; i += (s16)step) {
      gBgScroll[1].y += step;
      s->scroll -= step;
      if (s->scroll > 8) {
        s->rows--;
        if (s->rows == 0) {
          s->rows = 1;
          Dma_Set(s->src, s->dst, 0x840001e0, REG_DMA3);
          s->src -= 0x780;
          s->dst -= 0x780;
          for (j = 0; j < 30; j++)
            *s->cursor++ = s->tile++;
          s->cursor -= 30;
          s->tile -= 30;
        }
        s->scroll -= 8;
      }
      if (Title_WaitOrPress(s->frames))
        goto fail;
      // Read through a volatile cast: keeps the ldrh, its copy and the sign
      // extension apart, where a plain read folds them into one ldrsh.
      step = *(volatile u16 *)&s->step;
    }
  }
done:
  s->pos = i;
  return 0;
}
