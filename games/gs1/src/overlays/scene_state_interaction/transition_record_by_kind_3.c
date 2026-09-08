#include "types.h"

void Func_02001de6();
void Func_0200219e();
void Func_020021a4();
void Func_020021a6();
void Func_020021b4();
s32 Func_020021be();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_020005ec(void)
{
    s32 record;
    s32 kind;

    record = Value1(Func_020021be, 9);
    if (record != 0) {
        kind = (*(volatile s32 *)(record + 8)) >> 20;
        Call1(Func_0200219e, 0x310);
        Call1(Func_020021a4, 0x311);
        if (kind == 99) {
            Call1(Func_020021a6, 0x311);
        } else if (kind == 101) {
            Call1(Func_020021b4, 0x310);
        }
        Func_02001de6(0);
    }
}
