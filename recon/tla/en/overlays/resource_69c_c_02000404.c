// Draft, differ 14 of 83: overlay 69c's copy of the function drafted in
// resource_6a4_c_0200079c.c (overlay 6a4's Func_0200079c), whose
// header says what it is, what still differs and what was tried.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002cc0;
extern s32 gData_02002cc4;
extern u16 gData_02002cec[];

void Func_02000404(void) {
  u16 colors[16];
  s32 phase = (gData_02002cc0 / 4) % 5;
  s32 i;

  if (gData_02002cc4 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  for (i = 0; i <= 4; i++)
    colors[(phase + i) % 5] = gData_02002cec[i + 2];
  Dma_Set(colors, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002cc0++;
}
