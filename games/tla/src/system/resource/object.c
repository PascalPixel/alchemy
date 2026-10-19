/// @file
/// Resources: clear an animation record.

#include "dma.h"
#include "animspr.h"
#include "io_reg.h"

/// Zero one AnimationEntry; do nothing for NULL.
void ResourceMetadata_ClearRecord(void *dst) {
  if (dst != 0) {
    volatile u32 zero;

    zero = 0;
    Dma_Set(&zero, dst,
            ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16) |
                (sizeof(struct AnimationEntry) / 4),
            REG_DMA3);
  }
}
