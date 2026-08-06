#include "types.h"

/*
 * resource_3a4 owner at 0x02002804, 304 bytes: stage the actor-10
 * record for the sink scene -- park actors 8/9, seat 9, zero actor
 * 10's +0x55 mode, load its +0x18/+0x1c pair with 0xe666/0x9999 (0.9 /
 * 0.6 in 16.16, the same +0x18/+0x1c amplitude slots the 6dc twin
 * writes 0x1999 into on actor 8), point its sprite's +30 halfword at
 * 0x8000, sink actor 12 to y = -28.0 -- then two flag-0x908 blocks and
 * a selector-6 camera dive.
 *
 * Flag 0x908 set (first check): nudge actor 10 by (+14.0, -8.0) on
 * x/y and bump the sprite +30 halfword to 0xc000.  Second check, same
 * flag (the b.n at 0x2002884 only hops the interior pool): pop window
 * Func_08009180(25, 36, 43, 36, 11, 9) and panel Func_080091c0(25, 35,
 * 10, 5, 43, 35), then the no-argument Func_08009128() commit and one
 * frame.
 *
 * Selector 6 (scene table +450) with flag 0x109 clear: an
 * Func_0808a018 .. Func_0808a020 bracket that drops record 0 to
 * y = -88.0 and dives the camera with Func_0808a210(198 << 18, r1,
 * 0x02410000, 0) -- r1 rides along as 0xffa80000 from the y store, an
 * argument-register reuse kept as observed rather than reinterpreted --
 * followed by the same Func_08009128() / one-frame pair.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002804, `sub sp, #8`,
 * through `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x200291c-0x2002922, interior pool at 0x2002886-0x200289f, closing
 * pool at 0x2002924-0x2002933, immediately followed by the drafted
 * owner at 0x02002934.  No arguments, void.
 *
 * All 20 bl sites resolve with the `+2` stored-displacement rule, all
 * veneers:
 *   0x200280a (10), 0x2002826 (9), 0x200284a (12), 0x2002856 (12),
 *     0x20028fa (0) -> Func_0808a080
 *   0x2002816 (8), 0x2002820 (9) -> Func_0808a0f0
 *   0x200282c -> Func_080091e0   (record, 0), established
 *   0x2002860, 0x20028a0 (0x908), 0x20028ec (0x109) -> Func_080770c0
 *   0x20028b8 -> Func_08009180   six-arg window call
 *   0x20028cc -> Func_080091c0   six-arg panel call
 *   0x20028d0, 0x200290e -> Func_08009128   no-argument commit
 *   0x20028d6, 0x2002914 -> Func_080000c0   wait n frames
 *   0x20028f4 -> Func_0808a018, 0x2002918 -> Func_0808a020
 *   0x200290a -> Func_0808a210   camera (x, y, z, mode)
 */

           /* test story flag, established */
           /* scene-record accessor, established */
          /* place entity (id, x, z), established */
          /* (record, n), established */
          /* six-argument window call */
          /* six-argument panel call */
          /* no-argument commit */
          /* wait n frames, established */
          /* open bracket, established */
          /* close bracket, established */
          /* camera (x, y, z, mode), established */

extern u8 * Func_020063a4();
extern void Func_02006400();
extern void Func_0200640a();
extern void Func_020063c0();
extern u8 * Func_0200635e();
extern u8 * Func_020063e4();
extern u8 * Func_020063f0();
extern s32 Func_020063b2();
extern s32 Func_020063f2();
extern void Func_020063c2();
extern void Func_020063f6();
extern void Func_02006350();
extern s32 Func_0200643e();
extern void Func_02006466();
extern u8 * Func_02006494();
extern void Func_0200658c();
extern void Func_0200638e();
extern void Func_02006492();
void Func_02002804(void)
{
    u8 *actor10;
    u8 *record;

    actor10 = Func_020063a4(10);
    Func_02006400(8, 0, 0);
    Func_0200640a(9, 0, 0);
    Func_020063c0(Func_0200635e(9), 0);

    actor10[0x55] = 0;
    *(s32 *)(actor10 + 0x18) = 0xe666;      /* 0.9 in 16.16 */
    *(s32 *)(actor10 + 0x1c) = 0x9999;      /* 0.6 in 16.16 */
    *(u16 *)(*(u8 **)(actor10 + 0x50) + 30) = 0x8000;

    Func_020063e4(12)[0x55] = 0;
    *(s32 *)(Func_020063f0(12) + 12) = 0xffe40000;  /* -28.0 */

    if (Func_020063b2(0x908) != 0) {
        *(s32 *)(actor10 + 12) += 0xfff80000;       /* -8.0 */
        *(u16 *)(*(u8 **)(actor10 + 0x50) + 30) = 0xc000;
        *(s32 *)(actor10 + 8) += 0xe0000;           /* +14.0 */
    }

    if (Func_020063f2(0x908) != 0) {
        Func_020063c2(25, 36, 43, 36, 11, 9);
        Func_020063f6(25, 35, 10, 5, 43, 35);
        Func_020063c2();
        Func_02006350(1);
    }

    if (*(s16 *)((u8 *)0x02000240 + 450) == 6
        && Func_0200643e(0x109) == 0) {
        Func_02006466(0);       /* r0 is the flag test's zero result */
        record = Func_02006494(0);
        Func_0200658c(198 << 18, 0xffa80000, 0x02410000, 0);
        *(s32 *)(record + 12) = 0xffa80000;         /* -88.0 */
        Func_02006400();
        Func_0200638e(1);
        Func_02006492();
    }
}
