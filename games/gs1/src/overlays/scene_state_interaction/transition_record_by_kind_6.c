#include "types.h"

void Func_02001eba();
void Func_02002274();
void Func_0200227a();
void Func_0200227c();
void Func_02002288();
s32 Func_02002296();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_020006c4(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_02002296, 12);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_02002274, 0x316);
        Call1(Func_0200227a, 0x317);
        if (kind == 111) {
            Call1(Func_0200227c, 0x317);
        } else if (kind == 113) {
            Call1(Func_02002288, 0x316);
        }
        Func_02001eba(0);
    }
}
