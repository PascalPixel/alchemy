#include "types.h"

/*
 * Func_080e72e0 - point HBlank DMA channel 0 at the per-scanline palette
 * stream held in the workspace.
 *
 * Manifest row 080e72de is 66 bytes and is classified
 * `executable_gap_continuation` / `merge_with_owner`, but the walk from the
 * preceding row 080e727c stops at 0x080e72de: that function's last reached
 * instruction is its own return, nothing branches into this row, and no `bl`
 * or pool word anywhere in the image names it.  The row is therefore a
 * two-byte alignment halfword at 0x080e72de followed by an independent leaf
 * function at 0x080e72e0 that has no prologue and ends in `bx lr`.
 *
 * Executable range 0x080e72e0..0x080e730a; a two-byte alignment halfword and
 * the 20-byte literal pool at 0x080e730c close the row.
 *
 * This is the same shape as Func_080dbb9c and Func_0808f498: disable channel
 * 0 in two writes, then rewrite source, destination and control with one
 * `stmia`.
 */

extern u8 *Data_03001eec;

#define REG_DMA0SAD (*(volatile u32 *)0x040000b0)
#define REG_DMA0DAD (*(volatile u32 *)0x040000b4)
#define REG_DMA0CNT (*(volatile u32 *)0x040000b8)
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba)

void Func_080e72e0(void)
{
    u8 *root;

    root = Data_03001eec;

    REG_DMA0CNT_H = REG_DMA0CNT_H & 0xc5ff;
    REG_DMA0CNT_H = REG_DMA0CNT_H & 0x7fff;

    REG_DMA0SAD = (u32)(root + 0x1f80);
    REG_DMA0DAD = 0x05000000;
    REG_DMA0CNT = 0xa2600001;
}
