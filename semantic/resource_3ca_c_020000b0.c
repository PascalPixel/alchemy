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
 * The single call site is `bl 0x020013ce` at 0x020000b8, read straight off the
 * reference, and naming the callee at that address is what reproduces it. The
 * overlay's own assembly carries `.set sub_020013ce, 0x020013ce` and calls it
 * the same way. Declaring the IWRAM target instead made the linker synthesise a
 * fresh veneer at 0x02000100 and the `bl` missed by the whole veneer table.
 *
 * WITHDRAWN, not merely restated: this comment used to say the site resolved
 * `0x020000b8 -> veneer 0x02001314 -> Func_030003f0`, an ARM-mode helper
 * relocated into IWRAM. The overlay does not contain 0x02001314 anywhere, and
 * the veneer body around 0x020013ce loads the pool word 0x0808a071, which is a
 * main-image address with the Thumb bit, not an IWRAM one. Whatever the chain
 * is, it is not the one recorded here, so the name below stays at the call
 * address and claims nothing about what it reaches.
 */

extern u16 Data_0200981c;   /* animation frame counter */
extern u16 Data_020094ac[]; /* colour strip the window is copied from */

/*
 * The in-overlay call target, two integer arguments, named at its own address
 * because that is all the bytes establish.
 *
 * Behaviour is still read as a quotient: the result is zero-extended to 16 bits
 * and doubled to index the strip by halfwords, the strip advances one colour
 * every six frames, and the counter wraps at 36, which is six steps of six.
 * `counter % 6` would yield the same 0..5, so the call is preserved verbatim
 * rather than replaced by an operator.
 *
 * UNCERTAINTY: the divide-helper identification that used to be asserted here
 * rested on a veneer chain the overlay does not contain, so it is withdrawn
 * rather than reworded. What the arithmetic around the call site shows is a
 * value in 0..5; what produces it is unproven.
 */
s32 Func_020013ce();

void Func_020000b0(void)
{
    volatile u32 *dma3;
    s32 step;
    u32 frame;

    frame = Data_0200981c;
    step = Func_020013ce(frame, 6);

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
