#include "object_runtime.h"

void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void ObjectMotion_SetActionVariant(u32 object_id, s32 action);

void ObjectMotion_SnapHeadingAndOffset(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 current_angle;
    s32 snapped_angle;
    s16 angle_remainder;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        current_angle = *(s16 *)((u8 *)object + 0x0a);
        snapped_angle = current_angle;
        if (current_angle < 0) {
            snapped_angle += 15;
        }
        snapped_angle >>= 4;
        snapped_angle *= 16;
        angle_remainder = current_angle - snapped_angle;
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object,
            object->x + ((8 - angle_remainder) << 16),
            object->y, object->z);
        Object_CommitPosition(object);
        ObjectMotion_SetActionVariant(object_id, action);
        Object_SetPosition(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}
