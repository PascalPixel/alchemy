#include "types.h"

/*
 * resource_3a6 actor-9 particle descent at 0x02000e04 (372 bytes).
 *
 * Complete saved-high-register owner through the interworking return at
 * 0x02000f6a, followed by the three directly referenced fixed-point pool words
 * through 0x02000f77. It is the actor-9 companion to the independently
 * reviewed actor-10 rain sequence at 0x02000f78, but every address, constant,
 * argument and call was read again here. All 29 BL sites are retained in
 * machine order: ten record fetches, two object creates, the 68-frame particle
 * loop, presentation/cue updates, both object destroys, and the shared close.
 */















extern void Func_02002abc();
extern u8 * Func_02002ad2();
extern void Func_02002afa();
extern void Func_02002b4e();
extern u8 * Func_02002afc();
extern s32 Func_02002b0c();
extern u8 * Func_02002b14();
extern u8 * Func_02000eb2();
extern s32 Func_02002b2e();
extern u8 * Func_02002b3c();
extern u8 * Func_02000ed6();
extern u8 * Func_02002b52();
extern void Func_02002c0a();
extern s32 Func_02002ad8(void);
extern s32 Func_02002aea(void);
extern void Func_0200102a();
extern u8 * Func_02002bb0();
extern void Func_02002ba8();
extern void Func_02002b88();
extern u8 * Func_02002bde();
extern void Func_02002bc8();
extern u8 * Func_02002bf6();
extern void Func_02002c42();
extern void Func_02002b98();
extern void Func_02002b9e();
extern void Func_02002bfc();
extern void Func_02002c10();
void Func_02000e04(void)
{
    u8 params[40];
    s32 left;
    s32 right;
    u32 index;
    s32 x;
    s32 z;

    Func_02002abc();
    if ((*(s32 *)(Func_02002ad2(9) + 8) >> 20) != 23) goto close;

    Func_02002afa(0, 360, 664);
    Func_02002b4e(0, 0xe000, 10);
    *(s32 *)(Func_02002afc(9) + 8) += 0x20000;

    left = Func_02002b0c(*(s32 *)(Func_02002b14(9) + 8) - 0x100000,
                         0,
                         *(s32 *)(Func_02000eb2(9) + 16) + 0x340000,
                         241);
    right = Func_02002b2e(*(s32 *)(Func_02002b3c(9) + 8) + 0x100000,
                          241);
                          0,
                          *(s32 *)(Func_02000ed6(9) + 16) + 0x340000,
    Func_02002b52(9)[0x55] = 0;

    *(s32 *)(params + 8) = 0x9999;
    Func_02002c0a(216);
    *(s32 *)(params + 12) = 0x9999;
    *(s32 *)(params + 4) = 7;

    for (index = 0; index <= 67; index++) {
        x = ((Func_02002ad8() * 17) & ~0xffff) + 0x1b00000;
        z = ((Func_02002aea() * 14) & ~0xffff) + 0x2700000;
        Func_0200102a(x, 0, z, 0, 0, 0, 0x90000, params);
        *(s32 *)(Func_02002bb0(9) + 12) += -0x8000;
        Func_02002ba8(1);
    }

    Func_02002b88(23, 41, 1, 1, 23, 39);
    Func_02002bde(9)[0x23] |= 2;
    Func_02002bc8(0x200);
    *(s32 *)(Func_02002bf6(9) + 12) = -0x80000;
    Func_02002c42(9, 2);
    Func_02002b98(left);
    Func_02002b9e(right);
    Func_02002bfc(30);

close:
    Func_02002c10();
}
