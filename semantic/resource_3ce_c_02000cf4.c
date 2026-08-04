#include "types.h"

/*
 * Resource 3ce value-entry loop at 0x02000cf4 (532 bytes,
 * 0x02000cf4-0x02000f07).
 *
 * Link base proof for this overlay: see the header of
 * semantic/overlays/resource_3ce_c_02000b10.c — fifteen odd
 * `0x0200[89ab]xxx` words in `assets/code/resource_3ce_overlay.s` resolve at
 * `word - 0x8000 - 1` onto already-tracked byte-exact sibling entries.  Under
 * that base this owner's three even pool words 0x02009390, 0x0200939c and
 * 0x020093b4 are IN-IMAGE DATA at file offsets 0x1390, 0x139c and 0x13b4
 * (parity test: even = data, odd = Thumb entry), and all three are the first
 * argument of Func_08015098 — a text/label draw.  `resource_3ce_overlay.s`
 * spells those three words literally in its data block at that offset, which
 * confirms them as data rather than code.
 *
 * Role: unlike this overlay's other rows, 0x02000cf4 is NOT named by the
 * script-command table (`0xffffNNNN / entry / arg` triples) and no in-image
 * pool word holds 0x02008cf5, so nothing inside the overlay references it.
 * It is nonetheless an ordinary, self-contained, frame-balanced owner — a
 * prologue, one main loop and one interworking return — so it converts
 * normally.  Behaviourally it is a numeric value-entry screen: two windows,
 * a wrapping 0..269 counter driven by the d-pad, a preview redrawn whenever
 * the counter moves, and A/B to confirm.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 /
 * push {r6, r7} / sub sp, #4` prologue at 0x02000cf4 and the matching
 * interworking return `add sp,#4 / pop {r3,r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` at 0x02000ec0.  r0 holds the popped
 * return address, so nothing is returned: the owner is `void`.
 *
 * The 4 bytes of stack frame are the fifth argument of Func_08015010 (twice,
 * value 2) and of Func_080150a8 (once, the just-cleared redraw flag).
 *
 * High registers carry real state and were recovered from the predecessor
 * blocks rather than per-site: r6 is the counter, r7 the first window handle,
 * sl the second window handle, r8 the redraw flag.  r5 is reused three times
 * for unrelated purposes (the constant 2, the masked counter, and two
 * different IWRAM pointers); conflating those lifetimes is the standard
 * decompiler defect here.
 *
 * Pool map, derived by walking the owner's control flow from the prologue:
 * every halfword from 0x02000cf4 to 0x02000ecd is reached — the only backward
 * branch is the loop edge at 0x02000ea2 and every forward branch lands inside
 * the code — so the 2-byte zero word at 0x02000ece is the alignment word and
 * 0x02000ed0-0x02000f07 is the single trailing literal pool:
 *   0x040000d4 0x05000200 0x050001c0 0x80000010 0x050001e8 0x80000001
 *   0x02009390 0x000001ff 0x0200939c 0x00000182 0x00000075 0x020093b4
 *   0x03001c94 0x03001b04
 * No pool word decodes as a BL prefix, so no phantom call site is introduced.
 *
 * All 31 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are NOT the callees.  This
 * overlay resolves to veneers only, so every call here is a main-image import.
 * sites=31, distinct_targets=15, against the inventory's calls=31.
 *
 * Per-target multiset, tool count == C call count, checked PER TARGET:
 *   Func_030003ac 1   Func_080000c0 3   Func_08015010 2   Func_08015018 2
 *   Func_08015078 1   Func_08015080 1   Func_08015098 3   Func_080150a8 1
 *   Func_08015270 3   Func_08015278 1   Func_08077018 1   Func_08077030 1
 *   Func_08077250 1   Func_080a1048 1   Func_080f9010 9
 *
 * Func_030003ac is the relocated IWRAM modulo helper (HANDOVER: the veneers
 * into 0x030003xx are the divide/modulo pair reached as ordinary two-argument
 * calls, not `call_via`; this site loads no r3/r4).  It is used exactly once,
 * as `(counter + 270) % 270`, which is the wrap that keeps the counter in
 * 0..269 after a -30 step.
 *
 * The six `d-pad bit -> step -> redraw` arms are behaviourally identical apart
 * from the step, and are deliberately NOT merged: they are six distinct
 * Func_080f9010(0x6f) call sites and folding them would deflate the multiset
 * by five.  Steps are -1 / +1 / +10 / -10 / +30 / -30 for bits 0x40, 0x80,
 * 0x10, 0x20, 0x100 and 0x200 of the word at 0x03001b04.
 *
 * The A-button arm and the B-button arm SHARE their tail: when
 * `Func_08077030(counter)` returns -1 the A path branches directly into the B
 * path's body at 0x02000e0a.  That is one call site reached from two control
 * paths, so it is written once with a label and a `goto` rather than copied
 * per arm — copying would inflate the multiset by two.
 *
 * The 0x040000d4 block is two DMA3 register bursts written by `stmia r3!` with
 * r3 rewound by 12 between them, so both bursts start at 0x040000d4.  The
 * second destination is built in assembly as the first destination plus 28.
 *
 * Uncertainties: 0x03001c94 and 0x03001b04 are the two IWRAM input words (a
 * held mask and an edge mask, from how they are tested); which is which is not
 * established beyond the bit meanings above.  0x0182 and 0x0075 are id bases
 * added to the masked counter; 0x70, 0x71, 0x6f and 0xaf are Func_080f9010 cue
 * ids.  Arities are taken from the registers and stack slots written before
 * each site.
 */

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_080000c0();
void Func_08015018();
void Func_08015078();
void Func_08015080();
void Func_08015098();
void Func_080150a8();
void Func_08015270();
void Func_08015278();
void Func_08077018();
void Func_080a1048();
void Func_080f9010();

