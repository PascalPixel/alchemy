#include "types.h"

/*
 * Resource 37a overlay ROOT at 0x02002108 (1192 bytes,
 * 0x02002108-0x020025af).
 *
 * Role, settled BEFORE any disassembly (call-graph-first, HANDOVER §0):
 * this overlay's image offset 0 is an exported-entry veneer table of
 * `ldr r4,[pc,#0] / bx r4` pairs (`0x47204c00` followed by the target word),
 * and its FIRST entry is 0x0200a109 = 0x02002108 + the Thumb bit under the
 * proven 0x02008000 link base.  So this owner is exported entry 0 — the
 * overlay's root, the per-invocation map/scene entry point.  Reading the rest
 * of that table also named the other roots (0x02008031, 0x02008039,
 * 0x0200803d, 0x02008045, 0x0200804d) before anything was opened.
 *
 * Link base proof: see the header of
 * semantic/overlays/resource_37a_c_02000488.c — eleven odd
 * `0x0200[89ab]xxx` words in `assets/code/resource_37a_overlay.s` resolve to
 * already-tracked byte-exact sibling entries at `word - 0x8000 - 1`.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7} / sub sp,#8`
 * prologue at 0x02002108 and the matching interworking return
 * `add sp,#8 / pop {r3} / mov r8,r3 / pop {r5,r6,r7} / pop {r1} / bx r1` at
 * 0x02002582.  The popped return address goes to r1, N != 0, so r0 survives
 * and IS the result; `movs r0, #0` immediately precedes the epilogue, so the
 * owner returns the constant 0 (s32).
 *
 * The 8 bytes of stack frame are the fifth and sixth arguments of the
 * six-argument renderer entry Func_08009180 ([sp,#0] and [sp,#4]), which are
 * frequently written BEFORE the register arguments and are fed from long-lived
 * registers (r5, r6, r7, r8).  Those lifetimes were recovered from the
 * predecessor blocks; a per-site argument window would drop them.
 *
 * Pool map, derived by walking the owner's control flow from the prologue.
 * There are TWO pools and the first ends mid-row:
 *   0x0200238c-0x020023e3   hopped by the bare `b.n 0x02002510` at 0x0200238a;
 *                           code resumes at 0x020023e4, which is the target of
 *                           the `b.n` at 0x02002274.  Nothing falls into it.
 *   0x02002590-0x020025af   the trailing pool after `bx r1` at 0x0200258e.
 * Pool words, in order:
 *   0x03001ebc 0x00000809 0x00000814 0x00000819 0x00000109 0x00000201
 *   0x002051cc 0x00000202 0x00202db1 0x0000080a 0x02000240 0x00000813
 *   0x00000812 0x0000080b 0x0000080c 0x0000080d 0x0000080e 0x00000822
 *   0x00000826 0x00000827 0x00000828 0x00000829
 *   | 0x0000080b 0x0000080c 0x0000080d 0x0000080e 0x00000812 0x00000309
 *     0x02000240 0x00000814
 * No pool word decodes as a BL prefix, so neither pool injects a phantom call
 * site.
 *
 * All 82 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are NOT the callees.
 * sites=82, distinct_targets=19 (77 veneer, 5 overlay-local prologue), against
 * the inventory's calls=80 — a floor, not an equality test.
 *
 * Per-target multiset, tool count == C call count, checked PER TARGET:
 *   Func_02001380    1     Func_02001510    1     Func_02001790    1
 *   Func_020025b0    1     Func_02002614    1     Func_080000c0    3
 *   Func_08009180   24     Func_08009188    2     Func_080091c0    3
 *   Func_080091f0    1     Func_080770c0   23     Func_080770c8   10
 *   Func_080770d0    4     Func_0808a0f0    2     Func_0808a2f8    1
 *   Func_0808a330    1     Func_0808a338    1     Func_0808a348    1
 *   Func_0808a5e0    1
 *
 * Shared-call-site care taken (the trap that inflates a multiset):
 *  - Func_0808a330 at 0x0200217c is ONE site reached from two arms with a
 *    different r0 (0x002051cc from the 0x0201 arm, 0x00202db1 from the 0x0202
 *    arm).  The argument is hoisted into a local and the call written once.
 *  - Func_080091c0 is written three times because there are genuinely three
 *    distinct sites (0x0200250c, 0x02002544, 0x0200257e), on three different
 *    paths that never share one.
 *  - The `chapter == 6` arm falls THROUGH into the `progressed = 1` join at
 *    0x0200221e rather than branching to it; that is a fall-through, not a
 *    second assignment site.
 *  - The four behaviourally identical `flag -> two renderer rows -> any = 1`
 *    blocks at 0x020023e4 are NOT merged into a loop: they are eight distinct
 *    Func_08009180 sites with different row data, and folding them would
 *    deflate the multiset by six.
 *
 * Globals:
 *  - 0x03001ebc is the runtime-globals pointer CELL (HANDOVER: one
 *    dereference, not two).  The single write here is the additive form of the
 *    displacement/value shape: r2 is built as 448 (224 << 1) and used as the
 *    DISPLACEMENT, then `adds r2,#68` makes 516 which is the stored VALUE.  So
 *    the word at runtime + 448 becomes 516; reading it as "runtime + 516" is
 *    the natural mistake here.
 *  - 0x02000240 is the cross-overlay RAM global block, not an in-image
 *    address: it is BELOW the 0x02008000 link band, so the file-offset reading
 *    does not apply.  The owner reads the SIGNED HALFWORD at +450 (225 << 1)
 *    three times and switches on it; that is the neighbour of the documented
 *    +448 halfword of the `Data_02000240[224]` idiom.  It behaves as a chapter
 *    or progress step (cases 4, 5, 6 and 8).
 *
 * Func_080770c0/Func_080770c8/Func_080770d0 are the global story-flag
 * test/set/clear trio resolved in semantic/overlays/resource_3cb_c_020010e8.c
 * and re-used across this overlay (see resource_37a_c_020025b0.c, which tests
 * exactly the 0x080b..0x080e quartet this owner both reads and clears).
 * Func_020025b0 is that overlay-local "all four of 0x080b..0x080e are set"
 * predicate, and its result selects between the two big renderer blocks here —
 * an independent confirmation of that sibling's reading.
 *
 * Uncertainties: 0x0109/0x0201/0x0202/0x0200/0x0309/0x0144 and the 0x08xx
 * values are story-flag ids, not addresses; 0x002051cc and 0x00202db1 are the
 * two Func_0808a330 selectors and are neither in-image (they are below the
 * link band and 0x00202db1 is not word-aligned) nor further identified;
 * Func_0808a2f8 is reached with no argument register set by this owner.
 * Arities are taken from the registers and stack slots written before each
 * site.
 */

