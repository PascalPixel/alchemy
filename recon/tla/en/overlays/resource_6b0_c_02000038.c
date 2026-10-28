// Draft of overlay 6b0's Func_02000038, the twin of overlay 662's
// Func_020007dc: its draft (resource_662_c_020007dc.c) with this overlay's
// symbols: differ 48 of 80. Left: what that draft's header records.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_02004444;
extern u16 *Data_02004448;
extern u16 *Data_0200444c;
extern u16 *Data_02004438;

void Func_02003458(s32 a, s32 b);
void Func_02000328(void);

void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02004444 = slots;
  Data_02004448 = colours;
  Data_0200444c = a;
  Data_02004438 = b;
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
  Func_02003458(0x10000, 0);
  if ((s8)gAdventure.unk2c2 == 2)
    Func_02000328();
}
