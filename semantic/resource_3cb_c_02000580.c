#include "types.h"

/*
 * Resource 3cb link send stage at 0x02000580 (560 bytes,
 * 0x02000580-0x020007af).
 *
 * Complete owner: the `push {r5, r6, r7, lr}` + high-register save +
 * `sub sp, #32` prologue at 0x02000580 and the matching unwind at
 * 0x02000796-0x020007a6, ending `pop {r1} / bx r1`.  The popped return address
 * goes to r1, so r0 survives and IS the result; `ldr r0, [sp, #4]` at
 * 0x02000794 loads the status the body accumulated in that stack slot.
 *
 * The eight bytes at 0x020007a8-0x020007af are this owner's literal pool
 * (0x03001388, 0x03001f64).  Execution reaches `bx r1` at 0x020007a6 and never
 * falls into them; the `asrs`/`subs`/`lsls` halfwords the disassembler prints
 * there are data.  Func_020007b0 begins immediately after.
 *
 * All 22 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=21, a lower bound.  Placed multiset: Func_080000c0 x7,
 * Func_08000380 x3, Func_080003a8 x3, Func_08000170 x2, Func_08000178 x2,
 * two indirect calls through this overlay's `call_via` bank, and one each of
 * Func_08077000, Func_08077008 and Func_0200053c.
 *
 * The two `bl 0x020018f8` sites at 0x020005e6 and 0x020006ce are genuine
 * indirect calls: this overlay carries its own thunk bank at 0x020018f0+
 * (`bx rN / nop` pairs, 0x020018f8 being `bx r3`), and r3 is loaded from the
 * pool word 0x03001388, an IWRAM address.  The branch goes to that address,
 * so 0x03001388 is relocated IWRAM code, not a pointer cell, and r3 is a live
 * target rather than a fourth argument.  Its arguments are r0 (the session
 * handle), r1 (a block from Func_08077008/Func_08077000) and r2 (the size).
 *
 * Link base: resource_3cb is linked at 0x02008000 (handler table at the tail
 * of assets/code/resource_3cb_overlay.s: 0x02009051 = Func_02001050 + 1, which
 * is byte-exact in assets/code).  Both pool addresses are below the image:
 * 0x03001f64 is the IWRAM link-state halfword whose low two bits Func_0200008c
 * also tests, and 0x03001388 is the relocated helper described above.
 *
 * Behaviour: the send counterpart of Func_02000398.  Stage one sends exactly
 * three 340-byte blocks.  The roster ids come from Func_0200053c into a stack
 * buffer; for each id the matching block is fetched with Func_08077008,
 * serialised into the session by the relocated helper, tagged at +298 with 2,
 * and its id recorded in an eight-byte scratch map as `slot - 128` (so every
 * present id maps to a nonzero byte).  Fewer than three ids are padded out
 * with blocks tagged 0.  Every send is followed by a poll loop on
 * Func_080003a8 with the same budget/stall discipline as Func_02000398: one
 * 600-tick budget for the whole stage and at most 24 further ticks once the
 * budget is spent or the link drops.
 *
 * Stage two closes the session, reopens it at 320 bytes, serialises the block
 * from Func_08077000(0), and then compacts that block's record list in place:
 * each record's byte at +2 is remapped through the scratch map, and a record
 * whose byte maps to 0 — meaning its owner is not in the roster this reconstruction
 * sent — is removed by shifting the remaining 4-byte records down and
 * decrementing the count at +264.  The compacted block is then sent with the
 * same poll discipline.
 *
 * Any failure — a Func_08000380 rejection or a stall overrun — stores -1 in
 * the status slot and jumps to the shared close.  A successful Func_08000380
 * return is NOT written to that slot, which is why the return value is kept
 * separate from the status here.  Success leaves the initial
 * 0 in place, so the owner returns 0 or -1.
 *
 * Uncertainties: the identities of the 340- and 320-byte blocks, of the tag
 * byte at +298 and of the record layout beyond "4 bytes per record, selector
 * at +2, count at +264" are not established; the 0x03001388 helper is called
 * as f(handle, block, size) but is not identified.  The scratch map is only
 * eight bytes, so it assumes roster ids below 8 — nothing here bounds them.
 */

