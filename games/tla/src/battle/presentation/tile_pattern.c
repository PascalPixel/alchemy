/// @file
/// Battle presentation: compact VRAM patterns and reset BG0 vertical scrolling.

#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

/// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
/// 20 bytes from 0x0600028c.
s32 BattleStaging_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

void BattleTransition_ClearBg0Vofs(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

void Party_ReservedNoOp5B14(void) {}
