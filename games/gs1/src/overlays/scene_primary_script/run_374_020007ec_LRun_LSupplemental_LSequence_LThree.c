#include "types.h"

#define FieldScene_RunSupplementalSequenceThree Func_020007ec

extern u8 Value_0200beb4;
void Func_02000f20();
void Func_02002e38();
void Func_02002f02();
void Func_0200303e();
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


void Func_020007ec(void)
{
    Call1(Func_0200303e, 158);
    Call3(Func_02002e38, (s32)&Value_0200beb4, 43, 15);
    Call3(Func_02002f02, 0, 230, 0x197);
    Call1(Func_02000f20, 3);
}
