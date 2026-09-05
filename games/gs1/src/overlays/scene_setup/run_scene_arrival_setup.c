#include "types.h"
/* resource_3a8 owner at 0x02000504, 138 bytes. Scene arrival sequence. */
void Func_020040fe();
void Func_0200428c();
void Func_020040be();
void Func_0200403c();
void Func_020040d4();
void Func_02004052();
void Func_02004178();
u8 *Func_0200416e();
void Func_020041dc();
void Func_020041d8();
void Func_0200415e();
void Func_0200429c();
void Func_020042b0();
void Func_020042bc();
void Func_02004180();

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Func_020041d8(actor, axis, offset);
}

void RunSceneArrivalSetup(void)
{
    s32 two = 2;

    Func_020040fe();
    Func_0200428c(188);
    Func_020040be(36, 23, 43, 12, two, two);
    Func_0200403c(5);
    Func_020040d4(39, 23, 43, 12, two, two);
    Func_02004052(5);
    Func_02004178(0, 0x8000, 0x4000);
    Func_0200416e(0)[85] = 0;
    Func_020041dc(0, 2);
    SetOffset(0, 0, -8);
    Func_0200415e(10);
    Func_0200429c(2);
    Func_020042b0();
    Func_020042bc();
    Func_02004180();
}
