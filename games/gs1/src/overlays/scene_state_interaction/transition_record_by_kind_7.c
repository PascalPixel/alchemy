#include "types.h"

void Func_02001f06();
void Func_020022be();
void Func_020022c4();
void Func_020022c6();
void Func_020022d4();
s32 Func_020022de();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_0200070c(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_020022de, 13);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_020022be, 0x318);
        Call1(Func_020022c4, 0x319);
        if (kind == 115) {
            Call1(Func_020022c6, 0x319);
        } else if (kind == 117) {
            Call1(Func_020022d4, 0x318);
        }
        Func_02001f06(0);
    }
}
