// Draft, differ 48 of 80: overlay 694's copy of the function drafted in
// resource_662_c_020007dc.c (overlay 662's Func_020007dc, differ 48 there), whose
// header says what it is, what still differs and what was tried.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_02006010;
extern u16 *gData_02006014;
extern u16 *Data_02006018;
extern u16 *gData_02006004;

void Func_020041cc(s32 a, s32 b);
void Func_02000328(void);

void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02006010 = slots;
  gData_02006014 = colours;
  Data_02006018 = a;
  gData_02006004 = b;
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
  Func_020041cc(0x10000, 0);
  if ((s8)gAdventure.unk2c2 == 2)
    Func_02000328();
}
