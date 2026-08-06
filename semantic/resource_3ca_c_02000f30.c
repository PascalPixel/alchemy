#include "types.h"

/*
 * resource_3ca DMA bank-flip leaf at 0x02000f30, complete 80-byte span through
 * its six-word pool. It selects one of two 1,920-byte runtime banks, publishes
 * the bank's first word directly, then configures DMA0 to stream the remainder
 * to the same display register.
 *
 * The reference keeps ONE pointer to 0x040000b0 alive across the whole body:
 * the two 0x040000ba masks are `ldrh r4,[r3,#10]` / `strh r2,[r3,#10]` off
 * that same base, and the three register words are written with a single
 * `stmia r3!, {r0, r1, r2}` (followed by a dead `subs r3,#12` that restores
 * the pointer).  Reconstructing that -- a plain, NON-volatile three-word
 * struct at 0x040000b0 plus a volatile u16 view of the same base indexed at
 * [5] -- took this owner from 24 differing halfwords to 18 and restored the
 * exact 80-byte size.
 *
 * STILL-OPEN, 18 halfwords.  The whole residual is instruction scheduling:
 * the reference finishes the bank address (`adds r0,r0,r3`) before it touches
 * any of the register pool words, while the fork interleaves the control-mask
 * pool loads into the middle of that address computation.  Neither the
 * declaration order of the locals, splitting the bank arithmetic into a named
 * intermediate, nor writing the masks as `0xc5ff & control[5]` moves it.
 * overlay_mode_cohort.ts singles + 40 pairs (118 configurations) found no
 * exact: the best, sched-store-first, only goes 31 -> 28 differing bytes.
 */

typedef struct {
    u32 source;
    u32 destination;
    u32 control;
} DmaTransfer_02000f30;

extern u8 *Data_03001ed8;

void Func_02000f30(void)
{
    u8 *runtime = Data_03001ed8;
    u32 *words = (u32 *)(runtime + runtime[0x0f00] * 1920);
    DmaTransfer_02000f30 *dma0 = (DmaTransfer_02000f30 *)0x040000b0;
    volatile u16 *control = (volatile u16 *)dma0;
    volatile u32 *display = (volatile u32 *)0x0400001c;

    control[5] &= 0xc5ff;
    control[5] &= 0x7fff;
    *display = *words++;
    dma0->source = (u32)words;
    dma0->destination = (u32)display;
    dma0->control = 0xa6600001;
}
