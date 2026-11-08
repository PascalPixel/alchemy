#include "../../../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002194;
extern s32 gData_02002198;
extern u16 gData_020021c0[];

void Func_02000238(void) {
  u16 colors[16];
  s32 phase = (gData_02002194 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002198 != 0) {
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
    u8 *table = (u8 *)gData_020021c0;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, SPLIT_CONSTANT(0x84000002), SPLIT_CONSTANT(REG_DMA3));
  gData_02002194++;
}
