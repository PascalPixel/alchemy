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
 * All 82 call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
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
void Func_02004a80();
s32 Func_02004ade();
s32 Func_02004ae8();
s32 Func_02004af2();
void Func_02004b06();
void Func_02004c18();
s32 Func_02004b0e();
s32 Func_02004b18();
s32 Func_02004b26();
void Func_02004c3a();
void Func_02004c50();
void Func_02004af6();
void Func_02004b50();
s32 Func_02004b4e();
void Func_02004bc8();
s32 Func_02004b76();
void Func_0200354a(void);
void Func_02004b8c();
s32 Func_02004b90();
void Func_020036f4(void);
void Func_02004ba6();
void Func_02004bb4();
void Func_02004bba();
void Func_02004bc0();
void Func_02004bc6();
s32 Func_02004bc2();
void Func_020039a6(void);
void Func_02004bd8();
s32 Func_02004bd8_b();
void Func_02004bea();
s32 Func_02004be8();
void Func_02004bfa();
s32 Func_02004bf8();
void Func_02004c0a();
s32 Func_02004c08();
void Func_02004c1a();
void Func_02004bd0();
s32 Func_0200481e(void);
void Func_02004c1c();
void Func_02004c32();
void Func_02004c44();
void Func_02004c56();
void Func_02004c66();
void Func_02004c76();
s32 Func_02004cac();
s32 Func_02004cb6();
void Func_02004cac_b();
void Func_02004cbe();
void Func_02004cd0();
void Func_02004ce2();
void Func_02004cf2();
void Func_02004d06();
void Func_02004d14();
void Func_02004d1a();
void Func_02004da4();
s32 Func_02004d9e();
void Func_02004d96();
void Func_02004da6();
s32 Func_02004dce();
void Func_02004dc6();
void Func_02004dd6();
s32 Func_02004dfe();
void Func_02004df6();
void Func_02004e06();
s32 Func_02004e2e();
void Func_02004e26();
void Func_02004e36();
s32 Func_02004e5e();
void Func_02004e5a();
void Func_02004e6c();
void Func_02004e7c();
void Func_02004e8e();
void Func_02004eb2();
s32 Func_02004ec8();
void Func_02004b40(void);
void Func_02004eee();
void Func_02004eea();
s32 Func_02004f02();
void Func_02005044();
void Func_02004f14();
void Func_02005020();
void Func_02004f24();

                         

/* Old-style declarations: arities vary per call site across this overlay. */

                     

                     

                     

                     

/* Used for its return value. */

