// Draft of overlay 6aa's Func_020045f8, the twin of overlay 662's
// Func_020007dc: its draft (resource_662_c_020007dc.c) with this overlay's
// symbols: differ 48 of 80. Left: what that draft's header records.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_0200e88c;
extern u16 *gData_02006890;
extern u16 *Data_02006894;
extern u16 *Data_02006880;

void Func_02004d58(s32 a, s32 b);
void Func_020048e8(void);

void Func_020045f8(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_0200e88c = slots;
  gData_02006890 = colours;
  Data_02006894 = a;
  Data_02006880 = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int green = *colours++;
      int index = *slots++;
      int blue = *colours++;
      int value = (blue << 10) | (green << 5) | red;

      ((u16 *)0x05000000)[index] = value;
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)0x05000000, saved, 0x84000070, REG_DMA3);
  Dma_Set((const void *)0x05000200, saved + 448, 0x84000070, REG_DMA3);
  Func_02004d58(0x10000, 0);
  if ((s8)gPartyState.unk2c2 == 2)
    Func_020048e8();
}
