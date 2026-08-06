#include "types.h"

/*
 * resource_3ba party-count-selected interaction at 0x02001c7c, complete
 * 164-byte span through its two-word pool. Independently mapped against the
 * instruction-identical resource_3bb:1f14/resource_3bc:29ac family.
 */
















extern u8 * Func_0200593a(s32 selector);
extern void Func_0200592e(void);
extern s32 Func_0200590a(void);
extern void Func_020059cc(s32 message);
extern s32 Func_020059ec(s32 selector, s32 mode);
extern void Func_0200597e(s32 selector, s32 scale, s32 secondaryScale);
extern void Func_0200598c(s32 selector, s32 scale, s32 secondaryScale);
extern void Func_020059b0(s32 selector, s32 x, s32 z);
extern void Func_0200596e(s32 frames);
extern void Func_020059c8(s32 selector, s32 x, s32 z);
extern void Func_020059d4(s32 selector, s32 x, s32 z);
extern void Func_02005aa8(void);
extern void Func_02005ab4(void);
extern void Func_02005a82(s32 request);
extern void Func_02005a32(s32 message);
extern void Func_02005a4a(s32 selector, s32 mode);
extern void Func_020059b6(void);
void Func_02001c7c(s32 selector)
{
    u8 *actor = Func_0200593a(selector);
    s32 x = *(s16 *)(actor + 10);
    s32 z = *(s16 *)(actor + 18);

    Func_0200592e();
    if (Func_0200590a() <= 1) {
        Func_020059cc(0x20e5);
        if (Func_020059ec(selector, 0) == 0) {
            Func_0200597e(0, 0x10000, 0x8000);
            Func_0200598c(selector, 0x10000, 0x8000);
            Func_020059b0(selector, x, z + 64);
            Func_0200596e(15);
            Func_020059c8(0, x, z);
            Func_020059d4(0, x, z + 32);
            Func_02005aa8();
            Func_02005ab4();
            Func_02005a82(11);
        }
    } else {
        Func_02005a32(0x20e8);
        Func_02005a4a(selector, 0);
    }
    Func_020059b6();
}
