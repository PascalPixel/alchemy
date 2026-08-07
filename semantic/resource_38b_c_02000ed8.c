#include "types.h"

/*
 * Resource 38b per-frame effect task at 0x02000ed8 (100 bytes, 2 distinct
 * callees).
 *
 * Complete owner: `push {r5, lr}` at 0x02000ed8, interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000f28 — r0 is the popped return
 * address, so the owner is `void`.  0x02000f2e-0x02000f3b is the literal
 * pool (three zero halfwords of alignment/padding followed by two pointer
 * words); it is branched past, not executed.
 *
 * This is the task installed by 0x02000890, which passes the pool word
 * 0x02008ed9 to Func_080000d0.  Under the proven 0x02008000 link base that is
 * this function's entry plus the Thumb bit — which is simultaneously the
 * cheapest witness for the base and the proof of what this owner is.
 *
 * Two callees: the import veneer Func_0808a080 and this overlay's own
 * prologue at 0x02000f3c, both resolved with tools/lib/overlay_call_targets.ts.
 * The 0x02000f3c site is an ordinary call: the target is a real `push`
 * prologue, and r0 (the actor record) is live across it as that owner's
 * parameter.
 *
 * The counter at Data_02009d88 lives in the overlay's own image (file offset
 * 0x1d88).  Overlay data is writable EWRAM used as scene state, so it is not
 * const, and it is read both as s16 (for the zero test) and as u16 (for the
 * increment) by the original code.
 */

/* Cross-overlay scene-state halfword array; the word at byte offset 500 is a
 * slot handle, read 32 bits wide.  The array is s16 but this access is not,
 * so it is spelled through a byte pointer. */
extern s16 Data_02000240[];

/* Slot accessor. */
extern u8 *Func_0808a080();

/* This overlay's effect spawner. */
extern void Func_02000f3c(u8 *actor);

/* u16 beat counter in the overlay's own image at file offset 0x1d88. */
extern u16 Data_02009d88;

void Func_02000ed8(void)
{
    u8 *actor;
    u16 count;

    /* 250 << 1 = 500. */
    actor = Func_0808a080(*(s32 *)((u8 *)Data_02000240 + 500));

    /* Signed compares on 16.16 coordinates.  142 << 16 = 0x008e0000. */
    if (*(s32 *)(actor + 0x08) >= 0x008e0000) {
        return;
    }

    /* 128 << 12 = 0x00080000. */
    if (*(s32 *)(actor + 0x0c) >= 0x00080000) {
        /* Above the height threshold the effect is suppressed and the beat
         * counter is rearmed. */
        Data_02009d88 = 0;
        return;
    }

    /* Spawn on the beat, then advance.  The reload of the counter after the
     * call is deliberate in the original: Func_02000f3c may itself have
     * touched it, so the increment uses the post-call value. */
    if ((s16)Data_02009d88 == 0) {
        Func_02000f3c(actor);
    }
    Data_02009d88 = count;
    count = (u16)(Data_02009d88 + 1);

    /* 240 << 13 = 0x001e0000, compared against count << 16 — i.e. the
     * halfword test count == 30.  Wraps the 30-frame effect period. */
    if (count == 30) {
        Data_02009d88 = 0;
    }
}
