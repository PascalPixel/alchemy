// Draft, differ 48 of 80: overlay 675's copy of overlay 662's Func_020007dc
// (the same listing), whose draft holds its notes.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_02001a24;
extern u16 *gData_02001a28;
extern u16 *Data_02001a2c;
extern u16 *gData_02001a18;

void Func_020012ec(s32 a, s32 b);
void Func_02000328(void);

void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02001a24 = slots;
  gData_02001a28 = colours;
  Data_02001a2c = a;
  gData_02001a18 = b;
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
  Func_020012ec(0x10000, 0);
  if ((s8)gPartyState.unk2c2 == 2)
    Func_02000328();
}
