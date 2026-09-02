#include "types.h"

#define FieldScene_RunPrimarySequenceSecond Func_02000fbc

extern u8 Value_0200f55a;
void Func_02006ef8();
void Func_02006ff2();
void Func_020070d8();
void Func_0200710e();
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


void Func_02000fbc(void)
{
    Call1(Func_0200710e, 158);
    Call3(Func_02006ef8, (s32)&Value_0200f55a, 35, 74);
    Call3(Func_02006ff2, 0, 102, 0x4b6);
    Call1(Func_020070d8, 10);
}
