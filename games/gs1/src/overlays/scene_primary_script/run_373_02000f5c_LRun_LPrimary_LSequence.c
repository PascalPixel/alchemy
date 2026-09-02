#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000f5c

extern u8 Value_0200f570;
void Func_02006e98();
void Func_02006f94();
void Func_0200707a();
void Func_020070ae();
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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


void Func_02000f5c(void)
{
    Call1(Func_020070ae, 158);
    Call3(Func_02006e98, (s32)&Value_0200f570, 49, 69);
    Call3(Func_02006f94, 0, 326, 0x466);
    Call1(Func_0200707a, 8);
}
