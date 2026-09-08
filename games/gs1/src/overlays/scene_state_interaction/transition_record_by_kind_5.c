#include "types.h"

void Func_02001e76();
void Func_0200222e();
void Func_02002234();
void Func_02002236();
void Func_02002244();
s32 Func_0200224e();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_0200067c(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_0200224e, 11);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_0200222e, 0x314);
        Call1(Func_02002234, 0x315);
        if (kind == 107) {
            Call1(Func_02002236, 0x315);
        } else if (kind == 109) {
            Call1(Func_02002244, 0x314);
        }
        Func_02001e76(0);
    }
}
