#include "TYPES.H"
#include "FIELD_EVENT.H"

void KuupuappuDou_RaiseActorPriorities(void)
{
    s32 i;
    s32 id;

    id = 16;
    for (i = 0; i < 6; i++) {
        Engine_ActorGet(id)->priority_flags |= 2;
        id++;
    }
}
