#include "types.h"

#define FieldScene_RunSupplementalSequenceSix Func_0200087c

extern u8 Value_0200beb4;
void Func_02000fb0();
void Func_02002ec8();
void Func_02002f92();
void Func_020030ce();
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


void Func_0200087c(void)
{
    Call1(Func_020030ce, 158);
    Call3(Func_02002ec8, (s32)&Value_0200beb4, 35, 36);
    Call3(Func_02002f92, 0, 102, 0x263);
    Call1(Func_02000fb0, 6);
}
