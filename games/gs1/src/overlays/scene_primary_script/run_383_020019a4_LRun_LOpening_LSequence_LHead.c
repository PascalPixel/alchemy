#include "types.h"

#define FieldScene_RunOpeningSequenceHead Func_020019a4

void Func_02006736();
void Func_0200676c();
void Func_02006846();
void Func_020068f8();
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}


void Func_020019a4(void)
{
    Call3(Func_02006736, 0, 32768, 16384);
    Call3(Func_0200676c, 0, 728, 408);
    *(u32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Value1(Func_020068f8, 123);
    Call1(Func_02006846, 15);
}
