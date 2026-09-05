#include "types.h"

void Func_0200817a();
void Func_020081dc();
void Func_02008204();
void Func_02008212();
void Func_0200821e();
void Func_02008266(s32, s32, s32);
void Func_02008312();
void Func_02008358();

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Func_02008266(actor, axis, offset);
}

void RunSceneEffectSetup(void)
{
    Func_020081dc();
    Func_02008212(0, 32768, 16384);
    Func_02008358(158);
    Func_0200817a(33604944, 36, 10);
    SetOffset(0, 2, -16);
    Func_02008204(16);
    Func_02008312(2);
    Func_0200821e();
}
