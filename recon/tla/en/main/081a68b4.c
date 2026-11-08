// Differ 6 of 236 (tla-en, 2026-11-08, -mtune=gba): scheduling only, `movs r4, #29` one
// instruction out of place in both row copies. The second loop reads its step
// through a volatile cast (ordinary C under S2, noted at the read), which keeps the ldrh, its copy and
// the sign extension apart; plainly read, combine folds them into a second
// ldrsh (differ 35). Tried without effect on the step: a temporary, a block,
// an empty asm, an assignment in the increment, a do-while, s16 or u16 step
// locals, a volatile field (75). The row loop as j counting up is worse (18).
// Earlier notes follow.
// The step is an s32 local read from its u16 field: the test casts it to
// s16 where the listing has ldrsh.
// Runs one step of the intro's vertical picture scroll. The
// position moves from pos toward limit by step, waiting frames between moves
// (1 when a key cuts the wait short, else 0 with pos updated). Scrolling
// down, each 8 lines of scroll DMA the next 30 map rows' worth of tiles
// (0x780 bytes) and number the next map row, wrapping the tiles to block 0
// and the row cursor to 0x0600f800 after 32 rows; scrolling up undoes a row
// when the counter runs out. Two countdowns in the heap slot at 0xac start
// a fade to white or back when they reach 1.
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
          for (j = 29; j >= 0; j--)
            *s->cursor++ = s->tile++;
          s->cursor -= 30;
          s->tile -= 30;
        }
        s->scroll -= 8;
      }
      if (Title_WaitOrPress(s->frames))
        goto fail;
      // Read through a volatile cast: keeps the ldrh, its copy and the sign
      // extension apart (plainly read, 35 of 236 differ in tla-en).
      step = *(volatile u16 *)&s->step;
    }
  }
done:
  s->pos = i;
  return 0;
}
