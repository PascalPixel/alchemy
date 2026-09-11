#include "scene.h"
#include "object_runtime.h"

/* object/motion/aim/is_target_unset.c */
s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->flags == 0) {
        second = object->target_x;
        if (second == 0x80000000) {
            first = object->target_y;
            goto block_4;
        }
        goto block_6;
    }
    first = object->target_x;
    second = 0x80000000;
block_4:
    if ((first != second) || (object->target_z != first)) {
block_6:
        return 0;
    }
    return 1;
}

/* object/motion/pos/set_position_and_reset_motion.c */
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    object->z = z;
    object->target_x = 0x80000000;
    object->target_y = 0x80000000;
    object->target_z = 0x80000000;
    object->x = x;
    object->y = y;
    object->velocity_x = 0;
    object->velocity_y = 0;
    object->velocity_z = 0;
}
