/* Near miss, differ 2 of 94 (one reordered pair): the native stores the new
 * scale_y before it addresses the timer at offset 100; the draft addresses
 * it first. A one-pass block after the scale stores moves another pair
 * instead; a timer pointer and a sprite local were tried. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

s32 Func_02000824(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02000864(struct ObjectRuntime *object)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *other = Object_GetById(8);
    s32 vx = object->bounce;
    s32 vz;

    object->x += vx;
    object->y += object->gravity;
    vz = object->unknown_4c;
    object->z += vz;
    object->bounce = vx - vx / 18;
    object->unknown_4c = vz - vz / 16;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    asm("");
    ((struct FieldSprite *)object->animation)->rotation += object->action;
    if (leader->y == 0) {
        if (Func_02000824(object, leader)) {
            object->z -= object->unknown_4c;
            object->gravity += object->unknown_4c >> 2;
            object->unknown_4c = 0;
        }
        if (leader->y == 0 && Func_02000824(object, other)) {
            object->z -= object->unknown_4c;
            object->gravity += object->unknown_4c >> 2;
            object->unknown_4c = 0;
        }
    }
}

