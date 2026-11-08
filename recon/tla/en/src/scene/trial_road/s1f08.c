#include "../../../../../../games/tla/src/scene/trial_road/trial_road.h"
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
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02007e28;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)SPLIT_CONSTANT(0x50000a4), SPLIT_CONSTANT(0x84000002), SPLIT_CONSTANT(REG_DMA3));
  gData_02007e20++;
}
