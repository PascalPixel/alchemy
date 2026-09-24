/* Draft, not exact (2026-09-24): 103 differing halfwords, 212 of 220 bytes.
   Residual: the reference keeps the 0x40004000, 0x300 and 0x20000 loop
   constants in sl/r8/r7, writes each OAM pair through a stmia copy of the
   entry pointer, and computes frame & 7 before sign-extending frame. */

#include "TYPES.H"
#include "DMA.H"

void Func_080f0538(void)
{
    u16 frame = *(volatile u16 *)0x02004c00;
    u32 fine = frame & 7;
    s32 offset = (((s16)frame / 8) & 0x1f) * 24;
    u32 *entry = (u32 *)(*(u8 **)0x02004c0c + 192);
    s32 x = 16 - fine;
    s32 row;

    for (row = 0; row <= 15; row++) {
        s32 y = 0x180000;
        s32 col;

        for (col = 5; col >= 0; col--) {
            entry[0] = x | y | 0x40004000;
            entry[1] = offset;
            offset += 4;
            entry += 2;
            if (offset == 0x300)
                offset = 0;
            y += 0x20000;
        }
        x += 8;
    }
    Dma_Set(*(void **)0x02004c0c, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (*(s16 *)0x02004c04 == 0 && (*(u32 *)0x03001800 & 3) == 0)
        (*(u16 *)0x02004c00)++;
}