/* Used for their return values. */
s32 Func_030003ac();
s32 Func_08015010();
s32 Func_08077030();
s32 Func_08077250();

void Func_02000cf4(void)
{
    u32 *dma3 = (u32 *)0x040000d4;
    s32 *held = (s32 *)0x03001c94;
    s32 *pressed = (s32 *)0x03001b04;
    s32 window;                 /* r7 */
    s32 preview;                /* sl */
    s32 counter;                /* r6 */
    s32 redraw;                 /* r8 */
    s32 masked;                 /* r5 */

    Func_080f9010(0x70);

    window = Func_08015010(0, 0, 30, 7, 2);
    preview = Func_08015010(0, 8, 13, 10, 2);

    counter = 1;
    redraw = 1;

    /* Two DMA3 bursts; r3 is rewound 12 bytes, so both start at DMA3SAD. */
    dma3[0] = 0x05000200;
    dma3[1] = 0x050001c0;
    dma3[2] = 0x80000010;
    dma3[0] = 0x050001e8;
    dma3[1] = 0x050001c0 + 28;
    dma3[2] = 0x80000001;

    Func_080000c0(1);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            counter = Func_030003ac(counter + 270, 270);    /* 135 << 1 */

            Func_08015270(window);
            Func_08015278(window);
            Func_08015098(0x02009390, window, 0, 0);
            Func_080150a8(counter, 0, window, 0x50, redraw);

            if (Func_08077250() != 0) {
                masked = counter & 0x1ff;
                Func_08015098(0x0200939c, window, 0, 32);
                Func_08077018(masked);
                Func_08015080(masked + 0x182, window, 120, 0);
                masked += 0x75;
                Func_08015078(masked, window, 0, 16);
                Func_08015270(preview);
                Func_080a1048(preview, counter);
            } else {
                Func_08015098(0x020093b4, window, 0, 32);
            }
        }

        if ((*held & 1) != 0) {
            if (Func_08077030(counter) == -1)
                goto confirm;
            Func_080f9010(0xaf);
        }
        if ((*held & 2) != 0) {
        confirm:
            /* One call site, reached from the A arm and the B arm. */
            Func_080f9010(0x71);
            goto finish;
        }

        if ((*pressed & 0x40) != 0) {
            counter -= 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x80) != 0) {
            counter += 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x10) != 0) {
            counter += 10;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x20) != 0) {
            counter -= 10;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x100) != 0) {           /* 128 << 1 */
            counter += 30;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x200) != 0) {           /* 128 << 2 */
            counter -= 30;
            redraw = 1;
            Func_080f9010(0x6f);
        }

        Func_080000c0(1);
    }

finish:
    Func_08015270(window);
    Func_080000c0(1);
    Func_08015018(window, 1);
    Func_08015018(preview, 1);
}
