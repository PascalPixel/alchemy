#include "types.h"
#include "field_event.h"

void SceneActor_ParkRecord(struct FieldActor *actor)
{
    actor->target_x = ACTOR_NO_TARGET;
    actor->target_y = ACTOR_NO_TARGET;
    actor->target_z = ACTOR_NO_TARGET;

    actor->velocity_x = 0;
    actor->velocity_y = 0;
    actor->velocity_z = 0;

    actor->timer = 0;
}
