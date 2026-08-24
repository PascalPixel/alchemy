#include "types.h"

void Func_02002b12();
s32 Func_020029fe();
void Func_02002958();
s32 Func_02002a14();
void Func_02002a44();
s32 Func_02002a44_a();
void Func_02002a0c();
void Func_02002972();
s32 Func_02002a2e();
void Func_02002988();
s32 Func_02002a38();

s32 Func_02001228(void)
{
    s32 handle;
    Func_02002b12(85);
    handle = Func_020029fe(0x292c, 5, 4, 1);
    while (Func_02002a14() == 0)
        Func_02002958(1);
    Func_02002a44();
    Func_02002a0c(handle, 1);
    Func_02002972(1);
    handle = Func_02002a2e(0x292d, 5, 4, 1);
    while (Func_02002a44_a() == 0)
        Func_02002988(1);
    return Func_02002a38(handle, 1);
}
