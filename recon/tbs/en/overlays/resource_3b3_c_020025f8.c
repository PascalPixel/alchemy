/* NONMATCHING: 66 of 68 bytes (padding). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Local_020025f8(s32 id)
{
    struct FieldActor *actor = Engine_ActorGet(id);

    actor->collision_flags &= 254;
    actor->priority_flags |= 2;
    actor->motion_flags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->sprite->priority = 2;
}
