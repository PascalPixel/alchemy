#include "types.h"

#define FieldScene_RunLateSequenceHead Func_02001fdc

void Func_0200508e();
void Func_020050a0();
void Func_02005162();
void Func_02005170();
void Func_020051a2();
void Func_020051ac();
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}


void Func_02001fdc(void)
{
    Call6(Func_0200508e, 72, 49, 1, 1, 8, 49);
    Call6(Func_020050a0, 113, 43, 1, 1, 49, 43);
    Call3(Func_020051a2, 100, 0, 0);
    Call3(Func_020051ac, 101, 0, 0);
    Call3(Func_02005162, 15, 8912896, 51904512);
    Call3(Func_02005170, 16, 51904512, 45613056);
}
