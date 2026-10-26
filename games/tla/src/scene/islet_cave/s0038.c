#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

extern u16 *gData_02001a24;
extern u16 *gData_02001a28;
extern u16 *Data_02001a2c;
extern u16 *gData_02001a18;

void Func_020012ec(s32 a, s32 b);
void Func_02000328(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
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
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(one-pass): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)0x05000000)[index] = value;
      } while (0);
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
