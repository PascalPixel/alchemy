#include "types.h"

#define FieldScene_RunSupplementalSequenceSeven Func_020008ac

extern u8 Value_0200beb4;
void Func_02000fe2();
void Func_02002ef8();
void Func_02002fc4();
void Func_020030fe();
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


void Func_020008ac(void)
{
    Call1(Func_020030fe, 158);
    Call3(Func_02002ef8, (s32)&Value_0200beb4, 51, 39);
    Call3(Func_02002fc4, 0, 358, 0x29e);
    Call1(Func_02000fe2, 7);
}
