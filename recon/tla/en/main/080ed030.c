/* Draft, differ 16 of 51: the native keeps row + i in r0 and its wrapped copy in r5, and after the
 * modulo call builds the source index in r5 in place; this form keeps the sum in r5 and builds the
 * index in r0. Measured: with the source address computed before the cell (one Dma_Set expression)
 * loop.c strength-reduces the cell index and nothing lines up (differ 62); the cell first, as here,
 * keeps the loop plain. A conditional expression, a reused source variable and the body as an inline
 * are the same 16; eleven minutes of permuter reached differ 10 with a temporary for the source. */
#include "types.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gMapCellBuffer[];

static inline void MapCell_CopyToScreen(s32 column, s32 line)
{
    s32 source = line > 23 ? line - 24 : line;
    s32 cell;

    cell = ((column & 31) + (line << 5) + 128) % 0x300;
    Dma_Set(gMapCellBuffer + ((column + (source << 6)) << 6), (u8 *)0x06004000 + (cell << 6),
        0x84000010, REG_DMA3);
}

void Func_080ed030(s32 column, s32 row)
{
    u32 i;

    for (i = 0; i <= 20; i++)
        MapCell_CopyToScreen(column, row + i);
}
