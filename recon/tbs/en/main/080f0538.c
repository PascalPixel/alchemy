/* Draft, not exact (2026-09-24): 89 differing halfwords, 216 of 220 bytes.
   Rewritten on the DisplayScroll_InitObjectTable pattern (each OAM pair through
   a copied entry pointer, the column step is 0x200000). Residual: the
   reference loads the frame with ldrh and masks it with 7 before sign-extending
   it for the group, and keeps x in lr (copied to ip per row), 0x40004000 in sl,
   0x300 in r8 and the column step in r7. */
#include "TYPES.H"
#include "DMA.H"

void Func_080f0538(void)
{
    u16 frame = *(u16 *)0x02004c00;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = (u32 *)(*(u8 **)0x02004c0c + 192);
    s32 x = 16 - fine;
    s32 row;

    for (row = 0; row <= 15; row++) {
        s32 y = 0x180000;
        s32 col;

        for (col = 5; col >= 0; col--) {
            u32 *q = entry;

            *q++ = x | y | 0x40004000;
            *q = tile;
            tile += 4;
            entry += 2;
            if (tile == 0x300)
                tile = 0;
            y += 0x200000;
        }
        x += 8;
    }
    Dma_Set(*(void **)0x02004c0c, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (*(s16 *)0x02004c04 == 0 && (*(u32 *)0x03001800 & 3) == 0)
        (*(u16 *)0x02004c00)++;
}
