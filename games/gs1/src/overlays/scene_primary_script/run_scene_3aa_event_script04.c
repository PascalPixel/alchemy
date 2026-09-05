#include "types.h"

void Func_02002f92(s32, s32, s32);
void Func_02002f9e(s32, s32, s32);
void Func_02002faa(s32, s32, s32);
void Func_02002fb6(s32, s32, s32);
void Func_02002f08(s32);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

#define RunEventScript04 Func_02001450

void RunEventScript04(s32 handoff)
{
    Call3(Func_02002f92, 0, 0xc000, 0);
    Call3(Func_02002f9e, 1, 0xc000, 0);
    Call3(Func_02002faa, 2, 0xc000, 0);
    Call3(Func_02002fb6, 3, 0xc000, 0);
    if (handoff != 0) {
        Call1(Func_02002f08, handoff);
    }
}
