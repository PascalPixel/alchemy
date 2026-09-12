#include "types.h"
#include "scene.h"
#include "motion_object.h"
#include "battle_motion.h"

/* battle/motion/apply_variant_motion.c */
struct BattleObjectSlot *GetBattleObjectSlot();
u8 *Runtime_GetObject(s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 FixedPoint_Ratio(s32, s32);

extern s32 gRom[];
extern s32 gRom2[];
extern s32 gRom3[];
extern s32 gRom4[];

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 index;
    s32 x;
    s32 *table;
    s32 scale;

    slot = GetBattleObjectSlot(id);
    object = slot->object;
    if (Runtime_GetObject(id)[0x128] != 0x94) {
        table = gRom;
        index = variant * 4;
        object->acceleration = *(s32 *)((u8 *)table + index);
        table = gRom2;
        object->speed_limit = *(s32 *)((u8 *)table + index);
        if (object->y == 0 || variant > 4) {
            table = gRom3;
            object->velocity_y = *(s32 *)((u8 *)table + index);
        }
        object->vertical_motion_strength = 0x9999;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 0;
        Object_ResetMotion(object);
        scale = slot->anchor_x;
        table = gRom4;
        x = FixedPoint_Ratio(scale * *(s32 *)((u8 *)table + index), 100);
        Object_SetPosition(object, x, 0, slot->anchor_z);
    }
    Object_SetMode(object, 5);
}

/* battle/motion/approach_target.c */

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
    s32 delta_x = FixedPoint_Ratio(scale *difference_x, 100);
    s32 difference_z = target->z - object->z;
    s32 start_z = object->z;
    s32 delta_z = FixedPoint_Ratio(scale *difference_z, 100);
    s32 x = start_x + delta_x;
    s32 z = start_z + delta_z;
    s32 short_x = delta_x >> 8;
    s32 short_z = delta_z >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(
        short_x *short_x + short_z *short_z);
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

/* battle/motion/reset_slot_object_mode2.c */

void BattleMotion_ResetSlotObjectMode2(void)
{
    s32 object;

    object = (s32)GetBattleObjectSlot()->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}

/* battle/motion/reserved_no_op.c */
void BattleMotion_ReservedNoOp83B0()
{
}
