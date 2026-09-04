#include "types.h"

#define ActorPresentation_AdvanceActorTenStates Func_02000ea8

extern void Func_02003228(s32, s32);
extern void Func_02003230(s32, s32);

void ActorPresentation_AdvanceActorTenStates(void)
{
    Func_02003228(10, 1);
    Func_02003230(10, 2);
}
