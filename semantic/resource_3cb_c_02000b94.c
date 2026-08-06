#include "types.h"

/*
 * Resource 3cb link-session dialogue at 0x02000b94 (468 bytes,
 * 0x02000b94-0x02000d67).
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue at 0x02000b94 and the
 * matching interworking return `pop {r5, r6, r7} / pop {r1} / bx r1` at
 * 0x02000d40, which every path reaches through the shared Func_0808a020 call
 * at 0x02000d3c.  The popped return address goes to r1, so r0 survives and IS
 * the result — the value returned by that call.
 *
 * The 32 bytes at 0x02000d48-0x02000d67 are this owner's literal pool
 * (0x00002930, 0x02000240, 0x00000173, 0x00000202, 0x000002aa, 0x0000293d,
 * 0x00000201, 0x00000205), with two alignment bytes at 0x02000d46.  Execution
 * reaches `bx r1` at 0x02000d44 and never falls into them; the `cmp`/`lsls`
 * halfwords the disassembler prints there are data.
 *
 * All 51 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=49, which is a lower bound; the placed multiset is Func_0808a170 x9,
 * Func_080770c8 x9, Func_080770c0 x7, Func_0808a178 x6, Func_080770d0 x5,
 * Func_02000128 x4, Func_0200008c x3, Func_0808a070 x2, and one each of
 * Func_0808a018, Func_0808a148, Func_080000c0, Func_080770e8, Func_0808a020
 * and Func_0200003c.
 *
 * Func_02000128 is reported `unknown` because it has no `push` opening; it is
 * a genuine prologue-less leaf of this overlay ending in `bx lr`, and the four
 * sites are ordinary calls.  Func_0200003c and Func_0200008c are byte-exact /
 * converted owners of this overlay respectively.
 *
 * Link base: resource_3cb is linked at 0x02008000 (handler table at the tail
 * of assets/code/resource_3cb_overlay.s: 0x02009051 = Func_02001050 + 1, which
 * is byte-exact in assets/code).  0x02000240 is therefore below the image and
 * is the ordinary RAM record the byte-exact
 * assets/code/resource_3cb_c_02000060.c also reads.  This owner uses the word
 * at offset 500 (250 << 1) and clears the halfword at offset 682 (0x2aa).
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear (polarity forced by Func_020002d8's one-shot timer).
 *
 * Behaviour: the whole conversation for the link feature, addressed to
 * speaker 8, with cue ids drawn from the base 0x2930 kept in r6.
 *  - If the local unit does not agree with slot 0 (Func_0200008c(0) == 0) the
 *    owner waits a tick and, if it still does not agree, installs table 5,
 *    runs the interrupt-masked Func_0200003c probe, and — unless flag 0x173
 *    is already up — offers cue 0x2935.  A zero answer from Func_0808a070
 *    commits the session: it publishes 0 through Func_080770e8(1000, 0), sets
 *    0x173 and 0x202, drops 370 and 364, clears the record halfword at +682
 *    and reports cue 0x2937.  A nonzero answer backs out: drop 0x173, set 364,
 *    reinstall table 0, cue 0x2936.
 *
 * The shared tail is preserved as written: the assembly has ONE Func_0808a170
 * site at 0x02000c76 that four paths reach with different cue ids in r0, and
 * ONE Func_0808a178 site at 0x02000c7a that five paths reach.  Duplicating
 * them into each arm would put phantom calls into the multiset, so they are
 * spelled as the `emit`/`closeLine` joins below.  With that, the placed
 * multiset matches the assembly's exactly: Func_0808a170 x9, Func_0808a178 x6,
 * Func_080770c8 x9, Func_080770c0 x7, Func_080770d0 x5, Func_02000128 x4,
 * Func_0200008c x3, Func_0808a070 x2, and one each of the rest.
 *
 *  - If it does agree, an already-open session (flag 0x173) is torn down with
 *    cue 0x293d and both 0x202 and 0x173 dropped.  With flag 0x202 still set
 *    the owner reports cue 0x2933 and stops.
 *  - Otherwise it is the main prompt.  With neither 0x201 nor 768 set it just
 *    plays cue 0x2930 and raises 768.  Otherwise it raises 768, asks with cue
 *    0x2932 or 0x2931 depending on 0x201, and reads the answer: nonzero
 *    repeats cue 0x2930; zero with slot 0 agreeing sets 364 and 370, replies
 *    with cue 0x2933 or 0x2934 depending on 0x201, installs table 1 and sets
 *    0x202; zero without agreement just raises flag 0x205.
 *
 * Uncertainties: the meanings of the flag words (0x173, 0x201, 0x202, 0x205,
 * 364, 370, 768) and of the record halfword at +682 are not established, only
 * their read/write pairings.  Func_0808a070(0, 0) is the yes/no prompt — its
 * result is used as a boolean at both sites — but its two zero arguments are
 * uninterpreted.  Func_0808a018 and Func_0808a020 are reached with no
 * argument register set by this owner.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_0200003c();
s32 Func_0200008c();
void Func_02000128();
void Func_080000c0();
s32 Func_080770c0();
s32 Func_080770c8();
void Func_080770d0();
void Func_080770e8();
void Func_0808a018();
s32 Func_0808a020();
s32 Func_0808a070();
void Func_0808a148();
s32 Func_0808a170();
s32 Func_0808a178();

/* Used for their return values. */

