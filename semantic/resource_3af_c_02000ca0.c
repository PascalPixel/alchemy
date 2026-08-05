#include "types.h"

/*
 * resource_3af owner at 0x02000ca0, 1024 bytes: the overlay's longest
 * scripted callback -- a three-actor cutscene (20, 22, 23) with two
 * camera moves, gated on story flag 0x911 and setting flag 0x920 on the
 * way out.
 *
 * PUBLISHED, NOT CALLED. Thumb pointer 0x02008ca1 at 0x02005930 in the
 * script-record table. Absent from every inventory and from
 * `tools/overlay_unindexed.ts`; see resource_3af_c_02000b98.c for the
 * sweep that finds this class.
 *
 * It is the LARGEST function in this overlay with essentially no
 * control flow: 1024 bytes, and the only branch in the entire body is
 * the opening guard `if (Func_080770c0(0x911) == 0) return;`, spelled
 * as a `bne` over a `b.n` to the epilogue because the distance exceeds
 * a short conditional branch's reach. Everything after it is one
 * straight sequence. Size is not complexity; the branch census is what
 * says so, and it is worth running before assuming a big row is hard.
 *
 * FIRST CALLER FOUND FOR 0x02000bf0. The call at 0x0200100e is the only
 * one anywhere in the image to the second of the two once-only
 * flag-guarded reward twins. Its sibling 0x02000bb8 has seven callers,
 * all scripted beats; 0xbf0 has exactly this one. That completes the
 * pair's story: both twins are granted from inside scripted beats, and
 * the 0x02001444 driver clears both their flags (0x271 and 0x272) when
 * flag 0x109 is set.
 *
 * STORY CHAIN. Guarded on 0x911, sets 0x920. Both are in the family the
 * driver's tail chain tests, and 0x920 is exactly what 0x02001684 reads
 * first when choosing its actor arrangement -- so this callback is a
 * producer for that chooser, the same relationship 0x020029d4 has via
 * flag 0x92b.
 *
 * Six constants are hoisted into callee-saved registers and reused
 * throughout, which is what the fp/sl/r9/r8 spill in the prologue pays
 * for: r5 = 0x4016 and r6 = 0x4017 (packed actor ids -- mode 4 on
 * actors 22 and 23, the form argued in resource_3af_c_02003a0c.c),
 * fp = 0xb000, r9 = 0x3000, sl = 0xd000, r8 = 0x5000. They are spelled
 * as their literal values at each site below rather than as named
 * locals, because the source has no reason to have named them.
 *
 * The two camera moves are Func_0808a208/Func_0808a210 pairs at
 * 0x02000ccc and 0x02000f10, plus a third Func_0808a208/Func_0808a210
 * at 0x02000fa6; the first is followed by Func_0808a218 and the others
 * are not.
 *
 * The close writes actor 20's record: bit 0 of byte +35 set, then after
 * the reward grant the words at +24 and +28 both set to 0x10000
 * through two separate Func_0808a080(20) fetches.
 *
 * Complete owner: `push {r5, r6, lr}` plus the fp/sl and r9/r8 spills
 * (`mov r6,fp / mov r5,sl / push {r5, r6}` then `mov r6,r9 /
 * mov r5,r8 / push {r5, r6}`) at 0x02000ca0, through the matching
 * unwind `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r3} / mov fp,r3 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x02001046-0x02001056; eighteen trailing pool words (0x00000911,
 * 0x00019999, 0x00003333, 0x00004016, 0x00001d26, 0x00004017,
 * 0x00000101, 0x00001d40, 0x0000cccc, 0x00001999, 0x00006666,
 * 0x0200c464, 0x0200c49c, 0x03160000, 0x00013333, 0x00009999,
 * 0x0000030e, 0x0000031e) end at 0x0200109f, and the next owner
 * prologue (0x020010a0, the drafted callback twin) begins at
 * 0x020010a0.
 *
 * Second read done (over 400 bytes): the correction was the guard's
 * shape. A first pass read `bne 0x02000cba / b.n 0x02001046` as a
 * two-way branch with work on both sides; the `bne` skips only the
 * `b.n`, so it is a single early return and the taken side has no body
 * at all.
 *
 * Uncertainty: actor ids and argument roles by shape; the camera
 * constants, the two data pointers and the record field writes are
 * transcribed, meanings unknown. Flags 0x911 and 0x920 are exact.
 */

