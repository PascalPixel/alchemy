#include "types.h"

void Func_02002aa6();
s32 Func_02002992();
void Func_020028ec();
s32 Func_020029a8();
void Func_020029d8();
s32 Func_020029d8_a();
void Func_020029a0();
void Func_02002906();
s32 Func_020029c2();
void Func_0200291c();
s32 Func_020029cc();

s32 Func_020011bc(void)
{
    s32 handle;
    Func_02002aa6(85);
    handle = Func_02002992(0x292a, 5, 4, 1);
    while (Func_020029a8() == 0)
        Func_020028ec(1);
    Func_020029d8();
    Func_020029a0(handle, 1);
    Func_02002906(1);
    handle = Func_020029c2(0x292b, 5, 4, 1);
    while (Func_020029d8_a() == 0)
        Func_0200291c(1);
    return Func_020029cc(handle, 1);
}
