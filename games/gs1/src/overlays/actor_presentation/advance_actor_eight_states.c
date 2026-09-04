#include "types.h"

#define ActorPresentation_AdvanceActorEightStates Func_02000e78

extern void Func_020031f8(s32, s32);
extern void Func_02003200(s32, s32);

void ActorPresentation_AdvanceActorEightStates(void)
{
    Func_020031f8(8, 1);
    Func_02003200(8, 2);
}
