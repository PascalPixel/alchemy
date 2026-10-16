//--------------------------------------------------------------------------
// tile_pattern.c
// Battle presentation: compact VRAM patterns and reset BG0 vertical scrolling.
//--------------------------------------------------------------------------

#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

//--------------------------------------------------------------------------
// Move 16 bytes from VRAM 0x06000290 to 0x06000280,
// then zero 20 bytes starting at 0x0600028c.
//--------------------------------------------------------------------------
s32 BattlePresentation_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
          REG_DMA3); // 8 halfwords.
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

//--------------------------------------------------------------------------
// Reset BG0 vertical scroll to 0.
//--------------------------------------------------------------------------
void BattlePresentation_ClearBg0VerticalOffset(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

//--------------------------------------------------------------------------
// Do nothing.
//--------------------------------------------------------------------------
void Party_ReservedNoOp5B14(void) {}
