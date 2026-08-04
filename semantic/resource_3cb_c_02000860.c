#include "types.h"

/*
 * Resource 3cb link-session runner at 0x02000860 (820 bytes,
 * 0x02000860-0x02000b93).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 /
 * push {r6, r7}` prologue at 0x02000860 and the matching `pop {r3, r5} /
 * mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} / pop {r1} / bx r1` interworking
 * return at 0x02000b88.  The popped return address goes to r1, so r0 survives
 * and IS the result.
 *
 * Three literal pools sit inside the span and are branched over, so their
 * bytes are data and are not decoded as instructions: 0x02000ad4-0x02000b0b
 * (reached only by `ldr rN, [pc, ...]`, jumped over by the `b.n` at
 * 0x02000ad2), 0x02000b62-0x02000b87 (after the `b.n` at 0x02000b60), and the
 * two words at 0x02000b04/0x02000b08 inside the first.  Register state is
 * carried across every one of them: r5 is loaded at 0x02000ac0 from the pool
 * that follows and used at 0x02000ac4, and the whole 0x02000b1e block reads
 * pool words on both sides of the `b.n`.
 *
 * All 69 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=67, a lower bound.  Placed multiset: Func_080770d0 x8,
 * Func_080770c0 x7, Func_080000c0 x5, Func_0808a090 x5, Func_0808a0c8 x5,
 * Func_0808a0e8 x5, Func_0200008c x4, Func_02000128 x4, Func_080770c8 x2,
 * Func_0808a018 x2, Func_08009188 x2, Func_08009190 x2, Func_08000150 x2,
 * Func_0808a260 x2, Func_0808a268 x2, and one each of Func_08000148,
 * Func_080000d0, Func_080000d8, Func_080003b8, Func_08015018, Func_08015038,
 * Func_0808a020, Func_0808a148, Func_0808a170, Func_0808a178, Func_0808a250
 * and Func_020007b0.  Func_02000128 is reported `unknown` only because it has
 * no `push` opening; it is a prologue-less leaf of this overlay ending in
 * `bx lr`, and all four sites are ordinary calls.
 *
 * Link base: resource_3cb is linked at 0x02008000, and this owner is one of
 * the witnesses — the pool word 0x02008149 at 0x02000af0 is exactly
 * Func_02000148 (this overlay's per-frame state machine) plus the Thumb bit,
 * and it is used as the argument of Func_080000d8 (install), Func_080000d0 and
 * Func_080003b8.  So it is an installed callback, not data.  Under the same
 * base 0x02000240 and 0x02002224 are below the image and are ordinary RAM,
 * 0x03001ebc is the IWRAM workspace-pointer cell, and 0x02018000 is the EWRAM
 * staging area the payload is copied to.
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear (polarity forced by Func_020002d8's one-shot timer).
 *
 * Behaviour: the whole link session, driven from a single owner.
 *  - When flag 0x173 is already up the owner skips straight to the transfer.
 *    Otherwise it needs flag 512 up and 0x205 down, or it returns immediately.
 *  - It then sets 0x203, installs table 2, and — unless slot 2 already agrees
 *    — starts cue 0x2928 as a looping sound while it waits for slot 2 to
 *    agree.  The wait aborts when flag 0x201 goes down, flag 0x205 goes up, or
 *    neither slot 2 nor slot 1 has agreed for 26 consecutive ticks.  Aborting
 *    parks the workspace phase at 2, raises 0x205, drops 0x201/0x202/512 and
 *    reinstalls table 4; the `aborted` flag then skips the transfer entirely.
 *  - The transfer itself allocates a 1992-byte buffer, installs
 *    Func_02000148 as the per-frame task, fades the screen through the
 *    Func_0808a090/0c8/0e8 family, and calls Func_020007b0 to run the
 *    handshake.  A negative result tears the whole session down (restore the
 *    display, uninstall, drop every flag, park the phase at 2); a
 *    non-negative result installs the two result panels through
 *    Func_0808a260/Func_0808a268, writes the four-halfword tag "EXEC" at
 *    0x02002224, records 4 in the RAM record at +555, and copies the whole
 *    1992-byte payload to 0x02018000 before releasing the allocation.
 *  - When flag 0x173 was already up the transfer path is skipped and the owner
 *    only plays cue 0x293b to speaker 8 and shows the alternate panels.
 *
 * Faithfulness notes: r5 is a long-lived alias that is REASSIGNED mid-owner —
 * it holds 0x02008149 (the installed callback) from 0x0200096c and is reloaded
 * with 0xbe (the panel id) at 0x02000ac0 and 0x02000b0c.  Its uses are tracked
 * per-site here, not as one variable, which is the trap HANDOVER §0 records.
 * The payload copy at 0x02000b48 increments its counter before the test and
 * compares it to 1991 with an unsigned `bls`, so it runs 1992 times and moves
 * 1992 bytes, matching the allocation size.
 *
 * Uncertainties: the meanings of the flag words (0x173, 0x201, 0x202, 0x203,
 * 0x205, 0x303, 512), of the record byte at +555 and of the 0x02002224 tag are
 * not established; the Func_0808a090/0808a0c8/0808a0e8 argument triples are
 * transcribed as built (0x10000/0x8000, 0x8000/0x4000, 0x1999/0xccc and the
 * 216/200, 216/184, 216/168 pairs) without interpretation.  Func_0808a018 and
 * Func_0808a020 are reached with no argument register set by this owner.  The
 * paths that leave through 0x02000b88 without reaching Func_0808a020 return
 * whatever the last Func_080770c0 left in r0; that is what the epilogue shape
 * says, and no caller evidence contradicts it.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_0808a018();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0e8();
void Func_0808a250();
void Func_0808a260();
void Func_0808a268();
void Func_080770c8();
void Func_080770d0();
void Func_080000c0();
void Func_080000d0();
void Func_080000d8();
void Func_080003b8();
void Func_08000150();
void Func_08009188();
void Func_08009190();
void Func_08015018();
void Func_02000128();

/* Used for their return values. */
s32 Func_080770c0();
s32 Func_0200008c();
s32 Func_08015038();
s32 Func_0808a020();
s32 Func_020007b0();
u8 *Func_08000148();

