#include "types.h"

















/* One-time positioning bridge that hands actor 3 from flag 0x250 to 0x872. */
extern int Func_02003e30();
extern void Func_02003e44();
extern void Func_02003e60();
extern u8 * Func_02003e76();
extern u8 * Func_02003e80();
extern u8 * Func_02003e88();
extern void Func_02003ed8();
extern void Func_02003f1c();
extern void Func_02003f9c();
extern void Func_02003fb0();
extern void Func_02003ea6();
extern void Func_02003f28();
extern void Func_02003f20();
extern void Func_02003ebe();
extern void Func_02003f00();
extern void Func_02003ece();
extern void Func_02003f46();
extern void Func_02003f2c();
extern void Func_02003f3e();
extern void Func_02003edc();
extern void Func_02003f08();
void Func_0200196c(void)
{
    u8 *actor;
    u8 *workspace;

    s32 permuted_11;
    if (Func_02003e30(0x250) != 0) return;
    Func_02003e44(0x250);
    Func_02003e60();
    actor = Func_02003e76(12); *(int *)(actor + 24) = -0x10000;
    actor = Func_02003e80(13); *(int *)(actor + 24) = -0x10000;
    actor = Func_02003e88(14); *(int *)(actor + 24) = -0x10000;
    Func_02003ed8(3, 0x880000, 0x900000);
    Func_02003f1c(3, 0x10000, 10);
    permuted_11 = *(u8 **)0x03001ebc;
    *(int *)(workspace + 448) = 0x201;
    workspace  = permuted_11;
    Func_02003f9c();
    Func_02003fb0();
    Func_02003ea6(60);
    Func_02003f28(3, 0, 0);
    Func_02003f20(3, 3);
    Func_02003ebe(30);
    Func_02003f00(3, 136, 72);
    Func_02003ece(40);
    Func_02003f46(0, 1);
    Func_02003f2c(3);
    Func_02003f3e(3, 0, 0);
    Func_02003edc(0x872);
    *(int *)(workspace + 448) = 0x204;
    Func_02003f08();
}
