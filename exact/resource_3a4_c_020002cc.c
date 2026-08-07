/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-swap-adjacent-shifts.  The
 * last residual was `lsls r3,r3,#14' before `lsls r2,r2,#11' at 0x02000316,
 * where r3 continues the older chain (its `asrs r3,r3,#14' at 0x02000312)
 * while r2 was only just materialised.  The sibling shift pair at 0x02000354
 * keeps the scheduler's order under the same age rule, which is what makes the
 * rule usable on this row.
 */

#include "types.h"

/*
 * resource_3a4 owner at 0x020002cc, 188 bytes: a scripted approach --
 * face two records at each other, set a distance-derived field on the
 * first, walk it toward the second by spinning a one-call wait loop
 * until their +12 fields cross, then hand off to Func_02003850 with
 * this row's own incoming argument.  A PUBLISHED pointer (sweep B),
 * never the target of a `bl`.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020002cc through the
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02000378-0x0200037c, then the two-word literal pool
 * 0x02000380-0x02000387; the next owner begins at 0x02000388.
 * measureSpan reports 178; the pool makes it 188.  One argument (r0,
 * saved into r7 across the body and spent at the very end), void.
 *
 * Eighteen calls, every one resolved by targetOffset against the raw
 * image rather than read from the listing.  Two are into this overlay
 * (Func_02003850) and the rest are import veneers.
 *
 * THE WAIT LOOP is the shape worth naming: `Func_0808a0e8(0)` then a
 * test-first loop whose body is a single `Func_080000c0(1)`, comparing
 * the two records' +12 fields shifted right by 14.  The compiler laid
 * it out as a branch INTO the test at 0x02000350 with the body above
 * it at 0x0200034a, so read naively from the top the body looks like
 * unreachable code between two calls.  It is a while loop, not a
 * fall-through.
 *
 * Func_02003850's drafted signature is (s32 slot, s32 heading), so this
 * row's parameter is a slot; it is passed through untouched with a
 * heading of 0.
 *
 * Uncertainty: Func_0808a460 at 0x020002e0 is called with r0 still
 * holding the record-8 pointer that Func_0808a080 just returned, and
 * nothing re-materialises r0 for it.  That is equally consistent with a
 * no-argument function and with one taking that record, so it is
 * spelled void and flagged here rather than guessed -- the same
 * ambiguity applies to Func_0808a018 at 0x020002e4.  Record fields +8,
 * +12, +16, +40 and +80 are recorded by offset with roles open; +80
 * holds a pointer whose byte at +9 is OR-ed with 12, which matches the
 * handle-flags field resource_3a4_c_02002a10.c already describes.  The
 * two pool constants 0x00033333 and 0x00019999 and the literals 22, 10,
 * 152, 159, 7, 20 and the 128<<11 bias are transcribed as materialised.
 */
















extern u8 * Func_02003e6c(s32 id);
extern u8 * Func_02003e74(s32 id);
extern void Func_02003fe2(void);
extern void Func_02003e56(void);
extern void Func_02003ede(s32 arg0, s32 arg1);
extern void Func_02003e5c(s32 arg0);
extern void Func_0200401a(s32 arg0);
extern void Func_02003ea4(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003f14(s32 arg0, s32 arg1);
extern void Func_02003e28(u8 *record, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02003dae(s32 arg0);
extern void Func_02003f26(s32 arg0);
extern void Func_02003dc6(s32 arg0);
extern void Func_02003ed6(void);
extern void Func_02004084(s32 arg0);
extern void Func_02003bbc(s32 slot, s32 heading);
extern void Func_02003dea(s32 arg0);
extern void Func_0200408e(void);
void Func_020002cc(s32 slot)
{
    u8 *mover;
    u8 *goal;
    u8 *handle;
    s32 distance;

    mover = Func_02003e6c(0);
    goal = Func_02003e74(8);
    Func_02003fe2();
    Func_02003e56();
    Func_02003ede(0, 22);
    Func_02003e5c(10);
    Func_0200401a(152);
    Func_02003ea4(0, 0x00033333, 0x00019999);

    distance = *(s32 *)(goal + 12) - *(s32 *)(mover + 12);
    if (distance < 0) {
        distance = *(s32 *)(mover + 12) - *(s32 *)(goal + 12);
    }
    *(s32 *)(mover + 40) = ((distance >> 14) << 14) + (128 << 11);

    Func_02003f14(0, 7);
    Func_02003e28(mover, *(s32 *)(goal + 8), *(s32 *)(goal + 12),
        *(s32 *)(goal + 16));
    Func_02003dae(10);

    handle = *(u8 **)(mover + 80);
    handle[9] |= 12;

    Func_02003f26(0);
    while ((*(s32 *)(goal + 12) >> 14) < (*(s32 *)(mover + 12) >> 14)) {
        Func_02003dc6(1);
    }

    Func_02003ed6();
    Func_02004084(159);
    Func_02003bbc(slot, 0);
    Func_02003dea(20);
    Func_0200408e();
}
