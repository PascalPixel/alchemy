#include "types.h"
#include "dma.h"
#include "iwram_call.h"

/// Moves rows 1 to rows - 2 of a buffer down one row by DMA, dropping the
/// last, then clears row 0 (stride bytes per row).
void BattleEffect_ShiftRowsDown(u8 *base, s32 stride, s32 rows) {
  u8 *source = base + (rows - 2) * stride;
  u8 *destination = base + (rows - 1) * stride;
  s32 i;

  for (i = rows - 1; i != 1; i--) {
    Dma_Set(source, destination, (DMA_ENABLE | DMA_32BIT) << 16 | (stride / 4),
            REG_DMA3);
    source -= stride;
    destination -= stride;
  }
  Iwram_FillWords(base, stride, 0);
}