s32 Func_02000b94(void)
{
    u32 cue;
    u32 reply;

    Func_0808a018();
    Func_0808a148(8, *(u32 *)&Data_02000240[500], 0);

    if (Func_0200008c(0) == 0) {
        Func_080000c0(1);
    }

    if (Func_0200008c(0) == 0) {
        Func_02000128(5);
        Func_0200003c();

        if (Func_080770c0(0x173) != 0) {
            cue = 0x2930;
            reply = cue + 3;
            goto emit;
        }

        Func_0808a170(cue + 5);
        Func_0808a178(8, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_080770e8(1000, 0);
            Func_080770c8(0x173);
            Func_080770d0(370);
            Func_080770d0(364);
            Func_080770c8(0x202);
            *(u16 *)&Data_02000240[682] = 0;
            reply = cue + 7;
        } else {
            Func_080770d0(0x173);
            Func_080770c8(364);
            Func_02000128(0);
            reply = cue + 6;
        }
        goto emit;
    }

    if (Func_080770c0(0x173) != 0) {
        Func_02000128(0);
        Func_0808a170(0x293d);
        Func_0808a178(8, 0);
        Func_080770d0(0x202);
        Func_080770d0(0x173);
    }

    if (Func_080770c0(0x202) != 0) {
        reply = cue + 3;
        goto emit;
    }

    if (Func_0808a170(0x201) == 0 && Func_0808a178(768) == 0) {
        Func_080770c0(cue);
        Func_080770c0(8, 0);
        Func_0808a170(768);
        goto done;
    }

    Func_0808a178(768);
    if (Func_080770c8(0x201) != 0) {
        Func_080770c8(cue + 2);
    } else {
        Func_080770c0(cue + 1);
    }
    Func_0808a170(8, 0);

    if (Func_0808a170(0, 0) != 0) {
        Func_0808a178(cue);
        Func_0808a070(8, 0);
        goto done;
    }

    if (Func_0200008c(0) == 0) {
        Func_080770c8(0x205);
        goto done;
    }

    Func_080770c8(364);
    Func_080770c0(370);
    if (Func_0808a170(0x201) != 0) {
        Func_0808a170(cue + 3);
    } else {
        Func_02000128(cue + 4);
    }
    Func_080770c8(1);
    Func_080770c8(0x202);
    goto closeLine;

emit:
    Func_0808a170(reply);

closeLine:
    Func_0808a178(8, 0);

done:
    return Func_0808a020();
}
