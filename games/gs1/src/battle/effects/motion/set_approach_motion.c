#include "b5_context.h"
#include "types.h"
#include "motion_object.h"

s32 FixedPoint_Ratio(s32, s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);

void BattleEffect_SetApproachMotion(s32 first, s32 second, s32 divisor) {
    struct B5Context *first_context = Func_080b5098(first);
    struct B5Context *second_context = Func_080b5098(second);
    struct MotionObject *object = first_context->object;
    struct MotionObject *target = second_context->object;
    s32 scale = 80;
    s32 diff_x = target->x - object->x;
    s32 start_x = object->x;
    s32 delta_x = FixedPoint_Ratio(scale * diff_x, 100);
    s32 diff_z = target->z - object->z;
    s32 start_z = object->z;
    s32 delta_z = FixedPoint_Ratio(scale * diff_z, 100);
    s32 x = start_x + delta_x;
    s32 z = start_z + delta_z;
    s32 short_x = delta_x >> 8;
    s32 short_z = delta_z >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(short_x * short_x + short_z * short_z);
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
