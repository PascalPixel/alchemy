#include "types.h"

/*
 * Func_080dbb9c - point HBlank DMA channel 0 at the per-scanline BG2 affine
 * origin stream held in the workspace.
 *
 * Manifest row 080dbb9a is 66 bytes and is classified
 * `executable_gap_continuation` / `merge_with_owner`.  The walk from the
 * nearest preceding non-continuation row, 080db6e0, reaches 0x080dbb12 and
 * stops there, and the row is separated from it by a gap, so this is not that
 * function's tail.  Nothing in the image branches to 0x080dbb9c or stores it
 * as a pointer: like Func_080e72e0 it is an independent leaf reached only
 * through the caller-supplied dispatch that the boundary classifier cannot
 * see.  The row is a two-byte alignment halfword followed by the function.
 *
 * Executable range 0x080dbb9c..0x080dbbc4; the 24-byte literal pool at
 * 0x080dbbc4 closes the row.
 */

extern u8 *Data_03001eec;

#define REG_DMA0SAD (*(volatile u32 *)0x040000b0)
#define REG_DMA0DAD (*(volatile u32 *)0x040000b4)
#define REG_DMA0CNT (*(volatile u32 *)0x040000b8)
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba)

void Func_080dbb9c(void)
{
    u8 *root;

    root = Data_03001eec;

    REG_DMA0CNT_H = REG_DMA0CNT_H & 0xc5ff;
    REG_DMA0CNT_H = REG_DMA0CNT_H & 0x7fff;

    REG_DMA0SAD = (u32)(root + 0x6980);
    REG_DMA0DAD = 0x04000028;
    REG_DMA0CNT = 0xa6600001;
}
