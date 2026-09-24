#include "DMA.H"

#define Map_ShowBg1FromBuffer Func_0801161c

/* Points BG1 at character block 2 and copies the 32 KB of buffered
   characters into it. */
void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)0x0400000a;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: the wrap orders the store before the DMA operands. */
    Dma_Set((const void *)0x02038000, (void *)0x06008000, 0x84002000,
            (volatile u32 *)((u8 *)bg1cnt + 0xca));
}
