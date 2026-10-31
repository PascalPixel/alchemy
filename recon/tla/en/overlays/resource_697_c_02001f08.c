// Near miss, differ 6 of 85 (tla-en, 2026-10-31): a palette cycle (overlay
// 6a4's Func_0200079c, its draft's header says what differs), here rotating
// five colours into palette offset 0xa4, one step every fourth call, unless an
// event holds it. Left, as there: the native sets r7 = sp inside the loop,
// where gcse's PRE hoists the colours' address to before it (its gcse dump:
// "PRE/HOIST: edge (7,8)" when the address is formed only in the loop), and
// the table word loads as [table, offset] where expand_binop puts the
// register offset first. Also tried here: p set inside the loop, with or
// without colors throughout: 14; a u8 *table local: 28 (the table takes sl);
// for and do-while loops: 12 to 34.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02007e20;
extern s32 gData_02007e24;
extern u16 gData_02007e28[];

void Func_02001f08(void) {
  u16 colors[16];
  s32 phase = (gData_02007e20 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02007e24 != 0) {
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
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02007e28 + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x050000a4, 0x84000002, REG_DMA3);
  gData_02007e20++;
}
