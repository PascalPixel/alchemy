#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "battle_work.h"

void ColorBuffer_Scale(u16 *source, u16 *destination, s32 scale, s32 count);

/// Sets the palette brightness. Level 0 transfers the original colours
/// unchanged.
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  u16 *palette = screen->palette;
  u32 ime;
  u32 *slot;

  /* FAKEMATCH: the one-pass block and the loop on a zero local keep the IME
   * save through slot's register, as in ☀️; with the slot's address taken at
   * its declaration, 2 instructions differ in tla-en. */
  do {
    u16 *imeReg;
    s32 once;

    slot = &ime;
    imeReg = (u16 *)0x04000208;
    once = 0;

    do {
      *slot = *imeReg;
      *imeReg = (u32)imeReg;
    } while (once);
  } while (0);

  if (level == 0) {
    Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
  } else {
    ColorBuffer_Scale(palette, (u16 *)0x050000c0,
                      screen->brightness = 0x10000 - level * 1092, 128);
  }
  {
    u32 saved = *slot;

    REG_IME = saved;
  }
}
