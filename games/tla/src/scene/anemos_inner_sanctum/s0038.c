#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

extern u16 *gData_02004444;
extern u16 *Data_02004448;
extern u16 *Data_0200444c;
extern u16 *Data_02004438;

void Func_02000328(void);

/// Writes one background palette entry from its red, green and blue.
// A statement macro, written do { } while (0): this store matches through that
// shape (see the header).
#define BG_PLTT_SET_RGB(index, r, g, b)                                        \
  do {                                                                         \
    int value = RGB(r, g, b);                                                  \
                                                                               \
    BG_PLTT[index] = value;                                                    \
  } while (0)

void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02004444 = slots;
  Data_02004448 = colours;
  Data_0200444c = a;
  Data_02004438 = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int index = *slots++;
      int red = *colours++;
      int green = *colours++;
      int blue = *colours++;

      BG_PLTT_SET_RGB(index, red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)0x05000000, saved, 0x84000070, REG_DMA3);
  Dma_Set((const void *)0x05000200, saved + 448, 0x84000070, REG_DMA3);
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000328();
}
