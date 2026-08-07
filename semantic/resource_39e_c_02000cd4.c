#include "types.h"

/*
 * Resource 39e retreat-along-arc then triple emit at 0x02000cd4 (198 bytes,
 * 8 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000cd4 the only branch is the
 * loop back edge at 0x02000d1e -> 0x02000ce6, inside the body, and the walk
 * stops at the high-register unwind `add sp,#16 / pop {r3} / mov r8,r3 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` at 0x02000d8e-0x02000d98.  So the
 * executable extent is exactly 0x02000cd4-0x02000d99 (198 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map — the halfword at 0x02000d9a is `0x0000` alignment and the six
 * words after it are literals the walk never reaches as instructions:
 *   0x02000d9c  0xffff3334   -0.8 in 16.16
 *   0x02000da0  0xfff40000   -12.0 in 16.16
 *   0x02000da4  0x00003333   0.2
 *   0x02000da8  0x00004ccc   0.3
 *   0x02000dac  0x0000cccc   0.8
 *   0x02000db0  0x00006666   0.4
 * None is an address — all six are far below the 0x02008000 link base and all
 * six are used as arithmetic operands or as call arguments.  The next prologue
 * (`push {r5, r6, r7, lr}`) begins at 0x02000db4.
 *
 * All eight call sites were resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_39e 0cd4 d9a`, and the
 * per-target histogram is Func_0200013c x3, Func_0808a080 x1, Func_080000c0
 * x1, Func_08000118 x1, Func_08000120 x1, Func_080f9010 x1:
 *   0x02000cde -> veneer 0x04404 -> Func_0808a080   record fetch, index 19
 *   0x02000cf2 -> veneer 0x04344 -> Func_080000c0   yield (12-i)*2 frames
 *   0x02000cf8 -> veneer 0x04374 -> Func_08000120   cosine-side lookup
 *   0x02000d0a -> veneer 0x0436c -> Func_08000118   sine-side lookup
 *   0x02000d2e -> veneer 0x0457c -> Func_080f9010   one-argument notify, 227
 *   0x02000d54 0x02000d6c 0x02000d8a -> prologue 0x0013c -> Func_0200013c
 * `overlay_show` prints the three local calls as three different addresses
 * (0x02000e92, 0x02000eaa, 0x02000ec8) although all three are the one callee —
 * the pc-relative misreading.
 *
 * FOUR OF THE EIGHT SITES ARE INSIDE THE LOOP and each appears once.  The
 * multiset is over sites, not executions, so the yield and the two trig
 * lookups stay inside the loop body; unrolling the five iterations would put
 * fifteen calls in the C against three in the assembly.
 *
 * The loop runs i = 8..12 inclusive (`adds r6,#1 / cmp r6,#12 / bls`).  Each
 * iteration writes `i << 12` as a u16 into +30 of the sub-object at +80 — the
 * BAM angle field, so the run sweeps 0x8000 to 0xc000, a quarter turn — waits
 * (12 - i) * 2 frames so the steps get faster as the sweep ends, and then backs
 * the record off along that angle: `lsls #1 / adds / lsls #1` is a multiply by
 * six, and both results are SUBTRACTED, from +8 with the Func_08000120 term and
 * from +16 with the Func_08000118 term, which is the established along/across
 * pairing run in reverse.
 *
 * After the sweep +12 and +60 both take 18.0 (`movs #144 / lsls #13`) and +24
 * takes -0.8, then three spawns are issued through the local Func_0200013c.
 * They share y = +12 and z = +16 + 8.0 and differ in x and in two scalars:
 *   x = +8 - 12.0, 1.0,   0.2      (the -12.0 pool word)
 *   x = +8,        0.8,   0.3
 *   x = +8 + 10.0, 0.2,   0.4      (`movs #160 / lsls #12`)
 * The 0.2 of the third spawn is the SAME register the first spawn used, parked
 * in r8 across the second call rather than re-loaded — which is the evidence
 * that the first and third scalars really are one value and not two equal
 * constants.  The last three stack arguments are zero on all three calls.
 *
 * r4 is used as call-clobbered scratch without being saved, under a
 * `push {r5,r6,r7,lr}` frame — the same shape several resource_371 owners use.
 * It is not a missing save; nothing reads r4 across a call here.
 */

u8 *Func_020050e4();           /* record fetch, returns the record */
void Func_02005038();          /* advance the task scheduler */
s32 Func_0200506e();           /* along-axis trig lookup */
s32 Func_02005078();           /* across-axis trig lookup */
void Func_020052ac();          /* one-argument notify */
void Func_02000e92();          /* local: spawn, eight arguments (entry 1) */
void Func_02000eaa();          /* local: spawn, eight arguments (entry 2) */
void Func_02000ec8();          /* local: spawn, eight arguments (entry 3) */

void Func_02000cd4(void)
{
    u8 *record = Func_020050e4(19);
    u32 index;
    s32 angle;
    s32 scalar;

    for (index = 8; index <= 12; index++) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Func_02005038((12 - index) * 2);
        *(s32 *)(record + 8) -= Func_0200506e(angle) * 6;
        *(s32 *)(record + 16) -= Func_02005078(angle) * 6;
    }

    *(s32 *)(record + 12) = 0x120000;          /* 144 << 13, i.e. 18.0 */
    *(s32 *)(record + 60) = 0x120000;
    *(s32 *)(record + 24) = -0xcccc;           /* 0xffff3334 */

    Func_020052ac(227);

    scalar = 0x3333;                           /* parked in r8 for the third */
    Func_02000e92(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  *(s32 *)(record + 16) + 0x80000,
                  0x10000, scalar, 0, 0, 0);
    Func_02000eaa(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  0xcccc, 0x4ccc, 0, 0, 0);
    Func_02000ec8(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  scalar, 0x6666, 0, 0, 0);
}
