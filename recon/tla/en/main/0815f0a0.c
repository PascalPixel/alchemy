// Near miss of Func_0815f0a0, ported from The Broken Seal's
// Palette_StepTowardResource (listing 97 instructions, draft 97, differ 2,
// one reordered row): the native loads the halfword mask 0x1f from the pool
// (ldr r7, the loop invariant) before it stores dst[0] = 0; here it comes
// after the store and i's copy to ip. The load has the same priority as the
// store and loses on dependants; a load latency of 3 would put it first.
// Tried: dst[0] = 0 in a one-pass block, before or after it, in the for
// header, as dst[0] = i = 0, an empty asm after it (all differ 2 or 19),
// alchemy permute 4 minutes (no better).
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"

// Steps the 64 background colours one shade toward the resource's palette.
void Func_0815f0a0(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst,
          DMA_ENABLE32 | DMA_32BIT | 0x20, REG_DMA3);
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
  Dma_Set(dst, (void *)BG_PLTT, DMA_ENABLE32 | 64, REG_DMA3);
}
