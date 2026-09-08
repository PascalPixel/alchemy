#include "types.h"

void Func_02001e2a();
void Func_020021e4();
void Func_020021ea();
void Func_020021ec();
void Func_020021f8();
s32 Func_02002206();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_02000634(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_02002206, 10);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_020021e4, 0x312);
        Call1(Func_020021ea, 0x313);
        if (kind == 103) {
            Call1(Func_020021ec, 0x313);
        } else if (kind == 105) {
            Call1(Func_020021f8, 0x312);
        }
        Func_02001e2a(0);
    }
}
