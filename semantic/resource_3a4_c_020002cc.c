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

extern u8 *Func_0808a080(s32 id);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 arg0);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a460(void);
extern void Func_0808a5e8(void);
extern void Func_080000c0(s32 arg0);
extern void Func_08009150(u8 *record, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080f9010(s32 arg0);

extern void Func_02003850(s32 slot, s32 heading);

void Func_020002cc(s32 slot)
{
    u8 *mover;
    u8 *goal;
    u8 *handle;
    s32 distance;

    mover = Func_0808a080(0);
    goal = Func_0808a080(8);
    Func_0808a460();
    Func_0808a018();
    Func_0808a100(0, 22);
    Func_0808a010(10);
    Func_080f9010(152);
    Func_0808a090(0, 0x00033333, 0x00019999);

    distance = *(s32 *)(goal + 12) - *(s32 *)(mover + 12);
    if (distance < 0) {
        distance = *(s32 *)(mover + 12) - *(s32 *)(goal + 12);
    }
    *(s32 *)(mover + 40) = ((distance >> 14) << 14) + (128 << 11);

    Func_0808a100(0, 7);
    Func_08009150(mover, *(s32 *)(goal + 8), *(s32 *)(goal + 12),
        *(s32 *)(goal + 16));
    Func_080000c0(10);

    handle = *(u8 **)(mover + 80);
    handle[9] |= 12;

    Func_0808a0e8(0);
    while ((*(s32 *)(goal + 12) >> 14) < (*(s32 *)(mover + 12) >> 14)) {
        Func_080000c0(1);
    }

    Func_0808a020();
    Func_080f9010(159);
    Func_02003850(slot, 0);
    Func_080000c0(20);
    Func_0808a5e8();
}
