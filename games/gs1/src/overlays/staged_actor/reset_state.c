#include "staged_actor.h"

#define InitializeStagedActorState Func_02006488

void InitializeStagedActorState(struct StagedActor *actor, s32 state);

s32 ResetStagedActorState(struct StagedActor *actor) {
    InitializeStagedActorState(actor, 1);
    actor->x.value = 0;
    actor->y = 0;
    actor->z.value = 0;
    actor->unknown_24 = 0;
    actor->unknown_28 = 0;
    actor->unknown_2c = 0;
    actor->unknown_3c = 0x80000000;
    actor->unknown_38 = 0x80000000;
    return 0;
}
