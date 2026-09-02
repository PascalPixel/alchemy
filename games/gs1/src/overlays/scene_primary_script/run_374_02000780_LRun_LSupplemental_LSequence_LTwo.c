#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_02000780

void Func_02000ef4();
void Func_02002d8c();
void Func_02002da2();
void Func_02002dde();
void Func_02002df4();
void Func_02002ec8();
void Func_02002ede();
void Func_02002f60();
void Func_02002fd4();
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


void Func_02000780(void)
{
    Call1(Func_02002fd4, 188);
    Call6(Func_02002dde, 0, 63, 51, 8, 2, 2);
    Call1(Func_02002d8c, 10);
    Call6(Func_02002df4, 2, 63, 51, 8, 2, 2);
    Call1(Func_02002da2, 10);
    Call3(Func_02002ec8, 0, 352, 306);
    Call2(Func_02002f60, 0, 3);
    Call3(Func_02002ede, 0, 352, 296);
    Call1(Func_02000ef4, 2);
}
