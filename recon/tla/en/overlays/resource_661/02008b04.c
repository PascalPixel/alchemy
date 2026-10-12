/* Near miss, differ 6 of 59: the native stores scale_y before it forms the
   timer address, loads the party state address before its offset, and reads
   both priority bytes before it masks; this draft has each pair the other way
   round. */
#include "../../../../../games/tla/src/field/overlay_661/scene.h"

void Func_02000b04(struct ObjectRuntime *object)
{
    s32 vx = object->bounce;
    s32 vz = object->unknown_4c;
    struct FieldSprite *sprite;

    object->x += vx;
    object->y += object->gravity;
    object->z += vz;
    object->bounce = vx - vx / 18;
    object->unknown_4c = vz - vz / 16;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    sprite = object->animation;
    sprite->rotation += object->action;
    ((struct FieldSprite *)object->animation)->priority =
        Object_GetById(gPartyState.current_owner)->sprite->priority;
}
