// Near miss: differ 6 of 83. A palette cycle: every fourth call rotates five
// colours of the table into palette offset 0x184, unless an event holds it.
// The table is read by a byte offset in a goto loop (as a for loop, loop.c
// hoists the table's address and strength reduction folds it into the
// offset: 14), and the colours' address goes through p, set after the event
// checks, so it takes r7 as the native's does. Left: the native builds that
// address (mov r7, sp) inside the loop, before the __modsi3 call, where
// gcse's PRE hoists it out of the loop here (as p = colors inside the loop,
// too), and it loads the table word as [table, offset] where this has
// [offset, table], however the sum is written. Tried: register and volatile
// views of p and the colours, the counters set before the event checks;
// permute 12 minutes found nothing under 6.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

void Func_0200079c(void) {
  u16 colors[16];
  s32 phase = (gData_02003314 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02003318 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  p = colors;
  i = 0;
  offset = 4;
next:
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02003340 + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02003314++;
}
