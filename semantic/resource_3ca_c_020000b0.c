#include "types.h"

/*
 * Resource 3ca palette-cycle step at 0x020000b0 (80 bytes,
 * 0x020000b0-0x020000ff).
 *
 * Complete owner: `push {r5, lr}` at 0x020000b0, interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x020000e2.  The popped register is r0, so
 * the popped value is the return address and the owner is `void`.  The
 * remaining 24 bytes of the row (0x020000e8-0x020000ff) are this owner's
 * literal pool and are data:
 *
 *   0x020000e8  0x00000000   the reset value
 *   0x020000ec  0x0200981c   the frame counter (in-image, file offset 0x181c)
 *   0x020000f0  0x020094ac   the colour strip  (in-image, file offset 0x14ac)
 *   0x020000f4  0x040000d4   DMA3 source register
 *   0x020000f8  0x050000e8   palette RAM destination
 *   0x020000fc  0x80000006   DMA3 control: enable, 16-bit, 6 units
 *
 * The 0x02008000 link base holds for this overlay: 0x0200981c and 0x020094ac
 * both land inside the image at value - 0x8000, and the byte-exact sibling
 * assets/code/resource_3ca_c_020010d4.c already treats 0x02008f31/0x02008f81
 * the same way.  The overlay image is writable EWRAM, so the counter below is
 * genuinely mutable overlay data, not ROM.
 *
 * The single call site was resolved with tools/overlay_call_targets.ts:
 * 0x020000b8 -> veneer 0x02001314 -> Func_030003f0.  That word has no Thumb
 * bit, so it is one of the ARM-mode helpers relocated into IWRAM (the same
 * family as the 0x030001d8 square root and the 0x03000380 divide), reached
 * through the ordinary veneer table with no r3/r4 load.
 */

extern u16 Data_0200981c;   /* animation frame counter */
extern u16 Data_020094ac[]; /* colour strip the window is copied from */

/*
 * Relocated IWRAM helper, two integer arguments.  Its neighbour 0x030003e0 is
 * established as the remainder helper (see
 * semantic/overlays/resource_374_c_02000248.c), and the two are sixteen bytes
 * apart, which is the usual shape of a shared divide routine with a quotient
 * entry and a remainder entry.  So this is read as the quotient: the strip
 * index advances one colour every six frames and repeats after six steps,
 * which is exactly the 36-frame wrap below.
 *
 * UNCERTAINTY: that identification rests on the neighbouring entry point, not
 * on a disassembly of the IWRAM copy.  `counter % 6` would also yield 0..5, so
 * the call is preserved verbatim rather than replaced by an operator.
 */
s32 Func_030003f0();

void Func_020000b0(void)
{
    volatile u32 *dma3;
    s32 step;
    u16 frame;

    frame = Data_0200981c;
    step = Func_030003f0(frame, 6);

    /*
     * lsls #16 / lsrs #15 zero-extends the helper's result to 16 bits and
     * doubles it, i.e. it indexes the strip by halfwords.
     */
    dma3 = (volatile u32 *)0x040000D4;
    dma3[0] = (u32)&Data_020094ac[(u16)step];
    dma3[1] = 0x050000E8;
    dma3[2] = 0x80000006;
    /* The `subs r3, #12` that follows the `stmia r3!` restores the register to
     * the DMA base and is then dead; it has no observable effect. */

    frame = Data_0200981c;
    frame = (u16)(frame + 1);
    Data_0200981c = frame;

    /*
     * cmp is on the halfword shifted up by 16 against 0x23 << 16, so it is an
     * unsigned comparison of the 16-bit counter against 35: the counter runs
     * 0..35 and wraps, giving a 36-frame cycle.
     */
    if (frame > 0x23) Data_0200981c = 0;
}
