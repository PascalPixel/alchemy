#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000334

void Func_02000bd6();
void Func_02000c04();
void Func_02000c0e();
void Func_02000c12();
void Func_02000c20();
void Func_02000c2e();
void Func_02000c42();
void Func_02000c5c();
void Func_02000c82();
void Func_02000cbe();
void Func_02000cca();
static __inline__ void Call0(void (*f)())
{
    f();
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


void Func_02000334(void)
{
    Call0(Func_02000bd6);
    Call3(Func_02000c04, 8, 65536, 32768);
    Call3(Func_02000c12, 9, 65536, 32768);
    Call3(Func_02000c2e, 8, 136, 384);
    Call3(Func_02000c42, 9, 152, 384);
    Call3(Func_02000cbe, 8, 16384, 0);
    Call3(Func_02000cca, 9, 16384, 0);
    Call2(Func_02000c82, 8, 1);
    Call6(Func_02000c0e, 6, 27, 1, 1, 7, 27);
    Call6(Func_02000c20, 9, 26, 2, 1, 7, 26);
    Call0(Func_02000c5c);
}
