#include "types.h"

extern void Func_02003154();
extern void Func_020031e4();
extern void Func_02003202();
extern void Func_02003224();
extern s32 Func_0200313a();
extern void Func_02003174();
extern void Func_020021ee();
extern void Func_02003216();
extern void Func_020021e8();
extern void Func_0200322e();
extern void Func_0200320e();
extern void Func_02002200();
extern void Func_02003286();
extern void Func_02002210();
extern void Func_0200222e();
extern void Func_0200319c();
extern void Func_020031d8();

void Func_0200055c(void)
{
    Func_02003154();
    Func_020031e4(16, 2);
    Func_02003202(0x211b);
    Func_02003224(16, 0, 20);
    if (Func_0200313a(0x3c1)) {
        Func_02003174(20);
    } else {
        Func_020021ee(17, 0);
        Func_02003216(17, 1);
        Func_020021e8(17);
        Func_0200322e(17, 0, 20);
        Func_0200320e(17, 4);
        Func_02002200(17);
        Func_02003286(17, 0x105, 40);
        Func_02002210(17);
        Func_0200222e(17, 20480);
        Func_0200319c(0x3c1);
    }
    Func_020031d8();
}
