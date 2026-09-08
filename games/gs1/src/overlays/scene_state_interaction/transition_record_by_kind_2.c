#include "types.h"

void Func_0200215e();
void Func_02002164();
void Func_02002168();
void Func_02002174();
s32 Func_0200217e();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_020005ac(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_0200217e, 10);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_0200215e, 0x300);
        Call1(Func_02002164, 0x301);
        if (kind == 115) {
            Call1(Func_02002168, 0x300);
        } else if (kind == 113) {
            Call1(Func_02002174, 0x301);
        }
    }
}
