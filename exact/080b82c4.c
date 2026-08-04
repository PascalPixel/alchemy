#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *Func_080b7dd0(s32);
s32 Func_080022ec(s32, s32);
void Func_08009140(struct MotionObject *);
void Func_08009150(struct MotionObject *, s32, s32, s32);
void Func_08009080(struct MotionObject *, s32);

void Func_080b82c4(
    s32 first,
    s32 second,
    s32 divisor,
    s32 initial_velocity_y
)
{
    struct BattleObjectSlot *first_slot = Func_080b7dd0(first);
    struct BattleObjectSlot *second_slot = Func_080b7dd0(second);
    struct MotionObject *object = first_slot->object;
    struct MotionObject *target = second_slot->object;
    s32 scale = 75;
    s32 difference_x = target->x - object->x;
    s32 start_x = object->x;
    s32 delta_x = Func_080022ec(scale * difference_x, 100);
    s32 difference_z = target->z - object->z;
    s32 start_z = object->z;
    s32 delta_z = Func_080022ec(scale * difference_z, 100);
    s32 x = start_x + delta_x;
    s32 z = start_z + delta_z;
    s32 short_x = delta_x >> 8;
    s32 short_z = delta_z >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(
        short_x * short_x + short_z * short_z);
    distance = Func_080022ec(distance << 8, divisor);
    object->acceleration = distance;
    object->speed_limit = distance;
    object->snap_to_target = 1;
    if (object->motion_flags & 4)
        object->velocity_y = initial_velocity_y;
    object->velocity_y = initial_velocity_y;
    object->vertical_motion_strength = 0xab85;
    object->auto_face_motion = 1;
    Func_08009140(object);
    Func_08009150(object, x, 0, z);
    Func_08009080(object, 2);
}
