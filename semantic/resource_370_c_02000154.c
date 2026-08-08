#include "types.h"

/*
 * Resource 370 per-frame alpha-blend step at 0x02000154.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000154 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x020001d8.  The return address is
 * popped into r0, so the owner is `void` (HANDOVER section 0, epilogue rule).
 * An alignment halfword sits at 0x020001de and the owner's literal pool at
 * 0x020001e0-0x020001fb; neither is reached by the control-flow walk from the
 * prologue.
 *
 * LINK BASE CONFIRMED FOR THIS OVERLAY.  The pool word 0x02008155 is
 * `Func_02000154 + 1` -- this owner's own entry plus the Thumb bit -- which is
 * the cheapest witness from HANDOVER section 0 that resource_370 is linked at
 * 0x02008000.  Consequently the pool word 0x020096b0 is in-image data at file
 * offset 0x16b0 (the overlay image is writable EWRAM, so the frame counter it
 * holds is genuinely stored there), while 0x02002090 and 0x02000240-style
 * addresses, being below 0x02008000, are ordinary RAM globals outside the
 * image.
 *
 * The single call site is 0x020001d4 -> veneer at file offset 0x1334 ->
 * Func_080000d8 (`cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_370`), matching
 * the inventory's calls=1.  Func_080000d8 is the main image's per-frame task
 * entry point that takes a Thumb-tagged callback; here it is handed this
 * owner's own address, which reads as the task retiring itself once the fade
 * completes.  Whether Func_080000d8 registers or removes is not settled from
 * this owner alone, so no name is asserted for it.
 *
 * What the body does: it bumps an in-image u16 frame counter, derives the
 * blend level as `counter >> 1`, and appends two entries to the deferred
 * register-write queue at 0x02002090 -- one setting BLDCNT (0x04000050) to
 * 0x2e51, one setting BLDALPHA (0x04000052) to EVA = level with
 * EVB = 16 - level.  Each append is bracketed by an IME (0x04000208)
 * save/clear/restore, and is skipped entirely once the queue holds 32 entries.
 * Both appends re-read the queue length, so the second sees the first's
 * increment.
 *
 * Uncertainties: the third word of each queue entry is the constant 0x00020000
 * and is assumed to be the entry's timing/flags field; and the queue header's
 * first four bytes hold the length in their low halfword, with the upper
 * halfword untouched by this owner.
 */

struct RegWriteQueue_02000154 {
    u16 length;
    u16 unknown_02;
    struct {
        u32 value;
        u32 destination;
        u32 flags;
    } entries[32];
};

/* Overlay import (via the veneer table).  Old-style declaration: overlay
 * imports vary their argument count between call sites. */
void Func_0200150a();

void Func_02000154(void)
{
    struct RegWriteQueue_02000154 *queue =
        (struct RegWriteQueue_02000154 *)0x02002090;
    u16 *frame_counter = (u16 *)0x020096b0;
    u16 *ime = (u16 *)0x04000208;
    u16 saved_ime;
    u32 level;
    s32 index;

    *frame_counter = (u16)(*frame_counter + 1);
    level = (u32)*frame_counter >> 1;

    saved_ime = *ime;
    /* The address's own low halfword is stored back, which clears IME's
     * enable bit; this is the overlay's standard critical-section idiom. */
    *ime = (u16)(u32)ime;

    index = (s32)queue->length;
    if (index <= 31) {
        queue->length = (u16)(index + 1);
        queue->entries[index].value = 0x00002e51;
        queue->entries[index].destination = 0x04000050;
        queue->entries[index].flags = 0x00020000;
    }

    *ime = saved_ime;

    saved_ime = *ime;
    *ime = (u16)(u32)ime;

    index = (s32)queue->length;
    if (index <= 31) {
        queue->length = (u16)(index + 1);
        queue->entries[index].value = ((16 - (level & 0xffff)) << 8) |
                                      (level & 0xffff);
        queue->entries[index].destination = 0x04000052;
        queue->entries[index].flags = 0x00020000;
    }

    *ime = saved_ime;

    /*
     * The comparison is done on `level << 16` against 0x000f0000 unsigned,
     * which is exactly `level > 15` for the 15-bit level this owner derives.
     */
    if ((level << 16) > 0x000f0000u) {
        /* Func_02000154 + 1: this owner's own entry, Thumb-tagged. */
        Func_0200150a(0x02008155);
    }
}
