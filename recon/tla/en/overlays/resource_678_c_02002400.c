/* Draft, differ 60 of 114: the native keeps the leader in r4 and saves it around the division (caller-save,
 * so it counts at least five references), and the work in r8; here the leader has four references,
 * caller-save is not profitable and it takes r8, pushing the work to sl. Tried: an index loop (69),
 * the leader check in a one-pass block (68, the references stay four). */
#include "../../../../games/tla/src/field/overlay_678/local.h"

s32 Func_020023a8(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02002400(struct ObjectRuntime *object)
{
    struct VentWork *work = VENT_WORK;
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 dx = object->bounce;
    s32 dz;
    struct FieldSprite *sprite;
    s32 i;
    u16 *id;

    object->x += dx;
    object->y += object->gravity;
    dz = object->unknown_4c;
    object->z += dz;
    object->bounce = dx - dx / 18;
    object->unknown_4c = dz - dz / 16;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    sprite = object->animation;
    sprite->rotation += object->action;
    if (leader->y == leader->terrain_height && Func_020023a8(object, leader)) {
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 2;
        object->unknown_4c = 0;
    }
    for (i = 0, id = work->ids; i < 4 && *id != 0; i++, id++) {
        struct ObjectRuntime *other = Object_GetById(*id);

        if (other->y == other->terrain_height && Func_020023a8(object, other)) {
            object->z -= object->unknown_4c;
            object->gravity += object->unknown_4c >> 2;
            object->unknown_4c = 0;
        }
    }
}
