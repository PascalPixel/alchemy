#include "TYPES.H"
#include "FIELD_EVENT.H"

void ArutamiraDou_MatchLeaderPriority(struct FieldActor *actor)
{
    if (actor != NULL) {
        actor->priority_flags = 0;
        actor->sprite->priority = Engine_ActorGet(0)->sprite->priority;
    }
}
