#include "types.h"

#define FieldScene_RunSequenceTail Func_0200206c

void Func_020076a0();
void Func_020076f8();
void Func_0200771e();
void Func_0200772c();
void Func_0200773a();
void Func_0200774a();
void Func_02007752();
void Func_0200775a();
void Func_0200778c();
void Func_02007834();
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


void Func_0200206c(void)
{
    Call0(Func_020076a0);
    Call3(Func_0200771e, 12, 45088768, 5767168);
    Call3(Func_0200772c, 13, 46137344, 5767168);
    Call3(Func_0200773a, 14, 47185920, 6291456);
    Call2(Func_0200774a, 12, 5);
    Call2(Func_02007752, 13, 5);
    Call2(Func_0200775a, 14, 5);
    Call3(Func_0200778c, 0, 13, 0);
    Call0(Func_020076f8);
    Call0(Func_02007834);
}
