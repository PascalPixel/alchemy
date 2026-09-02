#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000750

extern u8 Value_0200beb4;
void Func_02000e84();
void Func_02002d9c();
void Func_02002e66();
void Func_02002fa2();
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


void Func_02000750(void)
{
    Call1(Func_02002fa2, 158);
    Call3(Func_02002d9c, (s32)&Value_0200beb4, 44, 7);
    Call3(Func_02002e66, 0, 248, 0x117);
    Call1(Func_02000e84, 1);
}
