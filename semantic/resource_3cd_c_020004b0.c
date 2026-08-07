#include "types.h"

/*
 * resource_3cd owner at 0x020004b0, 584 bytes: a wrapping 0..269 entry
 * selector — d-pad steps the cursor, each step then skips forward or backward
 * over entries the record store reports as unavailable, and B closes.
 *
 * Role known in advance from the in-image scene-script table: the record
 * `(0xffff000c, 0x020084b1, 0)` at file offset 0x0450 names this function
 * (0x020084b1 - 0x8000 - 1 = 0x04b0) as the selector-0x0c entry, invoked with
 * no arguments.  Link base evidence is in the header of
 * `semantic/overlays/resource_3cd_c_020000ec.c`.
 *
 * NEAR-TWIN of the already-admitted
 * `semantic/overlays/resource_3ce_c_02000cf4.c`, and it was diffed against it
 * before being written.  Shared verbatim: the Func_080f9010(0x70) opening, the
 * two DMA3 bursts (0x05000200 -> 0x050001c0 for 16 halfwords, then
 * 0x050001e8 -> that destination + 28 for one, r3 rewound by 12 between them),
 * the `counter = (counter + 270) % 270` wrap through the relocated IWRAM
 * modulo helper Func_030003ac, the six d-pad arms with steps -1 / +1 / +10 /
 * -10 / +30 / -30 for bits 0x40 / 0x80 / 0x10 / 0x20 / 0x100 / 0x200 of
 * 0x03001b04, each firing its own Func_080f9010(0x6f), the B arm on bit 1 of
 * 0x03001c94 with its Func_080f9010(0x71), and the closing sequence.  What
 * differs: the window is 12 rows rather than 7, there is no A arm and no
 * Func_08077250 variant in the repaint, and the two availability searches
 * below are new.
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` plus the sl/r9/r8 saves at
 * 0x020004b0, 4 bytes of frame for the fifth arguments; single epilogue at
 * 0x020005a2 ending `pop {r0} / bx r0`, so the popped word is the return
 * address and the owner is **void**.  Code runs 0x020004b0..0x020006bb — the
 * only backward branches are the loop edge at 0x020006ba and the two search
 * edges, and every forward branch lands inside the code — so the single
 * literal pool is 0x020006bc..0x020006f7, and 584 bytes matches the inventory
 * row exactly.  No pool word decodes as a BL prefix, so no phantom call site.
 *
 * All 28 call sites resolved with `bun tools/lib/overlay_call_targets.ts
 * resource_3cd 04b0` (an overlay `bl` stores target offset - 2).  Per-target
 * multiset, 12 distinct targets:
 *   Func_030003ac 3   Func_080000c0 3   Func_08015010 1   Func_08015018 2
 *   Func_08015078 1   Func_08015080 2   Func_08015098 2   Func_080150a8 1
 *   Func_08015270 2   Func_08015278 1   Func_08077080 2   Func_080f9010 8
 * The six d-pad arms are behaviourally identical apart from the step and are
 * deliberately NOT merged — folding them would deflate Func_080f9010 by five.
 *
 * High registers carry real state and were recovered from predecessor blocks:
 * r6 is the counter, r7 the window handle, sl the redraw flag, r8 the step
 * DIRECTION (an s8: 0, +1 or -1; the code tests it as `r8 << 24` compared
 * against -1 and against 0x01000000, i.e. a sign-extended byte).  Note that
 * r8 is the direction here where the resource_3ce twin uses it for the redraw
 * flag — the roles of r8 and sl are swapped between the two, which is exactly
 * the lifetime conflation to avoid.  r5 is reused for the masked counter and
 * for the 0x03001b04 pointer.
 *
 * THE AVAILABILITY SEARCHES.  After the d-pad arms, if the direction is -1 the
 * owner walks the counter DOWN and if it is +1 walks it UP until
 * `Func_08077080(counter & 0x3fff)` returns a record whose byte at +4 is
 * non-zero.  Each search re-enters at its `Func_030003ac` call, so the first
 * pass adds 270 (a pure wrap of the value the d-pad arm just produced) and
 * every later pass adds 269 or 271 — that is `counter -/+ 1` modulo 270
 * written as an addition, because the helper only takes a non-negative
 * dividend.  Reading the second and later steps as another plain wrap loses
 * the search entirely.  Both searches are written as `for (;;)` with the
 * modulo at the top, matching the `b.n` INTO the body that the listing shows.
 *
 * DEFECT PRESERVED — the preview window is closed but never opened.  r9 is
 * saved by the prologue, restored by the epilogue, and read exactly once, at
 * 0x0200059a, where it is handed to Func_08015018 as a window handle; nothing
 * anywhere in the owner writes it.  The resource_3ce twin has TWO
 * Func_08015010 calls and closes both handles; this copy has one, and the
 * second close survived the edit.  So `preview` below is deliberately
 * uninitialised: that is what the reference does, the call site is real and is
 * counted in the multiset, and "fixing" it would change behaviour.  It is not
 * live caller state — the owner is otherwise frame-balanced and self-contained
 * — so this is an original-source defect, not a reason to skip the row.
 *
 * Uncertainties: 0x03001c94 and 0x03001b04 are the two IWRAM input words (a
 * held mask and an edge mask, from how they are tested), as in the twin.
 * 0x0200890c and 0x02008914 are even in-image pool words, so DATA at file
 * offsets 0x090c and 0x0914, and both are first arguments of Func_08015098 —
 * label bitmaps.  0x333 and 0x53a are id bases added to the masked counter,
 * and 0x70 / 0x71 / 0x6f are Func_080f9010 cue ids; all three readings come
 * from the twin.  Arities are taken from the registers and stack slots written
 * before each site.
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
void Func_080f9010();

/* Used for their return values. */
s32 Func_030003ac();            /* relocated IWRAM modulo helper */
s32 Func_08015010();
u8 *Func_08077080();            /* entry record by id */

