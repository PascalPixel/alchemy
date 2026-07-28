#include "types.h"
#include "motion_object.h"

struct MotionObject **Func_080b5098(s32);
s32 Func_080022ec(s32, s32);
void Func_08009140(struct MotionObject *);
void Func_08009150(struct MotionObject *, s32, s32, s32);
void Func_08009080(struct MotionObject *, s32);

void Func_080df90c(s32 first, s32 second, s32 divisor) {
    struct MotionObject **first_context = Func_080b5098(first);
    struct MotionObject **second_context = Func_080b5098(second);
    struct MotionObject *object = *first_context;
    struct MotionObject *target = *second_context;
    s32 scale = 80;
    s32 differenceX = target->x - object->x;
    s32 startX = object->x;
    s32 deltaX = Func_080022ec(scale * differenceX, 100);
    s32 differenceZ = target->z - object->z;
    s32 startZ = object->z;
    s32 deltaZ = Func_080022ec(scale * differenceZ, 100);
    s32 x = startX + deltaX;
    s32 z = startZ + deltaZ;
    s32 shortX = deltaX >> 8;
    s32 shortZ = deltaZ >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(shortX * shortX + shortZ * shortZ);
    distance = Func_080022ec(distance << 8, divisor);
    object->acceleration = distance;
    object->speed_limit = distance;
    object->snap_to_target = 1;
    object->vertical_motion_strength = 0xab85;
    object->velocity_y = 0;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 1;
    Func_08009140(object);
    Func_08009150(object, x, 0, z);
    Func_08009080(object, 2);
}
