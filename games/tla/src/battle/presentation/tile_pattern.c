/// @file
/// Battle presentation: compact VRAM patterns and reset BG0 vertical scrolling.

#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

/// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
/// 20 bytes from 0x0600028c.
s32 BattlePresentation_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
          REG_DMA3); // 8 halfwords.
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

/// Resets BG0's vertical scroll to 0.
void BattlePresentation_ClearBg0VerticalOffset(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// Does nothing.
void Party_ReservedNoOp5B14(void) {}
