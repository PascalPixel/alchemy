#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"

// Steps the 64 background colours one shade toward the resource's palette.
void Func_0815f0a0(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(0x20),
          REG_DMA3);
  dst[0] = 0;
  for (i = 0; i != 64; i++, col++) {
    s32 r = *col & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 b = (*col >> 10) & 0x1f;
    s32 dst_r = dst[i] & 0x1f;
    s32 dst_g = (dst[i] >> 5) & 0x1f;
    s32 dst_b = (dst[i] >> 10) & 0x1f;

    if (r < dst_r)
      r++;
    else if (r > dst_r)
      r--;
    if (g < dst_g)
      g++;
    else if (g > dst_g)
      g--;
    if (b < dst_b)
      b++;
    else if (b > dst_b)
      b--;
    dst[i] = (b << 10) | (g << 5) | r;
  }
  Dma_Set(dst, (void *)BG_PLTT, DMA_COPY_HALFWORDS(64), REG_DMA3);
}
