/*
 * resource_3b3 owner at 0x0200274c, 696 bytes: 0x0200274c-0x02002a03.
 *
 * NO INVENTORY ROW.  Found by sweeping the two-byte gaps in
 * `metrics/gs1-en-executable.json` for `push {..,lr}` prologues; there is no
 * row for it in `out/decomp/overlays.json`, no exact sibling and no semantic
 * source, so the span below is DERIVED.
 *
 * How the span was established.  A control-flow walk from the prologue at
 * 0x0200274c, following every branch, reaches its last instruction at
 * 0x02002a02 and closes on the single matching unwind
 * `movs r0,#0 / add sp,#8 / pop {r5} / pop {r1} / bx r1` at
 * 0x020029fa-0x02002a03.  One epilogue, no `mov pc, rN` dispatch, and no `bl`
 * whose `+2` target lands inside the body, so there is no long tail branch and
 * nothing behind a jump table.  Extent 0x0200274c-0x02002a04 = 696 bytes.
 *
 * POOL MAP: EMPTY inside the span.  Every one of the 696 bytes is an
 * instruction the walk reaches.  The owner's ten literals live in the pool at
 * 0x02002a04-0x02002a2b, which is outside the registered span and is bounded
 * on the far side by the overlay's IMPORT VENEER BAND: 0x02002a2c is
 * `ldr r4,[pc,#0] / bx r4 / .word 0x03000380`, the first veneer, and it is the
 * same veneer `resource_3b3_c_020013b0.c` already documents.  So this owner is
 * the last code in the image and its end is pinned by the veneer table rather
 * than by alignment.
 *
 * LINK BASE.  One pool word is ODD: 0x0200a6b9 resolves under
 * `pool_word - 0x8000` to 0x020026b8 + the Thumb bit, and 0x020026b8 is a real
 * `push {r5, r6, lr}` prologue in this image.  It is the task handed to
 * Func_080000d0, the established task-install witness.  0x0200b6d0 is EVEN and
 * resolves to 0x000036d0, four bytes past the assembled image end (0x000036cc),
 * so by the two-sided band test it is NOT in-image; it is scratch adjacent to
 * the overlay and is treated here as an absolute address.  The same shape
 * appears in `resource_371_c_020039fc.c` with 0x0200e7a0, which is likewise a
 * few bytes past that overlay's assembled end - worth knowing before reading
 * such a word as in-image data.  0x02000240 and 0x03001ebc are BELOW the
 * 0x02008000 link band and are the usual RAM globals.
 *
 * Call accounting: 64 `bl` sites over 19 distinct targets - 37 intra-overlay
 * prologues and 27 import veneers, with no `call_via` slot.  Resolved with
 * `bun tools/overlay_call_targets.ts resource_3b3 274c 2a04`, never from
 * `overlay_show.ts`'s printed annotations.  Per-target multiset:
 * Func_02000ba4 x15, Func_0808a080 x11, Func_02000ec8 x7, Func_080770c0 x5,
 * Func_020025f8 x4, Func_02002694 x4, Func_08009180 x3, Func_0200263c x3,
 * Func_080091c0 x2, and one each of Func_020000a0, Func_0200209c,
 * Func_02001d84, Func_020024cc, Func_080000d0, Func_080091b8, Func_0808a010,
 * Func_0808a100, Func_0808a158, Func_0808a408.
 *
 * WHAT IT IS.  The overlay's scene-entry dispatcher.  It publishes the phase id
 * 516 into the workspace at `*(u32 *)0x03001ebc + 448` and then switches four
 * ways on the SIGNED halfword at `0x02000240 + 448`, the cross-overlay scene
 * sub-state: 0x74, 0x77, 0x79 and 0x7a.  Anything else falls straight to the
 * return.  The result is always 0 - `movs r0, #0` sits in the epilogue on every
 * path - but the epilogue pops into r1 rather than r0, so r0 IS the result and
 * the owner returns `s32`, not `void`.
 *
 * The 0x7a arm RE-READS the same halfword through a freshly rebuilt address
 * (`r1` still holds 0x02000240 from the entry, `r2` is rebuilt as 224 << 1)
 * before comparing.  It is the same value; the reload is written as a plain
 * `else if` rather than invented as a second slot.
 *
 * A SHARED TAIL, WRITTEN ONCE.  The 0x77 and 0x79 arms both end by calling the
 * six-argument renderer with `r2 = 1, r3 = 1` and differ only in the four
 * values they set first; 0x77 reaches it by `b.n 0x02002918` and 0x79 falls
 * into it.  There is ONE `bl` site there, so it is spelled once under a label
 * with a `goto`.  Copying it per arm would put Func_080091c0 in the source
 * three times against the histogram's two.  The third and last arm's renderer
 * call at 0x020029f6 is a genuinely separate site.
 *
 * Immediates are written as the assembly builds them: 129 << 2 = 516,
 * 224 << 1 = 448, 128 << 2 = 512, 146 << 18, 200 << 16, 200 << 4 = 3200,
 * 192 << 9 = 0x18000.
 *
 * UNCERTAINTIES:
 *  - Func_08009180 and Func_080091c0 are the established six-argument renderer
 *    ABI (four in registers, two on the stack); Func_080091b8 is called with
 *    the same six-argument shape and is assumed to share it.
 *  - The record fields touched by name are +85 (cleared on two records), +89
 *    (bit 3 set), +98 (a one-shot flag written and then tested), +108 (cleared)
 *    and the pointer at +80 whose byte at +9 gets bits 2 and 3 set.  None of
 *    them is otherwise witnessed here.
 *  - The four-entry, 20-byte-stride table at 0x0200b6d0 is cleared and given
 *    the ids 512..515; its consumer is not visible from this owner.
 *  - The 0x79 arm's search accepts the first record of 8..11 whose +8 and +16
 *    words, arithmetic-shifted right by 20, are 37 and 9.  The two tests are
 *    written separately because the assembly rejects on each independently.
 */

