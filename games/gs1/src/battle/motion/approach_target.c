#include "battle_motion.h"
#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *GetBattleObjectSlot(s32);
s32 FixedPoint_Ratio(s32, s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);

void BattleMotion_ApproachTarget(
    s32 actor_id,
    s32 target_id,
    s32 travel_divisor,
    s32 initial_velocity_y
)
{
    struct BattleObjectSlot *actor_slot = GetBattleObjectSlot(actor_id);
    struct BattleObjectSlot *target_slot = GetBattleObjectSlot(target_id);
    struct MotionObject *object = actor_slot->object;
    struct MotionObject *target = target_slot->object;
    s32 scale = 75;
    s32 difference_x = target->x - object->x;
    s32 start_x = object->x;
    s32 delta_x = FixedPoint_Ratio(scale * difference_x, 100);
    s32 difference_z = target->z - object->z;
    s32 start_z = object->z;
    s32 delta_z = FixedPoint_Ratio(scale * difference_z, 100);
    s32 x = start_x + delta_x;
    s32 z = start_z + delta_z;
    s32 short_x = delta_x >> 8;
    s32 short_z = delta_z >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(
        short_x * short_x + short_z * short_z);
    distance = FixedPoint_Ratio(distance << 8, travel_divisor);
    object->acceleration = distance;
    object->speed_limit = distance;
    object->snap_to_target = 1;
    if (object->motion_flags & 4)
        object->velocity_y = initial_velocity_y;
    object->velocity_y = initial_velocity_y;
    object->vertical_motion_strength = 0xab85;
    object->auto_face_motion = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
}
