typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 37b cutscene camera step at 0x02000c8c.
 *
 * Complete owner: the three-part prologue at 0x02000c8c
 * (`push {r5, r6, r7, lr}` / `push {r9, sl, fp}` via r5-r7 / `push {r8}`),
 * a 20-byte frame (`sub sp, #20`), and the matching interworking return at
 * 0x02000e8e..0x02000e9e which unwinds all of it and ends `pop {r0} / bx r0`.
 * The popped register is r0, so it holds the return address: the owner is
 * `void`.  The four words at 0x02000ea0-0x02000eaf are this owner's literal
 * pool; they sit after the `bx r0` and are loaded by the `ldr rN, [pc, #imm]`
 * sites at 0x02000c9a, 0x02000cb6, 0x02000d0e, 0x02000d46 and 0x02000e54.
 * They are data, not code, and register state is unaffected by them.
 *
 * Call sites resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores `target_offset - 2`, so the disassembler's annotations are wrong):
 *
 *   0x02000ca4                      -> veneer 0x0200246c -> Func_0808a080
 *   0x02000cba, 0x02000d12          -> veneer 0x02002494 -> Func_0808a0d0
 *   0x02000cc6, 0x02000d1e          -> veneer 0x020024f4 -> Func_0808a1b8
 *   0x02000d78 .. 0x02000e3e (8x)   -> veneer 0x02002404 -> Func_08009180
 *   the remaining 15 sites          -> veneer 0x0200244c -> Func_0808a010
 *
 * 28 sites, 5 distinct imports.  Multiset check: the tool's per-site list and
 * the `bl` halfword pairs in the span agree at 28 = 28, with the per-import
 * counts 8/15/1/2/2 above; the inventory's calls=25 is a lower bound, as
 * documented, and is not used as the completeness proof.
 *
 * Func_0808a080(0) returns the scene-state record; this owner reads its words
 * at +8, +12, +16 (a 16.16 coordinate triple) and branches on the +16 word
 * against 0xb30000.  The same accessor and the same field are used by
 * 0x02001624 in this overlay.  Func_08009180 is the six-argument renderer ABI:
 * r0-r3 plus two stack words at [sp, #0] and [sp, #4], which the assembly sets
 * explicitly at every one of its eight sites.  Func_0808a010(n) is the frame
 * wait used to pace each step.
 *
 * 0x03001e70 is the overlay workspace pointer.  The word AT that address is a
 * pointer; its first slot holds the address of the coordinate triple the
 * engine is currently reading.  This owner saves that slot, points it at its
 * own 12-byte stack snapshot for the duration of the pan, and restores it
 * before returning — so the local snapshot is genuinely live for the callees
 * and must not be optimised into plain locals.  It is spelled `volatile` for
 * that reason.
 *
 * Uncertainties recorded as found:
 *  - Func_0808a0d0's second argument is a plain pooled id (0x23f / 0x241) and
 *    its third a small count (0x84 / 0xde); their meaning is not established
 *    beyond "selects the dialogue or script for this branch".
 *  - The pan step is +0x10000 on the low-Z branch and -0x10000 (pool word
 *    0xffff0000) on the high-Z branch, and the closing pan reverses whichever
 *    was taken.  The branch tag is carried in r9 as 1 or 2; it is modelled as
 *    a local because nothing else reads it.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
s32 *Func_0808a080();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a010();
void Func_08009180();

void Func_02000c8c(void)
{
    volatile s32 view[3];
    s32 **anchor;
    s32 *saved;
    s32 *state;
    s32 direction;
    s32 i;

    anchor = *(s32 ***)0x03001e70;

    state = Func_0808a080(0);

    if (state[4] < 0xb30000) {
        Func_0808a0d0(0, 0x23f, 0x84);
        Func_0808a1b8(0, 0x4000, 0);
        Func_0808a010(30);

        saved = anchor[0];
        view[0] = state[2];
        view[1] = state[3];
        view[2] = state[4];
        anchor[0] = (s32 *)view;

        for (i = 0; i < 30; i++) {
            view[2] = view[2] + 0x10000;
            Func_0808a010(1);
        }
        Func_0808a010(40);
        direction = 1;
    } else {
        Func_0808a0d0(0, 0x241, 0xde);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a010(30);

        view[0] = state[2];
        view[1] = state[3];
        view[2] = state[4];
        saved = anchor[0];
        anchor[0] = (s32 *)view;

        for (i = 0; i < 30; i++) {
            view[2] = view[2] + 0xffff0000;
            Func_0808a010(1);
        }
        Func_0808a010(40);
        direction = 2;
    }

    /* Three paced runs of the same two renderer calls; only the frame wait
     * between them and the repeat count change (6 x 8, 10 x 4, 12 x 2). */
    for (i = 0; i < 6; i++) {
        Func_08009180(2, 28, 34, 10, 4, 2);
        Func_0808a010(8);
        Func_08009180(2, 30, 34, 10, 4, 2);
        Func_0808a010(8);
    }
    for (i = 0; i < 10; i++) {
        Func_08009180(2, 28, 34, 10, 4, 2);
        Func_0808a010(4);
        Func_08009180(2, 30, 34, 10, 4, 2);
        Func_0808a010(4);
    }
    for (i = 0; i < 12; i++) {
        Func_08009180(2, 28, 34, 10, 4, 2);
        Func_0808a010(2);
        Func_08009180(2, 30, 34, 10, 4, 2);
        Func_0808a010(2);
    }

    Func_08009180(2, 28, 34, 10, 4, 2);
    Func_08009180(8, 55, 32, 40, 8, 4);
    Func_0808a010(60);

    if (direction == 1) {
        for (i = 0; i < 30; i++) {
            view[2] = view[2] + 0xffff0000;
            Func_0808a010(1);
        }
    } else if (direction == 2) {
        for (i = 0; i < 30; i++) {
            view[2] = view[2] + 0x10000;
            Func_0808a010(1);
        }
    }

    anchor[0] = saved;
}
