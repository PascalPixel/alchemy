#include "TYPES.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_MOTION.H"
#include "FIXED_MATH.H"

u8 *Owner_GetStateFar(s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);

extern s32 BattleMotion_VariantAcceleration[];
extern s32 BattleMotion_VariantSpeedLimit[];
extern s32 BattleMotion_VariantVelocityY[];
extern s32 BattleMotion_VariantDistancePercent[];

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    /* FAKEMATCH: Byte indexing preserves the load operand order. */
    s32 offset;
    s32 x;
    s32 *table;
    s32 scale;

    slot = GetBattleObjectSlot(id);
    object = slot->object;
    if (Owner_GetStateFar(id)[0x128] != 0x94) {
        table = BattleMotion_VariantAcceleration;
        offset = variant * sizeof(*table);
        object->acceleration = *(s32 *)((u8 *)table + offset);
        table = BattleMotion_VariantSpeedLimit;
        object->speed_limit = *(s32 *)((u8 *)table + offset);
        if (object->y == 0 || variant > 4) {
            table = BattleMotion_VariantVelocityY;
            object->velocity_y = *(s32 *)((u8 *)table + offset);
        }
        object->vertical_motion_strength = 0x9999;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 0;
        Object_ResetMotion(object);
        scale = slot->anchor_x;
        table = BattleMotion_VariantDistancePercent;
        x = Math_Div(scale * *(s32 *)((u8 *)table + offset), 100);
        Object_SetPosition(object, x, 0, slot->anchor_z);
    }
    Object_SetMode(object, 5);
}

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
    s32 dx = target->x - object->x;
    s32 start_x = object->x;
    s32 step_x = Math_Div(scale * dx, 100);
    s32 dz = target->z - object->z;
    s32 start_z = object->z;
    s32 step_z = Math_Div(scale * dz, 100);
    s32 x = start_x + step_x;
    s32 z = start_z + step_z;
    s32 cell_x = step_x >> 8;
    s32 cell_z = step_z >> 8;
    s32 dist;

    dist = ((s32 (*)(s32))0x030001d8)(
        cell_x * cell_x + cell_z * cell_z);
    dist = Math_Div(dist << 8, travel_divisor);
    object->acceleration = dist;
    object->speed_limit = dist;
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

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
    struct MotionObject *object;

    object = GetBattleObjectSlot(id)->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}

void BattleMotion_ReservedNoOp83B0()
{
}