extern u8 *Func_0808a080(s32 id);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a148(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a460(void);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_02000bf0(void);
extern void Func_02003a00(s32 id, s32 arg1);
extern void Func_020039ec(s32 packedId);

void Func_02000ca0(void)
{
    u8 *record;

    if (Func_080770c0(0x911) == 0) {
        return;
    }

    Func_0808a018();
    Func_0808a460();
    Func_0808a148(0, 20, 10);
    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(190 << 16, -1, 177 << 18, 1);
    Func_0808a218();
    Func_0808a010(40);

    Func_0808a138(22, 1);
    Func_0808a170(0x1d26);
    Func_020039ec(0x4016);
    Func_0808a1e8(20, 129 << 1, 60);
    Func_0808a130(20, 2);
    Func_020039ec(20);
    Func_0808a138(22, 1);
    Func_0808a1b8(22, 160 << 7, 0);
    Func_020039ec(0x4016);
    Func_0808a138(20, 1);
    Func_02003a00(20, 176 << 8);
    Func_020039ec(20);

    Func_02003a00(23, 192 << 6);
    Func_0808a100(23, 3);
    Func_020039ec(0x4017);
    Func_0808a1e8(22, 0x101, 40);
    Func_0808a1b8(22, 128 << 8, 20);
    Func_020039ec(0x4016);
    Func_02003a00(23, 0);
    Func_0808a110(23, 4);
    Func_020039ec(0x4017);
    Func_0808a1e8(20, 128 << 1, 40);
    Func_0808a130(20, 2);
    Func_020039ec(20);
    Func_0808a110(22, 3);
    Func_020039ec(0x4016);
    Func_02003a00(20, 208 << 8);
    Func_0808a100(23, 3);
    Func_0808a110(20, 3);
    Func_0808a010(60);

    Func_0808a1e8(22, 131 << 1, 40);
    Func_02003a00(22, 160 << 7);
    Func_0808a170(0x1d40);
    Func_0808a130(22, 1);
    Func_020039ec(0x4016);
    Func_0808a1e8(20, 0x101, 40);
    Func_0808a130(20, 2);
    Func_020039ec(20);
    Func_0808a1e8(22, 132 << 1, 20);
    Func_0808a188(0x4016, 0, 20);
    Func_0808a1e8(23, 129 << 1, 60);
    Func_020039ec(0x4017);
    Func_02003a00(22, 128 << 8);
    Func_0808a110(22, 3);
    Func_0808a188(0x4016, 0, 20);
    Func_0808a1e8(20, 129 << 1, 40);
    Func_0808a130(20, 2);
    Func_020039ec(20);
    Func_02003a00(22, 160 << 7);
    Func_0808a100(22, 4);
    Func_020039ec(22);

    Func_0808a1b8(20, 176 << 8, 0);
    Func_0808a1b8(23, 192 << 6, 40);
    Func_0808a1b8(23, 0, 0);
    Func_0808a1b8(20, 208 << 8, 20);
    Func_0808a138(22, 2);
    Func_0808a010(20);
    Func_020039ec(0x4016);
    Func_0808a1f0(23, 129 << 1);
    Func_0808a1f0(20, 129 << 1);
    Func_0808a010(40);
    Func_0808a110(22, 3);
    Func_020039ec(0x4016);

    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(182 << 16, -1, 190 << 18, 1);
    Func_0808a090(23, 0xcccc, 0x6666);
    Func_0808a098(23, 0x0200c464);
    Func_0808a090(22, 0xcccc, 0x6666);
    Func_0808a098(22, 0x0200c49c);
    Func_0808a090(20, 0xcccc, 0x6666);
    Func_0808a0d0(20, 182, 190 << 2);
    Func_0808a130(20, 2);
    Func_0808a1e8(20, 128 << 1, 60);
    Func_02003a00(20, 208 << 8);
    Func_0808a188(20, 0, 20);
    Func_0808a110(20, 3);
    Func_0808a128(20, 4, 0);
    Func_0808a1b8(20, 192 << 6, 40);

    Func_0808a208(128 << 9, 128 << 6);
    Func_0808a210(216 << 16, -1, 0x03160000, 1);
    record[35] |= 1;
    record = Func_0808a080(20);
    Func_0808a090(20, 0x13333, 0x9999);
    Func_0808a0d0(20, 182, 0x30e);
    Func_0808a0d0(20, 192, 202 << 2);
    Func_0808a0d0(20, 216, 202 << 2);
    Func_02003a00(20, 208 << 8);
    Func_0808a138(20, 2);

    Func_02000bf0();

    Func_0808a0d0(20, 216, 0x31e);
    *(s32 *)(record + 24) = 128 << 9;
    *(s32 *)(record + 28) = 128 << 9;
    Func_0808a0f0(20, 0, 0);
    record = Func_0808a080(20);
    record = Func_0808a080(20);
    Func_080770c8(0x920);
    Func_0808a020();
}
