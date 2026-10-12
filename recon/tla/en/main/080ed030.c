#include "types.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gMapCellBuffer[];

void Func_080ed030(s32 column, s32 row)
{
    u32 i;

    for (i = 0; i <= 20; i++) {
        u32 line = row + i;
        s32 source = line;

        if ((s32)line > 23)
            source -= 24;
        Dma_Set(gMapCellBuffer + ((column + (source << 6)) << 6),
            (u8 *)0x06004000 + ((s32)((column & 31) + (line << 5) + 128) % 0x300 << 6),
            0x84000010, REG_DMA3);
    }
}