s32 Func_02002108(void)
{
    u8 **runtime_globals = (u8 **)0x03001ebc;
    u8 *runtime;
    s16 *progress = (s16 *)((u8 *)0x02000240 + 450);    /* 225 << 1 */
    s32 selector;
    s32 progressed;
    s32 any;
    s32 chapter;

    Func_02004a80(1);

    runtime = *runtime_globals;
    *(s32 *)(runtime + 448) = 516;                      /* 224 << 1; 448 + 68 */

    progressed = 0;

    if (Func_02004ade(0x809) != 0
        && Func_02004ae8(0x814) == 0
        && Func_02004af2(0x819) == 0)
        Func_02004b06(0x144);                           /* 162 << 1 */

    Func_02004c18(0x10000, 0);                          /* 128 << 9 */

    if (Func_02004b0e(0x109) != 0) {
        if (Func_02004b18(0x201) != 0) {
            selector = 0x2051cc;
            goto install;
        }
        if (Func_02004b26(0x202) != 0) {
            selector = 0x202db1;
        install:
            /* One call site, reached from both arms with a different r0. */
            Func_02004c3a(selector, 1);
            Func_02004c50(1);
            Func_02004af6(1);
        }
    } else {
        Func_02004b50(0x200);                           /* 128 << 2 */
        if (Func_02004b4e(0x80a) != 0)
            Func_02004bc8(16, 0x2400000, 0x780000);     /* 144 << 18, 240 << 15 */
    }

    chapter = *progress;
    if (chapter == 4) {
        if (Func_02004b76(0x813) != 0)
            goto after_chapter;
        Func_0200354a();
        Func_02004b8c(0x813);
        progressed = 1;
    } else if (chapter == 5) {
        if (Func_02004b90(0x812) != 0)
        Func_020036f4();
        Func_02004ba6(0x812);
        Func_02004bb4(0x80b);
            goto after_chapter;
        Func_02004bba(0x80c);
        Func_02004bc0(0x80d);
        Func_02004bc6(0x80e);
        progressed = 1;
    } else if (chapter == 6) {
        /* Both paths of this test reach the shared `progressed = 1` join. */
        if (Func_02004bc2(0x812) != 0) {
            Func_020039a6();
            Func_02004bd8(0x822);
        }
        progressed = 1;
    }

after_chapter:
    if (Func_02004bd8_b(0x80b) != 0)
        Func_02004bea(0x826);
    if (Func_02004be8(0x80c) != 0)
        Func_02004bfa(0x827);
    if (Func_02004bf8(0x80d) != 0)
        Func_02004c0a(0x828);
    if (Func_02004c08(0x80e) != 0)
        Func_02004c1a(0x829);

    Func_02004bd0(4);

    if (progressed != 0)
        goto epilogue_tail;

    if (Func_0200481e() != 0) {
        /* All four of 0x080b..0x080e are set: the completed layout. */
        Func_02004c1c(30, 44, 30, 38, 12, 5);
        Func_02004c32(30, 44, 34, 37, 4, 1);
        Func_02004c44(14, 41, 32, 41, 8, 4);
        Func_02004c56(45, 28, 34, 10, 4, 2);
        Func_02004c66(45, 30, 16, 10, 4, 2);
        Func_02004c76(14, 45, 14, 41, 8, 4);

        if (*progress != 8
            && Func_02004cac(0x814) == 0) {
            if (Func_02004cb6(0x819) != 0) {
                Func_02004cac_b(0, 32, 17, 39, 2, 1);
                Func_02004cbe(42, 62, 17, 36, 2, 3);
                Func_02004cd0(0, 32, 17, 40, 2, 1);
                Func_02004ce2(0, 32, 17, 41, 2, 1);
                Func_02004cf2(0, 32, 17, 42, 2, 3);
            } else {
                Func_02004d06(44, 59, 17, 37, 2, 6);
            }
            Func_02004d14(9);
            Func_02004d1a(10);
        }

        Func_02004da4(16, 0, 0);
        goto epilogue_tail;
    }

    /* Otherwise draw one pair of rows per set flag of the 0x080b..0x080e set. */
    any = 0;
    if (Func_02004d9e(0x80b) != 0) {
        Func_02004d96(45, 28, 34, 10, 2, 1);
        Func_02004da6(45, 30, 16, 10, 2, 1);
        any = 1;
    }
    if (Func_02004dce(0x80c) != 0) {
        Func_02004dc6(47, 28, 36, 10, 2, 1);
        Func_02004dd6(47, 30, 18, 10, 2, 1);
        any = 1;
    }
    if (Func_02004dfe(0x80d) != 0) {
        Func_02004df6(45, 29, 34, 11, 2, 1);
        Func_02004e06(45, 31, 16, 11, 2, 1);
        any = 1;
    }
    if (Func_02004e2e(0x80e) != 0) {
        Func_02004e26(47, 29, 36, 11, 2, 1);
        Func_02004e36(47, 31, 18, 11, 2, 1);
        any = 1;
    }

    if (Func_02004e5e(0x812) != 0 || any != 0) {
        Func_02004e5a(30, 43, 32, 40, 8, 3);
        Func_02004e6c(30, 43, 33, 39, 8, 1);
        Func_02004e7c(30, 43, 36, 38, 3, 3);
        Func_02004e8e(36, 58, 32, 41, 8, 4);
    }

    Func_02004eb2(15, 6, 2, 1, 17, 6);

epilogue_tail:
    if (Func_02004ec8(0x309) == 0 && *progress == 8) {
        Func_02004b40();
        Func_02004eee(0x309);
        Func_02004eea(15, 6, 2, 1, 17, 6);
    } else if (Func_02004f02(0x814) != 0) {
        Func_02005044(141);
        Func_02004f14(0x10000, 0x10000, 0x10000);        /* 128 << 9 */
        Func_02005020();
        Func_02004f24(15, 6, 2, 1, 17, 6);
    }

    return 0;
}
