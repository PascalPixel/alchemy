#include "types.h"

void Func_02001f4a();
void Func_02002304();
void Func_0200230a();
void Func_0200230c();
void Func_02002318();
s32 Func_02002326();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_02000754(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_02002326, 14);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_02002304, 0x31a);
        Call1(Func_0200230a, 0x31b);
        if (kind == 119) {
            Call1(Func_0200230c, 0x31b);
        } else if (kind == 121) {
            Call1(Func_02002318, 0x31a);
        }
        Func_02001f4a(0);
    }
}
