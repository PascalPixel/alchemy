#include "types.h"

#define FieldScene_RunPrimarySequenceHead Func_0200101c

s32 Func_02003456();
void Func_0200347e();
void Func_0200349c();
void Func_020034a6();
void Func_020034ae();
void Func_020034da();
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}


void Func_0200101c(void)
{
    if (Func_02003456(2059) == 0) {
        Call0(Func_0200347e);
        Call1(Func_0200349c, 9);
        Call3(Func_020034ae, 9, 13107, 0x00001999);
        Call3(Func_020034da, 9, 504, 152);
        Call0(Func_020034a6);
    }
}
