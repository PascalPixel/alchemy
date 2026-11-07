/// @file
/// Memory: fill halfwords using DMA.

#include "types.h"
#include "dma.h"
#include "io_reg.h"

/// Fills count halfwords at dst with value and returns the position after
/// the fill.
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count) {
  if (count > 0) {
    Dma_Fill16(value, dst, count * 2);
    dst += count;
  }
  return dst;
}
