#include "types.h"

#define ActorPresentation_RunActorModeOneThenZeroWithStep Func_02000354
#define SceneActor_SetModeZeroAndValue Func_020045f4
#define SceneActor_SetPairZeroAndValue Func_02004624
void Func_02005130(s32, s32);
void Func_0200498a(s32, s32, s32);
void Func_02005192(s32, s32);
void Func_02009420(s32, s32);
void Func_0200933e(s32);
void Func_02009438(s32, s32, s32);
void Func_0200936e(s32);

void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x)
{
    Func_02005130(x, 1);
    Func_0200498a(x, 0, 2);
    Func_02005192(x, 0);
}

void SceneActor_SetModeZeroAndValue(s32 a, s32 b)
{
    Func_02009420(a, 0);
    Func_0200933e(b);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Func_02009438(a, b, 0);
    Func_0200936e(c);
}
