#include "ov6ac.h"

void Func_02001e38(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target = Object_GetById(gPartyState.current_owner);
    s32 vx;
    s32 vz;
    s32 rate;
    struct Ov6acAnimation *animation;

    vx = object->bounce;
    object->x += vx;
    object->y += object->gravity;
    vz = object->unknown_4c;
    object->z += vz;
    object->bounce = vx - vx / 18;
    object->unknown_4c = vz - vz / 16;
    /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the Z velocity. */
    __asm__("");
    rate = object->speed_limit;
    object->scale_x += rate;
    animation = object->animation;
    object->scale_y += object->acceleration;
    /* FAKEMATCH: and would address the spin before storing the Y scale. */
    __asm__("");
    animation->angle += object->action;
    if (target->y == target->terrain_height && Func_02001de0(object, target)) {
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 2;
        object->unknown_4c = 0;
    }
}
