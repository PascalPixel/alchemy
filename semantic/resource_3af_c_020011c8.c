#include "types.h"

/*
 * resource_3af owner at 0x020011c8, 296 bytes: a flag-gated cutscene
 * beat on actor 20 -- two "hide, retarget, show" steps followed by a
 * placement and a fade-out.
 *
 * PUBLISHED, NOT CALLED. Thumb pointer 0x020091c9 at 0x02005948 in the
 * script-record table. Absent from every inventory and from
 * `tools/overlay_unindexed.ts`; see resource_3af_c_02000b98.c for the
 * sweep that finds this class.
 *
 * NEAR-IDENTICAL TWIN of 0x020010a0. The two spans are 296 bytes each
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
 * Complete owner: `push {r5, r6, lr}` at 0x020011c8 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x020012c0-0x020012c4; an
 * alignment halfword and ten pool words (identical to the twin's:
 * 0x00000911, 0x00000922, 0x00006666, 0x00003333, 0x00013333,
 * 0x00009999, 0x00033333, 0x00019999, 0x0000030a, 0x00000101) end at
 * 0x020012ef, and the next owner (0x020012f0, already tracked
 * byte-exact) begins at 0x020012f0.
 *
 * Uncertainty: the actor id and the Func_0808a0d0 arguments are
 * transcribed; flags 0x911 and 0x922 are exact and are two of the six
 * the 0x02001444 driver's tail chain also tests.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a010(s32 frames);
extern s32 Func_080770c0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a460(void);
extern void Func_020012f0(void);
extern void Func_020039ec(s32 packedId);

void Func_020011c8(void)
{
    u8 *record;

    if (Func_080770c0(0x911) != 0 && Func_080770c0(0x922) == 0) {
        Func_0808a018();
        Func_0808a460();
        Func_020012f0();

        Func_0808a090(20, 0x6666, 0x3333);
        record = Func_0808a080(20);
        record[90] &= 0xfe;
        Func_0808a0d0(20, 202, 204 << 2);
        Func_0808a010(1);
        record = Func_0808a080(20);
        record[90] |= 1;
        Func_0808a010(20);
        Func_0808a130(20, 2);
        Func_020039ec(20);

        Func_0808a090(20, 0x13333, 0x9999);
        record = Func_0808a080(20);
        record[90] &= 0xfe;
        Func_0808a0d0(20, 192, 201 << 2);
        Func_0808a010(1);
        record = Func_0808a080(20);
        record[90] |= 1;
        Func_0808a010(20);

        Func_0808a090(20, 0x33333, 0x19999);
        Func_0808a0d0(20, 180, 0x30a);
        Func_0808a0d0(20, 180, 175 << 2);
        Func_0808a0f0(20, 246 << 16, 128 << 18);
        Func_0808a1b8(20, 0, 0);
        Func_0808a1e8(0, 0x101, 60);
        Func_0808a020();
    }
}
