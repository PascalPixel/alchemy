#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

extern u16 *gData_02006010;
extern u16 *gData_02006014;
extern u16 *Data_02006018;
extern u16 *gData_02006004;

void Func_02000328(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
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
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000328();
}
