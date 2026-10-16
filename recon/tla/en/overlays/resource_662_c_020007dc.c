// Draft, differ 50 of 80: overlay 662's Func_020007dc, the representative of
// its copies in 675, 694, 6aa, 6ac, 6ae and 6b0. It records the four lists,
// writes each listed palette entry from its red, green and blue, then copies
// two palettes from block32 and starts the blend. Measured: as a while loop
// loop.c hoists 0x05000000 and the end marker and steps the colour pointer as a
// giv (97); the loop test written twice, at the entry and after the body, lets
// jump2 join them as the native does, but the entry keeps the whole marker
// build and the body allocates green and the index the other way round, with
// 0x05000000 kept in r7; u16 locals add shifts (63); the colour expression in
// other orders and a memory barrier at the loop head do not help.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_02001fd8;
extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;
extern u16 *gData_02001fcc;

void Func_020018f0(s32 a, s32 b);
void Func_02000acc(void);

void Func_020007dc(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02001fd8 = slots;
  gData_02001fdc = colours;
  gData_02001fe0 = a;
  gData_02001fcc = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int green = *colours++;
      int index = *slots++;
      int blue = *colours++;

      ((u16 *)0x05000000)[index] = (blue << 10) | (green << 5) | red;
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)0x05000000, saved, 0x84000070, REG_DMA3);
  Dma_Set((const void *)0x05000200, saved + 448, 0x84000070, REG_DMA3);
  Func_020018f0(0x10000, 0);
  if ((s8)gPartyState.unk2c2 == 2)
    Func_02000acc();
}
