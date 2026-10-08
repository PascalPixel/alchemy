#include "ram_buffer.h"
#include "dma.h"
extern u8 gMapCellBuffer[];

void BattleFx_BuildWindowEdgeTable(void)
{
    u16 *edge;
    u16 *line;
    u8 *inset;
    s32 i;
    s32 right;

    edge = (u16 *)gMapCellBuffer;
    line = (u16 *)(Ram_MapCellBuffer + 0x82);
    inset = Ram_MapCellBuffer + 2;
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
        Dma_Set(gMapCellBuffer + 0x82, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
    }
}
