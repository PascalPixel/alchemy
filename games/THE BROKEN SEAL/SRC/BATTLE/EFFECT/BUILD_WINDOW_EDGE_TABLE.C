#include "DMA.H"

/* Builds the per-line WIN0H table for lines 8-135 (the edge at 0x02010000
   less each line's inset, clamped to the screen; other lines closed) and
   arms the H-blank DMA that feeds it to WIN0H. */
void BattleFx_BuildWindowEdgeTable(void)
{
    u16 *edge;
    u16 *line;
    u8 *inset;
    s32 i;
    s32 right;

    edge = (u16 *)0x02010000;
    line = (u16 *)0x02010082;
    inset = (u8 *)0x02010002;
    for (i = 0; i != 160; i++) {
        if ((u32)(i - 8) <= 127) {
            right = *edge - inset[i - 8];
            if (right < 0)
                right = 0;
            if (right > 240)
                right = 240;
            line[i] = right;
        } else {
            line[i] = 0xfff1;
        }
    }
    {
        volatile u16 *channel = (volatile u16 *)0x040000b0;
        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
        (void)channel[5];
        Dma_Set((void *)0x02010082, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
    }
}
