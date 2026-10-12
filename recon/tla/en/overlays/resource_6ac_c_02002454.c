/* Near miss, differ 2 of 47: the listing loads the animation after it stores the Y scale;
 * this draft loads it before. A second empty barrier there moves the building of 0x1000 after it instead. */
#include "../../../../games/tla/src/field/overlay_6ac/ov6ac.h"

void Func_02002454(struct ObjectRuntime *object)
{
    s32 vx;
    s32 vz;
    s32 rate;

    Object_GetById(gPartyState.current_owner);
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
    object->scale_y += object->acceleration;
    ((struct Ov6acAnimation *)object->animation)->angle += 0x1000;
}
