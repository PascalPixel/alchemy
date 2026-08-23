#include "b5_context.h"
#include "types.h"
#include "motion_object.h"

s32 FixedPoint_Ratio(s32, s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);

void Func_080df90c(s32 first, s32 second, s32 divisor) {
    struct B5Context *first_context = Func_080b5098(first);
    struct B5Context *second_context = Func_080b5098(second);
    struct MotionObject *object = first_context->object;
    struct MotionObject *target = second_context->object;
    s32 scale = 80;
    s32 differenceX = target->x - object->x;
    s32 startX = object->x;
    s32 deltaX = FixedPoint_Ratio(scale * differenceX, 100);
    s32 differenceZ = target->z - object->z;
    s32 startZ = object->z;
    s32 deltaZ = FixedPoint_Ratio(scale * differenceZ, 100);
    s32 x = startX + deltaX;
    s32 z = startZ + deltaZ;
    s32 shortX = deltaX >> 8;
    s32 shortZ = deltaZ >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(shortX * shortX + shortZ * shortZ);
    distance = FixedPoint_Ratio(distance << 8, divisor);
    object->acceleration = distance;
    object->speed_limit = distance;
    object->snap_to_target = 1;
    object->vertical_motion_strength = 0xab85;
    object->velocity_y = 0;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
}
