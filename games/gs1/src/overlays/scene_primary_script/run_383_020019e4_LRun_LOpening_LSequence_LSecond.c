#include "types.h"

#define FieldScene_RunOpeningSequenceSecond Func_020019e4

s32 Func_0200671a();
extern u8 Value_00000854;
extern u8 Value_000012c3;
void Func_02006752();
void Func_0200676c();
void Func_02006776();
void Func_020067b4();
void Func_02006828();
void Func_02006840();
void Func_020068a6();
void Func_02006958();
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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


void Func_020019e4(void)
{
    Call3(Func_02006776, 0, 32768, 16384);
    Call3(Func_020067b4, 0, 744, 408);
    if (Func_0200671a((s32)&Value_00000854) == 0) {
        Call0(Func_02006752);
        Value1(Func_02006828, (s32)&Value_000012c3);
        Call2(Func_02006840, 8, 0);
        Call0(Func_0200676c);
    }
    *(u32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Value1(Func_02006958, 123);
    Call1(Func_020068a6, 14);
}