s32 Func_02000860(void)
{
    u16 *workspace = *(u16 **)0x03001ebc;
    u16 *phase = (u16 *)((u32)workspace + 386);
    s32 aborted = 0;
    s32 cueHandle = 0;
    s32 idle = 0;
    s32 stop;
    s32 gate;
    u8 *payload;
    u16 *tag;
    s32 i;

    if (Func_080770c0(0x173) != 0) {
        Func_0808a018();
        goto transfer;
    }

    gate = Func_080770c0(512);
    if (gate == 0) {
        return gate;
    }
    gate = Func_080770c0(0x205);
    if (gate != 0) {
        return gate;
    }

    /* The abort block physically precedes the normal wait setup. */
    goto beginTransferWait;

abortTransfer:
    *phase = 2;
    Func_080770c8(0x205);
    Func_080770d0(0x201);
    Func_080770d0(0x202);
    Func_02000128(4);
    aborted = 1;
    Func_080770d0(512);
    goto transferWaitComplete;

beginTransferWait:
    Func_0808a018();
    Func_080770c8(0x203);
    Func_02000128(2);

    if (Func_0200008c(2) == 0) {
        cueHandle = Func_08015038(0x2928, 5, 4, 1);
    }

    goto testTransferWait;

waitForTransfer:
    Func_080000c0(1);

    stop = 0;
    if (Func_080770c0(0x201) == 0) {
        stop = 1;
    }
    if (Func_080770c0(0x205) != 0) {
        stop = 1;
    }

    if (Func_0200008c(2) != 0 || Func_0200008c(1) != 0) {
        idle = 0;
    } else {
        idle++;
        if (idle > 25) {
            stop = 1;
        }
    }

    if (stop != 0) {
        goto abortTransfer;
    }

testTransferWait:
    if (Func_0200008c(2) == 0) {
        goto waitForTransfer;
    }

transferWaitComplete:
    if (cueHandle != 0) {
        Func_08015018(cueHandle, 1);
    }
    Func_080000c0(5);

    if (aborted != 0) {
        goto close;
    }

transfer:
    payload = Func_08000148(54, 1992);

    Func_080000d8((void *)0x02008149);
    Func_08009188(5);
    Func_080000c0(8);
    Func_08009190(5);

    if (Func_080770c0(0x173) != 0) {
        Func_0808a148(8, *(u32 *)&Data_02000240[500], 0);
        Func_0808a170(0x293b);
        Func_0808a178(8, 0);
        Func_080000c0(45);

        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0c8(0, 216, 184);
        Func_0808a0e8(0);
        Func_0808a0c8(0, 216, 168);
        Func_0808a0e8(0);
    } else {
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0c8(0, 216, 200);
        Func_0808a0e8(0);

        Func_0808a090(0, 0x1999, 0xccc);
        Func_0808a0c8(0, 216, 168);

        if (Func_020007b0() < 0) {
            Func_0808a090(0, 0x10000, 0x8000);
            Func_0808a0c8(0, 216, 200);
            Func_08009188(5);
            Func_080000c0(8);
            Func_08009190(5);
            Func_0808a0e8(0);
            Func_08000150(54);
            Func_02000128(0);
            Func_02000128(4);
            Func_080000d0((void *)0x02008149, 3200);
            Func_080003b8((void *)0x02008149, 1);
            Func_080770d0(0x201);
            Func_080770d0(0x202);
            Func_080770d0(0x303);
            Func_080770d0(0x203);
            Func_080770d0(512);
            *phase = 2;
            goto close;
        }

        Func_0808a090(0, 0x8000, 0x4000);
        Func_0808a0e8(0);
    }

    if (Func_080770c0(0x173) != 0) {
        Func_0808a260(0xbe, 8);
        Func_0808a268(0xbe, 9);
    } else {
        Func_0808a260(0xbe, 10);
        Func_0808a268(0xbe, 11);
    }

    Data_02000240[0x22b] = 4;
    Func_0808a250(1, 1);

    tag = (u16 *)0x02002224;
    tag[1] = 0x58;
    tag[0] = 0x45;
    tag[2] = 0x45;
    tag[3] = 0x43;

    {
        u8 *dst = (u8 *)0x02018000;
        u8 *src = payload;

        for (i = 0; i < 1992; i++) {
            *dst++ = *src++;
        }
    }

    Func_08000150(54);
close:
    /* Early aborts and the normal path share the overlay's one close site. */
    return Func_0808a020();
}
