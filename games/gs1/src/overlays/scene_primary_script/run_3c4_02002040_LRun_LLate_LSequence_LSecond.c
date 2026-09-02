#include "types.h"

#define FieldScene_RunLateSequenceSecond Func_02002040

void Func_020050f2();
void Func_02005104();
void Func_020051ca();
void Func_020051d4();
void Func_0200520a();
void Func_02005218();
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}


void Func_02002040(void)
{
    Call6(Func_020050f2, 8, 113, 1, 1, 8, 49);
    Call6(Func_02005104, 49, 107, 1, 1, 49, 43);
    Call3(Func_0200520a, 100, -1, -1);
    Call3(Func_02005218, 101, -1, -1);
    Call3(Func_020051ca, 15, 0, 0);
    Call3(Func_020051d4, 16, 0, 0);
}
