#include "types.h"

/*
 * Resource 3cb dialogue step at 0x0200106c (124 bytes,
 * 0x0200106c-0x020010e7).
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue at 0x0200106c and the
 * matching interworking return `pop {r5, r6, r7} / pop {r1} / bx r1` at
 * 0x020010ce.  The popped return address goes to r1, so r0 survives and IS the
 * result — the value returned by the final Func_0808a020 call.
 *
 * The twenty bytes at 0x020010d4-0x020010e7 are this owner's literal pool
 * (0x00002985, 0x0000297f, 0x00002982, 0x02000240, 0x00000305).  Execution
 * reaches `bx r1` at 0x020010d2 and never falls into them; the three `cmp`
 * halfwords the disassembler prints there are cue-base ids, not code.
 *
 * All seven call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=6, which is the distinct-target count and a lower bound; the placed
 * multiset is:
 *   0x02001072 -> veneer 0x02001844 -> Func_0808a018
 *   0x0200109a -> veneer 0x0200187c -> Func_0808a148
 *   0x020010a2 -> veneer 0x0200180c -> Func_080770c0
 *   0x020010ac -> veneer 0x0200180c -> Func_080770c0
 *   0x020010be -> veneer 0x02001884 -> Func_0808a170
 *   0x020010c6 -> veneer 0x0200188c -> Func_0808a178
 *   0x020010ca -> veneer 0x0200184c -> Func_0808a020
 *
 * Same family as Func_020010e8 and Func_02001158: barrier, publish the speaker
 * and the RAM record's word field at offset 500 (250 << 1) through
 * Func_0808a148, select a cue, emit it, close with Func_0808a178(speaker, 0)
 * and Func_0808a020.  resource_3cb is linked at 0x02008000, so the pool word
 * 0x02000240 is below the image and is the same RAM record the byte-exact
 * assets/code/resource_3cb_c_02000060.c reads.
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear (polarity forced by Func_020002d8's one-shot timer).
 * This owner only tests.
 *
 * Behaviour: the cue is a base chosen by the speaker id — 0x2985 for 12,
 * 0x297f for 13, 0x2982 for anything else — plus a variant offset.  The
 * variant is 0 unless flag 772 (193 << 2) is set, in which case it is 1 when
 * flag 0x305 is also set and 2 when it is not.
 *
 * Faithfulness note: the assembly computes the "flag 0x305 is nonzero" test
 * branchlessly as `((-x) | x) >> 31` at 0x020010b0-0x020010b4 and then forms
 * `2 - that`.  That is the standard logical-negation-free idiom for
 * `x != 0 ? 1 : 0` (the sign bit of `-x | x` is set for every nonzero x,
 * including 0x80000000), and it is written as an ordinary comparison here.
 *
 * Uncertainties: the third argument 0 to Func_0808a148 is literal at every
 * site in this overlay and its meaning is not established; the speaker
 * comparisons at 0x02001076-0x02001080 are signed; Func_0808a018 and
 * Func_0808a020 are reached with no argument register set by this owner.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_020028b8();
void Func_02002918();
s32 Func_020028b0();
s32 Func_020028ba();
void Func_02002944();
void Func_02002954();
s32 Func_02002918_b();

                    

                     

/* Used for their return values. */

s32 Func_0200106c(s32 speaker)
{
    u32 cueBase;
    s32 variant = 0;

    Func_020028b8();

    if (speaker == 13) {
        cueBase = 0x297f;
    } else if (speaker > 13) {
        cueBase = 0x2982;
    } else if (speaker == 12) {
        cueBase = 0x2985;
    } else {
        cueBase = 0x2982;
    }

    Func_02002918(speaker, *(u32 *)&Data_02000240[500], 0);

    if (Func_020028b0(772) != 0) {
        variant = (Func_020028ba(0x305) != 0) ? 1 : 2;
    }

    Func_02002944(cueBase + (u32)variant);
    Func_02002954(speaker, 0);
    return Func_02002918_b();
}
