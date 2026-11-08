#include "types.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

struct ScrollStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
};

void Func_0813baec(void) {
  struct ScrollStepWork *work;

  work = (struct ScrollStepWork *)Ram_HeapSlots->battleFxWork[0];

  if (++work->counter == work->period) {
    gBgScroll[1].x += work->stepX;
    gBgScroll[1].y += work->stepY;
    work->counter = 0;
  }
}
