typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3cb dialogue step at 0x02000f8c (196 bytes,
 * 0x02000f8c-0x0200104f).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` prologue at
 * 0x02000f8c and the matching `pop {r3} / mov r8, r3 / pop {r5, r6, r7} /
 * pop {r1} / bx r1` interworking return at 0x02001022.  The popped return
 * address goes to r1, so r0 survives and IS the result — the value returned by
 * the final Func_0808a020 call.
 *
 * The 36 bytes at 0x0200102c-0x0200104f are this owner's literal pool
 * (0x0000294e, 0x02000240, 0x00000305, 0x00002967, 0x0000296c, 0x00002971,
 * 0x00002976, 0x00002953, 0x00002958).  Execution reaches `bx r1` at
 * 0x0200102a and never falls into them; the `cmp` halfwords the disassembler
 * prints there are cue-base ids, not code.  Func_02001050 (byte-exact in
 * assets/code) begins immediately after.
 *
 * All eleven call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=10, which is a lower bound counting distinct targets; the placed
 * multiset is:
 *   0x02000f98 -> 0x02000f30  (this overlay, real prologue)
 *   0x02000fa0 -> 0x02000f30  (this overlay, real prologue)
 *   0x02000fa6 -> veneer 0x02001844 -> Func_0808a018
 *   0x02000fb8 -> veneer 0x0200187c -> Func_0808a148
 *   0x02000fc0 -> veneer 0x0200180c -> Func_080770c0
 *   0x02000fcc -> veneer 0x0200180c -> Func_080770c0
 *   0x02000fd6 -> veneer 0x0200180c -> Func_080770c0
 *   0x02000fde -> veneer 0x0200180c -> Func_080770c0
 *   0x02001012 -> veneer 0x02001884 -> Func_0808a170
 *   0x0200101a -> veneer 0x02001894 -> Func_0808a180
 *   0x0200101e -> veneer 0x0200184c -> Func_0808a020
 *
 * Func_02000f30 is this overlay's roster membership test, converted alongside
 * this owner; it is a genuine prologue and an ordinary call.
 *
 * Link base: resource_3cb is linked at 0x02008000 (handler table at the tail
 * of assets/code/resource_3cb_overlay.s: 0x02009051 = Func_02001050 + 1, which
 * is byte-exact in assets/code).  0x02000240 is therefore below the image and
 * is the ordinary RAM record the byte-exact
 * assets/code/resource_3cb_c_02000060.c also reads; the word used here is at
 * offset 500 (built as 250 << 1).
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear (polarity forced by Func_020002d8's one-shot timer).
 * This owner only tests.
 *
 * Behaviour: the same family shape as Func_0200106c and Func_020010e8 —
 * barrier, publish the speaker through Func_0808a148, choose a cue, emit,
 * close — but the cue base is chosen from six alternatives.  With flag 772
 * set, the base depends on flag 0x305 and on the per-speaker flag at
 * 752 + speaker: (0x2967, 0x296c) when 0x305 is set and (0x2971, 0x2976)
 * when it is not, taking the first of each pair when the per-speaker flag is
 * set.  With flag 772 clear the base depends on the roster instead: 0x2958
 * when member 0 is absent, 0x2953 when member 0 is present but the speaker is
 * not, and the preloaded default 0x294e when both are present.  The emitted
 * cue is then `base + speaker - 1`.
 *
 * Uncertainties: the test of flag 752 at 0x02000fcc discards its result — it
 * is issued for whatever side effect Func_080770c0 has and cannot be a dead
 * read at the C level, so it is kept.  The third argument 0 to Func_0808a148
 * and the second argument 0 to Func_0808a180 are literal at every site in this
 * overlay.  Func_0808a018 and Func_0808a020 are reached with no argument
 * register set by this owner.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_0808a018();
void Func_0808a148();
void Func_0808a170();
void Func_0808a180();

/* Used for their return values. */
s32 Func_080770c0();
s32 Func_0808a020();
s32 Func_02000f30();

s32 Func_02000f8c(s32 speaker)
{
    u32 cueBase = 0x294e;
    s32 leaderPresent;
    s32 speakerPresent;
    s32 speakerFlag;

    leaderPresent = Func_02000f30(0);
    speakerPresent = Func_02000f30(speaker);

    Func_0808a018();
    Func_0808a148(speaker, *(u32 *)&Data_02000240[500], 0);

    if (Func_080770c0(772) != 0) {
        Func_080770c0(752);
        speakerFlag = Func_080770c0(752 + speaker);

        if (Func_080770c0(0x305) != 0) {
            cueBase = (speakerFlag != 0) ? 0x2967 : 0x296c;
        } else {
            cueBase = (speakerFlag != 0) ? 0x2971 : 0x2976;
        }
    } else {
        if (leaderPresent == 0) {
            cueBase = 0x2958;
        } else if (speakerPresent == 0) {
            cueBase = 0x2953;
        }
    }

    Func_0808a170(cueBase + (u32)speaker - 1);
    Func_0808a180(speaker, 0);
    return Func_0808a020();
}
