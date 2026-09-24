#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Whether the actor's destination, or its position when it has none, lies
 * within 16 pixels of the point (x, z). */
s32 KuupuappuHeya_IsActorNearPoint(s32 x, s32 z, s32 id)
{
    struct FieldActor *actor = Engine_ActorLookup(id);
    s32 ax;
    s32 az;

    ax = actor->target_x;
    if (ax == ACTOR_NO_TARGET)
        ax = actor->x.fixed;
    az = actor->target_z;
    if (az == ACTOR_NO_TARGET)
        az = actor->z.fixed;
    ax = (ax - x) >> 16;
    az = (az - z) >> 16;
    if (ax * ax + az * az <= 0x100)
        return 1;
    return 0;
}