/* In-image label bitmaps. */
extern u8 Data_0200890c[];
extern u8 Data_02008914[];

void Func_020004b0(void)
{
    u32 *dma3 = (u32 *)0x040000d4;
    s32 *held = (s32 *)0x03001c94;
    s32 *pressed = (s32 *)0x03001b04;
    s32 window;                 /* r7 */
    s32 preview;                /* r9 — see the defect note above */
    s32 counter;                /* r6 */
    s32 redraw;                 /* sl */
    s32 direction;              /* r8, an s8 */
    s32 masked;                 /* r5 */

    direction = 0;
    Func_080f9010(0x70);

    window = Func_08015010(0, 0, 30, 12, 2);

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

loop:
    {
        if (redraw != 0) {
            redraw = 0;
            counter = Func_030003ac(counter + 270, 270);    /* 135 << 1 */

            Func_08015270(window);
            Func_08015278(window);
            Func_08015098(Data_0200890c, window, 0, 0);
            Func_080150a8(counter, 0, window, 0x50, redraw);

            Func_08015098(Data_02008914, window, 0, 72);
            masked = counter & 0x3fff;
            Func_08015080(masked + 0x333, window, 120, 0);
            masked += 0x53a;
            Func_08015080(masked, window, 0, 24);
            Func_08015078(masked, window, 0, 48);
        }

        if ((*held & 2) == 0) {
            goto interact;
        }

close_panel:
        Func_080f9010(0x71);
        Func_08015270(window);
        Func_080000c0(1);
        Func_08015018(window, 1);
        Func_08015018(preview, 1);
        return;

interact:
        if ((*pressed & 0x40) != 0) {
            counter -= 1;
            direction = -1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x80) != 0) {
            counter += 1;
            direction = 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x10) != 0) {
            counter += 10;
            direction = 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x20) != 0) {
            counter -= 10;
            direction = -1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x100) != 0) {          /* 128 << 1 */
            counter += 30;
            direction = 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x200) != 0) {          /* 128 << 2 */
            counter -= 30;
            direction = -1;
            redraw = 1;
            Func_080f9010(0x6f);
        }

        if ((s32)(s8)direction == -1) {
            s32 value = counter + 270;

            for (;;) {
                counter = Func_030003ac(value, 270);
                if (Func_08077080(counter & 0x3fff)[4] != 0) {
                    break;
                }
                value = counter + 269;          /* one step DOWN, modulo 270 */
            }
        }

        if ((u8)direction == 1) {
            s32 value = counter + 270;

            for (;;) {
                counter = Func_030003ac(value, 270);
                if (Func_08077080(counter & 0x3fff)[4] != 0) {
                    break;
                }
                value = counter + 271;          /* one step UP, modulo 270 */
            }
        }

        direction = 0;
        goto scheduler;
    }

scheduler:
    Func_080000c0(1);
    goto loop;
}
