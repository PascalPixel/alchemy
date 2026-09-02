#include "types.h"

#define FieldScene_RunSupplementalSequenceFive Func_0200084c

extern u8 Value_0200beb4;
void Func_02000f80();
void Func_02002e98();
void Func_02002f62();
void Func_0200309e();
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


void Func_0200084c(void)
{
    Call1(Func_0200309e, 158);
    Call3(Func_02002e98, (s32)&Value_0200beb4, 41, 32);
    Call3(Func_02002f62, 0, 200, 0x222);
    Call1(Func_02000f80, 5);
}
