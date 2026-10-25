// Draft of overlay 6ae's Func_02004ef0, the twin of overlay 662's
// Func_020007dc: its draft (resource_662_c_020007dc.c) with this overlay's
// symbols: differ 48 of 80. Left: what that draft's header records.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_02006ce0;
extern u16 *Data_02006ce4;
extern u16 *Data_02006ce8;
extern u16 *Data_02006cd4;

void Func_0200575c(s32 a, s32 b);
void Func_020051e0(void);

void Func_02004ef0(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02006ce0 = slots;
  Data_02006ce4 = colours;
  Data_02006ce8 = a;
  Data_02006cd4 = b;
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
  Func_0200575c(0x10000, 0);
  if ((s8)gPartyState.unk2c2 == 2)
    Func_020051e0();
}
