/* Near miss of Func_02000508 of overlay 696 (51 instructions, 4 differ): in the first arm the
 * native loads the scale between `movs r2, #192` and its shift; the draft loads it after the
 * shift. Measured: a one-pass block with the step as a local, and the scale behind a "+l"
 * barrier, both load it before the first byte (2 differ). The chained store in each arm is
 * what lets the two arms share their tail as the native does. */
#include "../../../../games/tla/src/field/overlay_696/ov696.h"

void Object_SetSpritePriority(struct Ov696Object *object, s32 priority);

void Func_02000508(struct Ov696Object *object)
{
    object->x += object->field_44;
    object->y += object->field_48;
    object->z += *(s32 *)((u8 *)object + 76);
    if (object->x >> 16 < 168) {
        object->field_44 += 0x18000;
        object->scale_y = object->scale_x += 0xccc;
    } else {
        object->field_44 -= 0x18000;
        object->scale_y = object->scale_x -= 0x51e;
    }
    if (object->field_44 > 0)
        Object_SetSpritePriority(object, 1);
    else
        Object_SetSpritePriority(object, 2);
    object->sprite->rotation += 0x1800;
}
