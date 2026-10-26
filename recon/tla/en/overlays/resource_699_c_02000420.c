// Draft, differ 6 of 83: overlay 699's copy of the function drafted in
// resource_6a4_c_0200079c.c (overlay 6a4's Func_0200079c), whose
// header says what it is, what still differs and what was tried.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002384;
extern s32 gData_02002388;
extern u16 gData_020023b0[];

void Func_02000420(void) {
  u16 colors[16];
  s32 phase = (gData_02002384 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002388 != 0) {
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
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_020023b0 + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002384++;
}
