#include "types.h"

#define FieldScene_RunSupplementalSequenceFour Func_0200081c

extern u8 Value_0200beb4;
void Func_02000f52();
void Func_02002e68();
void Func_02002f34();
void Func_0200306e();
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


void Func_0200081c(void)
{
    Call1(Func_0200306e, 158);
    Call3(Func_02002e68, (s32)&Value_0200beb4, 52, 18);
    Call3(Func_02002f34, 0, 374, 0x1a3);
    Call1(Func_02000f52, 4);
}
