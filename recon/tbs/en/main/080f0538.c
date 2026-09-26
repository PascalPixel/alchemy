/* NONMATCHING: 224 of 220 bytes, 24 aligned edits. A u32 frame snapshot
   restores the unsigned load, mask, and signed group calculation. Keeping
   the row's x separate from the next x restores both high-register saves,
   but advances x before the inner loop and changes register priorities.
   The reference keeps x in lr, row x in ip, the column step in r7 and row
   count in r6. Explicit negation gives 25 edits; retain this closer model. */
#include "TYPES.H"
#include "DMA.H"

void Func_080f0538(void)
{
    u32 frame = *(u16 *)0x02004c00;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = (u32 *)(*(u8 **)0x02004c0c + 192);
    s32 x = 16 - fine;
    s32 row;

    for (row = 0; row <= 15; row++) {
        u32 row_x = x;
        s32 y = 0x180000;
        s32 col;

        x += 8;
        for (col = 5; col >= 0; col--) {
            u32 *q = entry;

            *q++ = row_x | y | 0x40004000;
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
