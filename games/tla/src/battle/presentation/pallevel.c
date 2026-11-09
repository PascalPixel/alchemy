#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "battle_work.h"

void ColorBuffer_Scale(u16 *source, u16 *destination, s32 scale, s32 count);

/// Sets the palette brightness. Level 0 transfers the original colours
/// unchanged.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  u16 *palette = screen->palette;
  u32 ime;
  u32 *slot;

  // The save in a once-only block loads REG_IME's address after the stack
  // frame is made, and a loop on a zero local stores it through slot before
  // IME is cleared; without them, 8 of 54 instructions differ in tla-en.
  do {
    u16 *imeReg;
    s32 once;

    /* FAKEMATCH(temporary): a pointer to ime, which keeps the save in a stack
     * slot stored through r6; as ime directly, 33 of 54 instructions differ
     * in tla-en. */
    slot = &ime;
    imeReg = (u16 *)&REG_IME;
    once = 0;

    do {
      *slot = *imeReg;
      *imeReg = (u32)imeReg;
    } while (once);
  } while (0);

  if (level == 0) {
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(6, 0), DMA_COPY_HALFWORDS(128),
            REG_DMA3);
  } else {
    ColorBuffer_Scale(palette, (u16 *)0x050000c0,
                      screen->brightness = 0x10000 - level * 1092, 128);
  }
  {
    u32 saved = *slot;

    REG_IME = saved;
  }
}
