/* Draft, not exact (2026-09-28): 220 of 220 bytes, 8 differing halfwords.
   Writes the 16x6 object-attribute strip for the scrolling frame, then
   DMAs it to OAM and advances the frame counter every fourth tick.
   The row x is the loop giv row * 8 + 16 - fine: strength reduction gives
   the reference's x in lr and the per-row copy in ip, and an up-counting
   column loop is reversed into its 5..0 counter. Every loop body and the
   tail are exact. Residual, preheader order only: the reference computes
   16 - fine before loading the 0x300 tile limit, and copies lr to ip
   between the two halves of the 0x180000 row start; here loop pass 1
   hoists the 0x300 compare constant ahead of the giv initialisation. The
   tile-wrap spelling (==, >, -=, compound, reordered), loop bound forms
   and declaration placement do not move it. */
#include "TYPES.H"
#include "DMA.H"

void Func_080f0538(void)
{
    u32 frame = *(u16 *)0x02004c00;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = (u32 *)(*(u8 **)0x02004c0c + 192);
    s32 row;

    for (row = 0; row <= 15; row++) {
        s32 y = 0x180000;
        s32 col;

        for (col = 0; col < 6; col++) {
            u32 *q = entry;

            *q++ = (row * 8 + 16 - fine) | y | 0x40004000;
            *q = tile;
            tile += 4;
            entry += 2;
            if (tile == 0x300)
                tile = 0;
            y += 0x200000;
        }
    }
    Dma_Set(*(void **)0x02004c0c, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (*(s16 *)0x02004c04 == 0 && (*(u32 *)0x03001800 & 3) == 0)
        (*(u16 *)0x02004c00)++;
}
