#include "types.h"

/*
 * resource_3af owner at 0x020010a0, 296 bytes: a flag-gated cutscene
 * beat on actor 20 -- two "hide, retarget, show" steps followed by a
 * placement and a fade-out.
 *
 * PUBLISHED, NOT CALLED. Thumb pointer 0x020090a1 at 0x0200593c in the
 * script-record table. Absent from every inventory and from
 * `tools/overlay_unindexed.ts`; see resource_3af_c_02000b98.c for the
 * sweep that finds this class.
 *
 * NEAR-IDENTICAL TWIN of 0x020011c8. The two spans are 296 bytes each
 * and differ in exactly FOUR halfwords, all of them the `movs r1,#imm`
 * that supplies Func_0808a0d0's second argument:
 *
 *     offset  0x42    0xbe / 0xca
 *     0x10a0   232     248        (and 244 at 0x8c)
 *     0x11c8   202     180        (and 192 at 0x8c)
 *
 * Every other halfword matches, including all ten pool words. The diff
 * was taken before either draft was written; that is the habit that
 * made 0x02000bb8/0x02000bf0 and 0x02000a48/0x02000af0 cheap in this
 * same overlay.
 *
 * The whole body sits under one guard: `flag 0x911 set AND flag 0x922
 * clear`. Both tests branch to the same epilogue, so it is a
 * short-circuit `&&`, not two nested ifs with separate tails.
 *
 * The repeated shape inside is the record +90 bit-0 pair also used by
 * 0x02003c88: clear bit 0, issue a Func_0808a0d0, wait one frame, set
 * bit 0. It appears twice. The masks come from `movs r5,#254` and
 * `movs r6,#1`, and both registers are then reused as the accumulators
 * for the second pair, so r5 stops holding 0xfe partway through -- the
 * same register-reuse pattern as 0x02003c88's close.
 *
 * The three Func_0808a090 calls walk a fixed-point ramp: (0x6666,
 * 0x3333), then (0x13333, 0x9999), then (0x33333, 0x19999). Each pair
 * is the previous one doubled and a bit -- transcribed, not derived.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020010a0 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02001198-0x0200119c; an
 * alignment halfword and ten pool words (0x00000911, 0x00000922,
 * 0x00006666, 0x00003333, 0x00013333, 0x00009999, 0x00033333,
 * 0x00019999, 0x0000030a, 0x00000101) end at 0x020011c7, and the next
 * owner prologue (its twin, 0x020011c8) begins at 0x020011c8.
 *
 * Uncertainty: the actor id and the Func_0808a0d0 arguments are
 * transcribed; flags 0x911 and 0x922 are exact and are two of the six
 * the 0x02001444 driver's tail chain also tests.
 */
















extern s32 Func_02005372(s32 flagId);
extern s32 Func_0200537c(s32 flagId);
extern void Func_020053a4(void);
extern void Func_02005510(void);
extern void Func_020023b0(void);
extern void Func_020053e6(s32 id, s32 arg1, s32 arg2);
extern u8 * Func_020053dc(s32 id);
extern void Func_02005444(s32 id, s32 arg1, s32 arg2);
extern void Func_020053d2(s32 frames);
extern u8 * Func_02005400(s32 id);
extern void Func_020053e8(s32 frames);
extern void Func_02005498(s32 id, s32 arg1);
extern void Func_02004afe(s32 packedId);
extern void Func_02005438(s32 id, s32 arg1, s32 arg2);
extern u8 * Func_0200542e(s32 id);
extern void Func_02005492(s32 id, s32 arg1, s32 arg2);
extern void Func_02005420(s32 frames);
extern u8 * Func_0200544e(s32 id);
extern void Func_02005434(s32 frames);
extern void Func_02005476(s32 id, s32 arg1, s32 arg2);
extern void Func_020054c0(s32 id, s32 arg1, s32 arg2);
extern void Func_020054cc(s32 id, s32 arg1, s32 arg2);
extern void Func_020054ea(s32 id, s32 arg1, s32 arg2);
extern void Func_0200555c(s32 id, s32 arg1, s32 arg2);
extern void Func_0200557e(s32 id, s32 arg1, s32 arg2);
extern void Func_0200548a(void);
void Func_020010a0(void)
{
    u8 *record;

    if (Func_02005372(0x911) != 0 && Func_0200537c(0x922) == 0) {
        Func_020053a4();
        Func_02005510();
        Func_020023b0();

        Func_020053e6(20, 0x6666, 0x3333);
        record = Func_020053dc(20);
        Func_02005444(20, 232, 204 << 2);
        Func_020053d2(1);
        record = Func_02005400(20);
        record[90] &= 0xfe;
        record[90] |= 1;
        Func_020053e8(20);
        Func_02005498(20, 2);
        Func_02004afe(20);

        Func_02005438(20, 0x13333, 0x9999);
        record = Func_0200542e(20);
        record[90] &= 0xfe;
        Func_02005492(20, 244, 201 << 2);
        Func_02005420(1);
        record = Func_0200544e(20);
        record[90] |= 1;
        Func_02005434(20);

        Func_02005476(20, 0x33333, 0x19999);
        Func_020054c0(20, 248, 0x30a);
        Func_020054cc(20, 248, 175 << 2);
        Func_020054ea(20, 246 << 16, 128 << 18);
        Func_0200555c(20, 0, 0);
        Func_0200557e(0, 0x101, 60);
        Func_0200548a();
    }
}
