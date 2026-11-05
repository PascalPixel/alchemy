// Draft, differ 12 of 80 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 22 of 80 (tla-en); older figures below predate it.
// Draft, differ 48 of 80 (was 50): overlay 662's Func_020007dc, the
// representative of its copies in 675, 694, 6aa, 6ac, 6ae and 6b0. It records
// the four lists, writes each listed palette entry from its red, green and
// blue, then copies two palettes from block32 and starts the blend. Measured:
// as a while loop loop.c hoists 0x05000000 and the end marker and steps the
// colour pointer as a giv (97); the loop test written twice, at the entry and
// after the body, lets jump2 join them as the native does; u16 locals add
// shifts (63). The colour computed into a local before the store frees green's
// register for 0x05000000, as the native does (stored directly, reload spills
// the constant to r7 and the function saves r7). Left, mostly scheduling: the
// native loads each entry's next marker before it builds 0xffff, so jump2
// joins the two tests one insn later than here; it steps both pointers right
// after the loads; and it allocates green and the index the other way round.
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
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value = (blue << 10) | (green << 5) | red;

      /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
       * order around it; as the plain statement, 22 of 80 instructions differ
       * in tla-en. */
      do {
        ((u16 *)0x05000000)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)0x05000000, saved, 0x84000070, REG_DMA3);
  Dma_Set((const void *)0x05000200, saved + 448, 0x84000070, REG_DMA3);
  Func_020018f0(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000acc();
}
