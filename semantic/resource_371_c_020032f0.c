#include "types.h"

















/* Move actor 8 through the complete staged scale-and-position approach. */
extern void Func_020075d0(void);
extern void Func_020076fa(s32, s32, s32, s32);
extern void Func_02007490(s32);
extern void Func_02007678(s32, s32);
extern void Func_0200767c(s32, s32, s32);
extern u8 * Func_02007622(s32);
extern void Func_020074b6(s32);
extern void Func_020077c6(s32, s32);
extern void Func_020076a0(s32, s32, s32);
extern void Func_02007730(s32, s32);
extern void Func_020074d6(s32);
extern void Func_020077a0(void);
extern void Func_0200767a(s32, s32, s32);
extern void Func_020076ae(s32, s32, s32);
extern void Func_020076ba(s32, s32, s32);
extern void Func_0200769c(s32, s32, s32);
extern void Func_020076d0(s32, s32, s32);
extern void Func_020076b2(s32, s32, s32);
extern void Func_020076e6(s32, s32, s32);
extern void Func_020076c8(s32, s32, s32);
extern void Func_020076fc(s32, s32, s32);
extern void Func_02007734(s32, s32);
extern void Func_020076aa(s32);
extern void Func_0200781e(void);
extern void Func_0200782a(void);
extern void Func_020077f0(s32);
void Func_020032f0(void)
{
    u8 *actor;

    *(u16 *)(actor + 6) = 0xa000;
    Func_020075d0();
    Func_020076fa(-1, -1, -1, 0);
    Func_02007490(1);
    Func_02007678(8, 2);
    Func_0200767c(8, 0x13080000, 0x03280000);
    actor = Func_02007622(8);
    Func_020074b6(1);
    Func_020077c6(0x13333, 1);
    Func_020076a0(0, 0, 0);
    Func_02007730(8, 1);
    *(s32 *)(*(u8 **)0x03001ebc + 448) = 256;
    Func_020074d6(1);
    Func_020077a0();

    Func_0200767a(8, 0x6666, 0x3333);
    Func_020076ae(8, 0x12d80000, 0x2c8);
    Func_020076ba(8, 0x12a80000, 0x268);
    Func_0200769c(8, 0x4ccc, 0x2666);
    Func_020076d0(8, 0x12a80000, 0x1d8);
    Func_020076b2(8, 0x3333, 0x1999);
    Func_020076e6(8, 0x12980000, 0x1c8);
    Func_020076c8(8, 0x1999, 0x0ccc);
    Func_020076fc(8, 0x12980000, 0x1b8);
    Func_02007734(8, 1);
    Func_020076aa(40);
    Func_0200781e();
    Func_0200782a();
    Func_020077f0(110);
}
