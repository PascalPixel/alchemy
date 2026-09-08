#include "types.h"

void Func_0200211c();
void Func_02002122();
void Func_02002124();
void Func_02002130();
s32 Func_0200213e();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_0200056c(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_0200213e, 9);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_0200211c, 0x302);
        Call1(Func_02002122, 0x303);
        if (kind == 93) {
            Call1(Func_02002124, 0x303);
        } else if (kind == 95) {
            Call1(Func_02002130, 0x302);
        }
    }
}
