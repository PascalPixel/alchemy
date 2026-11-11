#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"

/// Moves the first 64 BG colours one step toward the palette of resource; the
/// backdrop colour is not written back.
void Palette_StepTowardResource(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(0x20),
          REG_DMA3);
  for (i = 0; i != 64; i++, col++) {
    s32 r = *col & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 b = (*col >> 10) & 0x1f;
    s32 targetRed = dst[i] & 0x1f;
    s32 targetGreen = (dst[i] >> 5) & 0x1f;
    s32 targetBlue = (dst[i] >> 10) & 0x1f;

    if (r < targetRed)
      r++;
    else if (r > targetRed)
      r--;
    if (g < targetGreen)
      g++;
    else if (g > targetGreen)
      g--;
    if (b < targetBlue)
      b++;
    else if (b > targetBlue)
      b--;
    dst[i] = RGB(r, g, b);
  }
  Dma_Set(dst + 1, (void *)&BG_PLTT[1], DMA_COPY_HALFWORDS(63), REG_DMA3);
}
