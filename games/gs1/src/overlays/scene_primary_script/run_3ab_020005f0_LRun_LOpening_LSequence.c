#include "types.h"

#define FieldScene_RunOpeningSequence Func_020005f0

extern u8 Value_02008469;
void Func_02001fa8();
void Func_02001fc2();
void Func_02001fd4();
void Func_02001fd6();
void Func_02001fea();
void *Func_02001ff8();
void Func_02002000();
void Func_02002042();
void Func_02002046();
void Func_0200204c();
void Func_02002050();
void Func_02002072();
void *Func_0200208e();
s32 Func_02002098();
void Func_020020d0();
void Func_0200214c();
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}


void Func_020005f0(void)
{
    void *p1;
    void *p12;

    p1 = Func_02001ff8(0);
    Call3(Func_02002042, 19, 0, 0);
    Call3(Func_0200204c, 8, 0, 0);
    Call6(Func_02001fa8, 38, 38, 1, 1, 46, 4);
    Call6(Func_02001fc2, 37, 37, 3, 3, 13, 3);
    Call6(Func_02001fd6, 37, 37, 1, 1, 14, 2);
    Call6(Func_02001fea, 8, 16, 1, 1, 7, 16);
    Call3(Func_0200214c, 102, 0, 0);
    Call6(Func_02002000, 32, 42, 3, 2, 1, 15);
    Call1(Func_02002050, 512);
    Call2(Func_020020d0, 8, 1);
    p12 = Func_0200208e(8);
    *(s32 *)(p12 + 108) = 0;
    Value2(Func_02002046, Value1(Func_02002098, 8), 0);
    Value1(Func_02001fd4, (s32)&Value_02008469);
    Value1(Func_02002072, 0x201);
}
