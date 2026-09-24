#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Draw the actor, both parts, at the leader's sprite priority. */
s32 ShindenHeya_MatchLeaderPriority(struct FieldActor *actor)
{
    actor->sprite->priority = Engine_ActorGet(0)->sprite->priority;
    actor->sprite->second_priority = Engine_ActorGet(0)->sprite->priority;
    return 0;
}
