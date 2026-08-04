#include "types.h"

/*
 * Resource 3ca per-frame actor step at 0x02000100 (104 bytes,
 * 0x02000100-0x02000167).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000100, interworking return
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02000158.  The popped register is r0,
 * so it holds the return address and the owner is `void`.  The tail of the row
 * is padding and this owner's literal pool, both data:
 *
 *   0x0200015e  0x0000       alignment
 *   0x02000160  0x0000cccc   the per-frame drift added to +12
 *   0x02000164  0x020094c4   in-image address, file offset 0x14c4
 *
 * All four call sites resolved with tools/overlay_call_targets.ts (the
 * disassembler's own `bl` annotations are wrong for overlays by construction):
 *
 *   0x02000116 -> veneer 0x02001334 -> Func_080000f8
 *   0x0200011c -> veneer 0x02001334 -> Func_080000f8
 *   0x02000144 -> veneer 0x0200136c -> Func_08009080
 *   0x02000154 -> veneer 0x02001374 -> Func_08009098
 *
 * Placed calls per target: Func_080000f8 x2, Func_08009080 x1, Func_08009098
 * x1 — matching the four resolved sites exactly.
 *
 * Func_080000f8 is entry 7 of the ROM's fixed dispatch stub table and is read
 * across this project as the image's random source; it takes no arguments.
 * Note that r0 is left holding the first call's result when the second is
 * made.  On the random-source reading that register is not an argument, and
 * the two results are simply independent draws — this file follows that
 * reading, which the rest of the semantic tree already uses.
 */

/* Record laid out by the caller; only the fields this owner touches are
 * modelled.  The two halfword counters at +100 and +102 are read signed for
 * their tests and written as unsigned halfwords. */
typedef struct {
    unsigned char filler0[8];
    s32 drift8;             /* +0x08 */
    s32 drift12;            /* +0x0c */
    unsigned char filler16[78];
    u16 restart_delay;      /* +0x5e = 94 */
    unsigned char filler96[4];
    u16 countdown;          /* +0x64 = 100 */
    u16 pending;            /* +0x66 = 102 */
} Actor_02000100;

extern s16 Data_020094c4[]; /* in-image script/table handed to Func_08009098 */

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations: arity varies between call sites of one import elsewhere in
 * this overlay. */
s32 Func_080000f8();   /* ROM dispatch[7] -> random source, no arguments */
void Func_08009080();
void Func_08009098();

void Func_02000100(Actor_02000100 *actor)
{
    s32 first;

    if ((s16)actor->countdown != 0) {
        /* Still drifting: burn one frame off the countdown and nudge the two
         * 16.16 accumulators. */
        actor->countdown = (u16)(actor->countdown - 1);

        first = Func_080000f8();
        actor->drift8 += first - Func_080000f8();
        actor->drift12 += 0xcccc;
        return;
    }

    /* Countdown exhausted.  Nothing happens unless a restart is pending. */
    if ((s16)actor->pending == 0) return;

    /*
     * The value stored back into +102 is r1, which is the +100 halfword this
     * arm already tested as zero — so the pending flag is cleared with a zero
     * that came from the other counter rather than an immediate.  Written as
     * the constant it provably is.
     */
    actor->pending = 0;
    Func_08009080(actor, 1);
    actor->restart_delay = 20;
    Func_08009098(actor, Data_020094c4);
}
