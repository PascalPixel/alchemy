#include "types.h"

#define ActorPresentation_AdvanceActorElevenStates Func_02001bfc
void Func_02003900(s32 actor, s32 state);
void Func_02003908(s32 actor, s32 state);

/* Advance actor eleven through the two presentation states used at scene end. */

void ActorPresentation_AdvanceActorElevenStates(void)
{
    Func_02003900(11, 1);
    Func_02003908(11, 2);
}