#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_080000d0();           /* install a per-frame task */
void Func_08009180();           /* six-argument renderer, last two on the stack */
void Func_080091b8();           /* six-argument renderer, last two on the stack */
void Func_080091c0();           /* six-argument renderer, last two on the stack */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_0808a010();           /* wait n frames */
u8 *Func_0808a080();            /* record by slot index */
void Func_0808a100();           /* set a slot's presentation mode */
void Func_0808a158();           /* per-slot presentation request */
void Func_0808a408();           /* per-slot request; interface not established */

/* Intra-overlay bodies, resolved as real prologues in this overlay. */
void Func_020000a0();
void Func_02000ba4();
void Func_02000ec8();
void Func_02001d84(void);
void Func_0200209c(void);
void Func_020024cc(void);
void Func_020025f8();
void Func_0200263c();
void Func_02002694();
void Func_020026b8(void);       /* named by the odd pool word 0x0200a6b9 */

struct Marker_0200274c {
    s32 a;
    s32 b;
    s32 c;
    s32 unused;
    s32 id;
};

s32 Func_0200274c(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s16 substate;
    u8 *record;
    u8 *attached;
    s32 slot;
    s32 index;
    struct Marker_0200274c *marker;
    s32 renderX;
    s32 renderY;
    s32 renderA;
    s32 renderB;

    *(s32 *)(workspace + (224 << 1)) = 129 << 2;        /* +448 = 516 */

    substate = *(s16 *)(0x02000240 + (224 << 1));       /* +448 */

    if (substate == 0x74) {
        Func_02000ba4(8);
        Func_02000ba4(9);
        Func_02000ba4(10);
        Func_02000ba4(11);
        Func_02000ba4(12);
        return 0;
    }

    if (substate == 0x77) {
        Func_080091b8(32, 0, 64, 32, 0, 64);

        Func_02000ba4(8);
        Func_02000ba4(9);
        Func_02000ba4(10);
        Func_02000ba4(11);
        Func_02000ba4(12);
        Func_02000ba4(13);
        Func_02000ba4(14);
        Func_02000ba4(15);

        if (Func_080770c0(0x109) == 0) return 0;
        if (Func_080770c0(128 << 2) == 0) return 0;     /* flag 512 */

        Func_08009180(79, 34, 84, 24, 1, 2);
        Func_08009180(0, 32, 32, 0, 32, 32);
        Func_08009180(32, 32, 64, 0, 32, 32);

        Func_02000ec8(9);
        Func_02000ec8(10);
        Func_02000ec8(11);
        Func_02000ec8(12);
        Func_02000ec8(13);
        Func_02000ec8(14);
        Func_02000ec8(15);

        renderA = 24;
        renderB = 8;
        renderX = 24;
        renderY = 3;
        goto render;
    }

    if (substate == 0x79) {
        Func_020000a0(146 << 18, 0, 200 << 16, 223);

        if (Func_080770c0(0x109) == 0) {
            record = Func_0808a080(0);
            record[98] = 1;
        }

        Func_0808a408(0);

        record = Func_0808a080(0);
        if (record[98] == 0) Func_0200209c();

        Func_02002694(8);
        Func_02002694(9);
        Func_02002694(10);
        Func_02002694(11);

        marker = (struct Marker_0200274c *)0x0200b6d0;
        index = 0;
        do {
            marker->a = 0;
            marker->b = 0;
            marker->c = 0;
            marker->id = index + (128 << 2);            /* 512 + index */
            marker += 1;
            index += 1;
        } while (index <= 3);

        Func_02001d84();
        Func_0808a010(1);
        Func_080000d0((s32)Func_020026b8 + 1, 200 << 4);        /* 3200 */

        if (Func_080770c0(0x109) == 0) return 0;

        for (slot = 8; slot <= 11; slot += 1) {
            record = Func_0808a080(slot);
            renderA = *(s32 *)(record + 8) >> 20;
            if (renderA != 37) continue;
            renderB = *(s32 *)(record + 16) >> 20;
            if (renderB != 9) continue;
            renderX = 27;
            renderY = 8;
            goto render;
        }
        return 0;

    render:
        Func_080091c0(renderX, renderY, 1, 1, renderA, renderB);
        return 0;
    }

    /* The 0x7a arm reloads the same halfword through a rebuilt address. */
    if (*(s16 *)(0x02000240 + (224 << 1)) != 0x7a) return 0;

    Func_0808a100(10, 2);
    Func_0808a158(10, 6);
    Func_02000ba4(8);
    Func_02000ba4(9);

    record = Func_0808a080(8);
    record[85] = 0;
    record = Func_0808a080(9);
    record[85] = 0;

    Func_020024cc();

    Func_020025f8(11);
    Func_020025f8(12);
    Func_020025f8(13);
    Func_0200263c(11);
    Func_0200263c(12);
    Func_0200263c(13);

    record = Func_0808a080(13);
    *(s32 *)(record + 108) = 0;

    Func_020025f8(14);

    record = Func_0808a080(14);
    record[89] = (u8)(record[89] | 8);

    if (Func_080770c0(0x202)) return 0;

    record = Func_0808a080(13);
    *(s32 *)(record + 24) = 192 << 9;                   /* 0x18000 */
    record = Func_0808a080(13);
    *(s32 *)(record + 28) = 192 << 9;

    record = Func_0808a080(13);
    attached = *(u8 **)(record + 80);
    attached[9] = (u8)(attached[9] | 12);

    record = Func_0808a080(14);
    attached = *(u8 **)(record + 80);
    attached[9] = (u8)(attached[9] | 12);

    Func_080091c0(26, 12, 1, 1, 22, 16);
    return 0;
}
