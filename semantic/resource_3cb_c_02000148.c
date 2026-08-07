#include "types.h"

/*
 * Resource 3cb link-session state machine at 0x02000148 (400 bytes,
 * 0x02000148-0x020002d7).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 /
 * push {r6, r7}` prologue at 0x02000148 and the matching `mov r0, sl /
 * pop {r3, r5} / mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} / pop {r1} /
 * bx r1` interworking return at 0x020002a6.  The popped return address goes to
 * r1, so r0 survives and IS the result — the status kept in sl, which is 1
 * except on the one path that clears it.
 *
 * The 36 bytes at 0x020002b4-0x020002d7 are this owner's literal pool
 * (0x03001ebc, 0x00000303, 0x02009f4c, 0x03000164, 0x02002024, 0x00000201,
 * 0x00000202, 0x00000173, 0x00000205).  Execution reaches `bx r1` at
 * 0x020002b2 and never falls into them; the `lsls`/`subs`/`ldr` halfwords the
 * disassembler prints there are data.  Func_020002d8 begins immediately after.
 *
 * All 24 call sites were resolved with tools/lib/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=24.  Placed multiset: Func_080770c0 x9, Func_080770d0 x4,
 * Func_080770c8 x2, Func_0808a080 x1, Func_0200008c x5, Func_02000128 x2, and
 * one indirect call through this overlay's own `call_via` bank.
 *
 * Two classifications need explanation:
 *  - 0x02000128 is reported `unknown` because it has no `push` opening.  It is
 *    a genuine prologue-less leaf of this overlay ending in `bx lr`; an
 *    ordinary call.
 *  - 0x02001908 IS a real `call_via`: this overlay carries its own thunk bank
 *    at 0x020018f0+ (`bx rN / nop` pairs, 0x02001908 being `bx r7`), and r7 is
 *    loaded from the pool word 0x03000164 — squarely in the IWRAM
 *    0x030001xx relocated-helper band, which is the discriminator HANDOVER §0
 *    gives.  So the site at 0x020001a8 calls the relocated helper in r7 with
 *    r0 and r1 as its arguments, and r7 is a live target, not scratch.
 *
 * Link base: resource_3cb is linked at 0x02008000 (handler table at the tail
 * of assets/code/resource_3cb_overlay.s: 0x02009051 = Func_02001050 + 1, which
 * is byte-exact in assets/code).  So 0x02009f4c is IN-IMAGE data at file
 * offset 0x1f4c — a writable counter in the overlay's own EWRAM image, the
 * neighbour of the 0x02009f50 counter Func_020002d8 advances.  0x03001ebc is
 * below the image: it is the IWRAM workspace-pointer cell the rest of the
 * overlay loads directly, and 0x02002024 is the RAM slot table
 * Func_0200008c indexes with the same 24-byte stride (this owner clears four
 * of those records, so the table has at least four).
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear (polarity forced by Func_020002d8's one-shot timer).
 *
 * Behaviour: one per-frame step of the link session.  It first drops flag 772
 * when the resource block Func_0808a080(0) reports at +16 exceeds 0x00e00000.
 * Phase 2 (workspace halfword at +386 equal to 2) skips straight to the
 * teardown checks.  Otherwise, while flag 0x303 is clear it runs a 25-step
 * countdown; on overrun it clears the four slot records through the relocated
 * IWRAM helper, resets the counter and re-installs table 4.  With the counter
 * back at zero it polls the three slots through Func_0200008c: slot 0 plus
 * either slot 1 or slot 2 agreeing raises flag 0x201 and, if flag 0x202 is
 * also up, advances the workspace phase to 1; no agreement drops flag 0x201
 * and makes the owner report 0.  Finally, phase 2 is entered when the session
 * has run past step 24 with slot 0 no longer agreeing, which sets flag 0x205,
 * drops 0x201/0x202 and re-installs table 4; and flag 0x205 alone is enough to
 * force phase 2 on any later frame.
 *
 * Uncertainties: the meanings of the individual flag words (0x201, 0x202,
 * 0x205, 0x303, 0x173, 512, 772) are not established, only their read/write
 * pairings; the +16 field of the Func_0808a080(0) block and the 0x00e00000
 * threshold are likewise uninterpreted (the comparison is signed).  The
 * relocated helper in r7 is called as f(record, 20) with a 24-byte stride, so
 * it clears or initialises the first 20 bytes of each record, but its identity
 * is not pinned down here.  The Func_0200008c(0) result at 0x02000182 is
 * discarded; it is kept because Func_0200008c has side effects on flags 0x302
 * and 0x303.
 */

/* Old-style declarations: arities vary per site across this overlay. */
void Func_080770c8();
void Func_080770d0();
void Func_02000128();
void Func_03000164();

/* Used for their return values. */
s32 Func_080770c0();
s32 Func_0200008c();
s32 *Func_0808a080();

s32 Func_02000148(void)
{
    u16 *workspace = *(u16 **)0x03001ebc;
    s16 *phase = (s16 *)((u32)workspace + 386);
    u32 *step = (u32 *)0x02009f4c;
    s32 status = 1;
    s32 *resource;

    resource = Func_0808a080(0);
    if (resource[4] > 0x00e00000) {
        Func_080770d0(772);
    }

    if (*phase != 2) {
        Func_0200008c(0);

        if (Func_080770c0(0x303) == 0) {
            *step = *step + 1;
            if ((s32)*step > 25) {
                unsigned char *record = (unsigned char *)0x02002024;
                s32 remaining = 3;

                do {
                    remaining--;
                    Func_03000164(record, 20);
                    record += 24;
                } while (remaining >= 0);

                *step = 0;
                Func_02000128(4);
            }
        } else {
            *step = 0;
        }

        if (*step == 0) {
            if (Func_0200008c(0) != 0 &&
                (Func_0200008c(1) != 0 || Func_0200008c(2) != 0)) {
                Func_080770c8(0x201);
                if (Func_080770c0(0x202) != 0) {
                    *phase = 1;
                }
                status = 1;
            } else {
                Func_080770d0(0x201);
                status = 0;
            }
        }

        if (Func_080770c0(0x201) != 0 &&
            Func_080770c0(0x202) != 0 &&
            Func_080770c0(512) == 0) {
            *phase = 1;
        }
    }

    if (Func_080770c0(0x201) != 0 || Func_080770c0(0x202) != 0) {
        if (Func_080770c0(0x173) == 0 &&
            Func_0200008c(0) == 0 &&
            (s32)*step > 24) {
            *phase = 2;
            Func_080770c8(0x205);
            Func_080770d0(0x201);
            Func_080770d0(0x202);
            Func_02000128(4);
        }
    }

    if (Func_080770c0(0x205) != 0) {
        *phase = 2;
    }

    return status;
}
