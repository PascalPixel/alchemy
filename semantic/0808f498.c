#include "types.h"

/*
 * Func_0808f498 - install the window rectangle and its HBlank window DMA.
 *
 * Manifest row 0808f498 is 148 bytes and is classified
 * `executable_gap_continuation` / `merge_with_owner`, but a control-flow walk
 * shows it is not a continuation of anything: the preceding row 0808f32c never
 * branches or falls into it, the body has no prologue and no stack frame, and
 * it ends in `bx lr`.  Its entry is published as a Thumb function pointer
 * (0x0808f499) by five separate pool words - 0x0808f7bc, 0x0808feac,
 * 0x0808fec8, 0x08090114 and 0x08090274 - so it is an independently callable
 * leaf that is only ever reached indirectly, which is why no direct `bl`
 * reaches it and why the boundary classifier could not place it.
 *
 * Executable range 0x0808f498..0x0808f4f8 and 0x0808f51c..0x0808f526.  The
 * 36-byte literal pool at 0x0808f4f8 sits between the two halves - the `b.n`
 * at 0x0808f4f6 jumps over it - and the trailing 6 bytes from 0x0808f526 are
 * alignment plus the last pool word.
 *
 * The DMA channel is channel 0 at 0x040000B0; `stmia r3!, {r0,r1,r2}` writes
 * source, destination and control in one go, which is why the control word is
 * a single constant here.
 */

extern u8 *Data_03001ecc;

#define REG_DISPCNT (*(volatile u16 *)0x04000000)
#define REG_WIN0H (*(volatile u16 *)0x04000040)
#define REG_WIN1H (*(volatile u16 *)0x04000042)
#define REG_WIN0V (*(volatile u16 *)0x04000044)
#define REG_WIN1V (*(volatile u16 *)0x04000046)
#define REG_WININ (*(volatile u16 *)0x04000048)
#define REG_WINOUT (*(volatile u16 *)0x0400004a)
#define REG_DMA0SAD (*(volatile u32 *)0x040000b0)
#define REG_DMA0DAD (*(volatile u32 *)0x040000b4)
#define REG_DMA0CNT (*(volatile u32 *)0x040000b8)
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba)

void Func_0808f498(void)
{
    u8 *root;
    const u16 *window;
    u32 slot;

    root = Data_03001ecc;
    slot = root[0x539];

    /* 644 bytes per slot, built as ((slot * 5) << 5 + slot) * 4. */
    window = (const u16 *)(root + ((((slot * 4 + slot) << 5) + slot) * 4));

    /* Stop channel 0 before repointing it: first drop the timing and
       control bits, then the enable bit. */
    REG_DMA0CNT_H = REG_DMA0CNT_H & 0xc5ff;
    REG_DMA0CNT_H = REG_DMA0CNT_H & 0x7fff;

    REG_DISPCNT = REG_DISPCNT | 0x6000;

    REG_WININ = window[0];
    REG_WINOUT = window[1];
    REG_WIN0H = window[2];
    REG_WIN1H = window[3];
    REG_WIN0V = 160;
    REG_WIN1V = 160;

    /* Per-scanline refresh of WIN0H/WIN1H from the table that follows the
       four halfwords just installed. */
    REG_DMA0SAD = (u32)(window + 4);
    REG_DMA0DAD = 0x04000040;
    REG_DMA0CNT = 0xa6600001;
}