/* Old-style declarations: arities vary per site across this overlay. */
void Func_080000c0();
void Func_08000178();
void Func_03001388();

/* Used for their return values. */
s32 Func_08000170();
s32 Func_08000380();
s32 Func_080003a8();
s32 Func_0200053c();
u8 *Func_08077008();
u8 *Func_08077000();

s32 Func_02000580(void)
{
    volatile u16 *linkState = (volatile u16 *)0x03001f64;
    u16 ids[8];
    u8 scratch[8];
    s32 status = 0;
    s32 rc;
    s32 handle;
    s32 budget = 600;
    s32 stalls;
    s32 present;
    s32 slot;
    s32 sent;
    s32 i;
    u8 *block;

    handle = Func_08000170(340);
    present = Func_0200053c(ids);

    for (i = 7; i >= 0; i--) {
        scratch[i] = 0;
    }

    for (slot = 0; slot < present; slot++) {
        block = Func_08077008(ids[slot]);
        Func_03001388(handle, block, 340);
        *(u8 *)((u32)handle + 298) = 2;
        scratch[ids[slot]] = (u8)(slot - 128);

        rc = Func_08000380(handle, 340);
        stalls = 0;
        if (rc == -1) {
            status = rc;
            goto close;
        }

        /* The machine enters each polling loop at its bottom test. */
        goto testRosterSend;
waitRosterSend:
        budget--;
        Func_080000c0(1);
        if (budget < 0 || (*linkState & 3) != 3) {
            stalls++;
            if (stalls > 24) {
                goto fail;
            }
        }
testRosterSend:
        if (Func_080003a8() != 0) {
            goto waitRosterSend;
        }
        Func_080000c0(2);
    }

    sent = present;
    /* Padding is sent from the block below the shared polling test. */
    goto sendPadding;
waitPadding:
    budget--;
    Func_080000c0(1);
    if (budget < 0 || (*linkState & 3) != 3) {
        stalls++;
        if (stalls > 24) {
            goto fail;
        }
    }
testPadding:
    if (Func_080003a8() != 0) {
        goto waitPadding;
    }
    Func_080000c0(2);
    sent++;

sendPadding:
    if (sent <= 2) {
        *(u8 *)((u32)handle + 298) = 0;
        stalls = 0;
        rc = Func_08000380(handle, 340);
        if (rc == -1) {
            status = rc;
            goto close;
        }
        goto testPadding;
    }

    Func_08000178(handle);
    handle = Func_08000170(320);
    block = Func_08077000(0);
    Func_03001388(handle, block, 320);

    {
        s32 *count = (s32 *)((u32)handle + 264);
        u8 *records = (u8 *)((u32)handle + 8);
        s32 index = 0;

        while (index < *count) {
            u8 *record = records + index * 4;
            u8 mapped = scratch[record[2]];

            record[2] = mapped;
            if (mapped == 0) {
                s32 move;

                for (move = index; move < *count - 1; move++) {
                    *(u32 *)(records + move * 4) =
                        *(u32 *)(records + move * 4 + 4);
                }
                *count = *count - 1;
                index--;
            }
            index++;
        }
    }

    budget = 600;
    stalls = 0;
    rc = Func_08000380(handle, 320);
    if (rc == -1) {
        status = rc;
        goto close;
    }

    goto testFinalSend;
waitFinalSend:
    budget--;
    Func_080000c0(1);
    if (budget < 0 || (*linkState & 3) != 3) {
        stalls++;
        if (stalls > 24) {
            goto fail;
        }
    }
testFinalSend:
    if (Func_080003a8() != 0) {
        goto waitFinalSend;
    }
    Func_080000c0(1);
    Func_080000c0(2);
    goto close;

fail:
    status = -1;

close:
    Func_08000178(handle);
    return status;
}