/* Overlay-local callees (this owner's script-command handlers and predicate). */
void Func_02001380(void);
void Func_02001510(void);
void Func_02001790(void);
void Func_02002614(void);
s32 Func_020025b0(void);

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_080000c0();
void Func_08009180();
void Func_08009188();
void Func_080091c0();
void Func_080091f0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a0f0();
void Func_0808a2f8();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();
void Func_0808a5e0();

/* Used for its return value. */
s32 Func_080770c0();

s32 Func_02002108(void)
{
    u8 **runtime_globals = (u8 **)0x03001ebc;
    u8 *runtime;
    s16 *progress = (s16 *)((u8 *)0x02000240 + 450);    /* 225 << 1 */
    s32 selector;
    s32 progressed;
    s32 any;
    s32 chapter;

    Func_080000c0(1);

    runtime = *runtime_globals;
    *(s32 *)(runtime + 448) = 516;                      /* 224 << 1; 448 + 68 */

    progressed = 0;

    if (Func_080770c0(0x809) != 0
        && Func_080770c0(0x814) == 0
        && Func_080770c0(0x819) == 0)
        Func_080770c8(0x144);                           /* 162 << 1 */

    Func_0808a338(0x10000, 0);                          /* 128 << 9 */

    if (Func_080770c0(0x109) != 0) {
        if (Func_080770c0(0x201) != 0) {
            selector = 0x2051cc;
            goto install;
        }
        if (Func_080770c0(0x202) != 0) {
            selector = 0x202db1;
        install:
            /* One call site, reached from both arms with a different r0. */
            Func_0808a330(selector, 1);
            Func_0808a348(1);
            Func_080000c0(1);
        }
    } else {
        Func_080770c8(0x200);                           /* 128 << 2 */
        if (Func_080770c0(0x80a) != 0)
            Func_0808a0f0(16, 0x2400000, 0x780000);     /* 144 << 18, 240 << 15 */
    }

    chapter = *progress;
    if (chapter == 4) {
        if (Func_080770c0(0x813) != 0)
            goto after_chapter;
        Func_02001380();
        Func_080770c8(0x813);
        progressed = 1;
    } else if (chapter == 5) {
        if (Func_080770c0(0x812) != 0)
        Func_02001510();
        Func_080770c8(0x812);
        Func_080770d0(0x80b);
            goto after_chapter;
        Func_080770d0(0x80c);
        Func_080770d0(0x80d);
        Func_080770d0(0x80e);
        progressed = 1;
    } else if (chapter == 6) {
        /* Both paths of this test reach the shared `progressed = 1` join. */
        if (Func_080770c0(0x812) != 0) {
            Func_02001790();
            Func_080770c8(0x822);
        }
        progressed = 1;
    }

after_chapter:
    if (Func_080770c0(0x80b) != 0)
        Func_080770c8(0x826);
    if (Func_080770c0(0x80c) != 0)
        Func_080770c8(0x827);
    if (Func_080770c0(0x80d) != 0)
        Func_080770c8(0x828);
    if (Func_080770c0(0x80e) != 0)
        Func_080770c8(0x829);

    Func_080000c0(4);

    if (progressed != 0)
        goto epilogue_tail;

    if (Func_020025b0() != 0) {
        /* All four of 0x080b..0x080e are set: the completed layout. */
        { s32 f1 = 12; s32 g1 = 5; Func_08009180(30, 44, 30, 38,  f1, g1); }
        { s32 f2 = 4; s32 g2 = 1; Func_08009180(30, 44, 34, 37,  f2, g2); }
        { s32 f3 = 8; s32 g3 = 4; Func_08009180(14, 41, 32, 41,  f3, g3); }
        { s32 f4 = 4; s32 g4 = 2; Func_08009180(45, 28, 34, 10,  f4, g4); }
        { s32 f5 = 4; s32 g5 = 2; Func_08009180(45, 30, 16, 10,  f5, g5); }
        { s32 f6 = 8; s32 g6 = 4; Func_08009180(14, 45, 14, 41,  f6, g6); }

        if (*progress != 8
            && Func_080770c0(0x814) == 0) {
            if (Func_080770c0(0x819) != 0) {
                { s32 f7 = 2; s32 g7 = 1; Func_08009180(0, 32, 17, 39,  f7, g7); }
                { s32 f8 = 2; s32 g8 = 3; Func_08009180(42, 62, 17, 36,  f8, g8); }
                { s32 f9 = 2; s32 g9 = 1; Func_08009180(0, 32, 17, 40,  f9, g9); }
                { s32 f10 = 2; s32 g10 = 1; Func_08009180(0, 32, 17, 41,  f10, g10); }
                { s32 f11 = 2; s32 g11 = 3; Func_08009180(0, 32, 17, 42,  f11, g11); }
            } else {
                { s32 f12 = 2; s32 g12 = 6; Func_08009180(44, 59, 17, 37,  f12, g12); }
            }
            Func_08009188(9);
            Func_08009188(10);
        }

        Func_0808a0f0(16, 0, 0);
        goto epilogue_tail;
    }

    /* Otherwise draw one pair of rows per set flag of the 0x080b..0x080e set. */
    any = 0;
    if (Func_080770c0(0x80b) != 0) {
        { s32 f13 = 2; s32 g13 = 1; Func_08009180(45, 28, 34, 10,  f13, g13); }
        { s32 f14 = 2; s32 g14 = 1; Func_08009180(45, 30, 16, 10,  f14, g14); }
        any = 1;
    }
    if (Func_080770c0(0x80c) != 0) {
        { s32 f15 = 2; s32 g15 = 1; Func_08009180(47, 28, 36, 10,  f15, g15); }
        { s32 f16 = 2; s32 g16 = 1; Func_08009180(47, 30, 18, 10,  f16, g16); }
        any = 1;
    }
    if (Func_080770c0(0x80d) != 0) {
        { s32 f17 = 2; s32 g17 = 1; Func_08009180(45, 29, 34, 11,  f17, g17); }
        { s32 f18 = 2; s32 g18 = 1; Func_08009180(45, 31, 16, 11,  f18, g18); }
        any = 1;
    }
    if (Func_080770c0(0x80e) != 0) {
        { s32 f19 = 2; s32 g19 = 1; Func_08009180(47, 29, 36, 11,  f19, g19); }
        { s32 f20 = 2; s32 g20 = 1; Func_08009180(47, 31, 18, 11,  f20, g20); }
        any = 1;
    }

    if (Func_080770c0(0x812) != 0 || any != 0) {
        { s32 f21 = 8; s32 g21 = 3; Func_08009180(30, 43, 32, 40,  f21, g21); }
        { s32 f22 = 8; s32 g22 = 1; Func_08009180(30, 43, 33, 39,  f22, g22); }
        { s32 f23 = 3; s32 g23 = 3; Func_08009180(30, 43, 36, 38,  f23, g23); }
        { s32 f24 = 8; s32 g24 = 4; Func_08009180(36, 58, 32, 41,  f24, g24); }
    }

    { s32 f25 = 17; s32 g25 = 6; Func_080091c0(15, 6, 2, 1,  f25, g25); }

epilogue_tail:
    if (Func_080770c0(0x309) == 0 && *progress == 8) {
        Func_02002614();
        Func_080770c8(0x309);
        { s32 f26 = 17; s32 g26 = 6; Func_080091c0(15, 6, 2, 1,  f26, g26); }
    } else if (Func_080770c0(0x814) != 0) {
        Func_0808a5e0(141);
        Func_080091f0(0x10000, 0x10000, 0x10000);        /* 128 << 9 */
        Func_0808a2f8();
        { s32 f27 = 17; s32 g27 = 6; Func_080091c0(15, 6, 2, 1,  f27, g27); }
    }

    return 0;
}
