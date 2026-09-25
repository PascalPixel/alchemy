#include "TYPES.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 ArcTan2(s32, s32);

void Actor_ResetMotionAtAnchor(s32 slot_id)
{
    s32 z;
    s32 zero;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;

    slot = GetBattleObjectSlot(slot_id);
    object = slot->object;
    zero = 0;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xab85;
    object->velocity_y = zero;
    object->vertical_motion_phase = zero;
    object->auto_face_motion = zero;
    object->snap_to_target = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchor_x, 0, slot->anchor_z);
    z = slot->anchor_z;
    if (z < 0)
        z += 7;
    object->angle =
        (s16)(ArcTan2(z >> 3, slot->anchor_x) - 0x8000);
}

void BattleMotion_SetupEscapeObject(s32 object_id)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;

    slot = GetBattleObjectSlot(object_id);
    object = slot->object;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->velocity_y = 0x50000;
    object->vertical_motion_strength = 0x7851;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchor_x * 3, 0, slot->anchor_z);
    Object_SetMode(object, 1);
}

void BattleMotion_InterpolatePosition(struct BattleObjectSlot *start_slot,
    struct BattleObjectSlot *end_slot, s32 progress)
{
    s32 z_step;
    s32 start_x;
    s32 x;
    s32 start_z;
    s32 end_z;
    struct MotionObject *start;
    struct MotionObject *end;

    start = start_slot->object;
    end = end_slot->object;
    start_x = start->x;
    x = start_x + Math_Div(progress * (end->x - start_x), 100);
    end_z = end->z;
    start_z = start->z;
    z_step = Math_Div(progress * (end_z - start_z), 100);
    *(s16 *)0x04000050 = 0;
    start->acceleration = 0x20000;
    start->speed_limit = 0x80000;
    start->velocity_y = 0x40000;
    start->vertical_motion_strength = 0xab85;
    start->vertical_motion_phase = 0;
    start->auto_face_motion = 1;
    Object_SetPosition(start, x, 0, start_z + z_step);
    Object_SetMode(start, 2);
}

void BattleMotion_SetObjectPosition(struct BattleObjectSlot *slot)
{
    struct MotionObject *object = slot->object;

    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xab85;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_SetPosition(object, slot->anchor_x, 0, slot->anchor_z);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id)
{
    u32 scaled_x;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;

    slot = GetBattleObjectSlot(object_id);
    object = slot->object;
    object->acceleration = 0x10000;
    object->speed_limit = 0x40000;
    object->velocity_y = 0x30000;
    object->vertical_motion_strength = 0x9999;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    scaled_x = slot->anchor_x * 3;
    Object_SetPosition(object, (s32)(scaled_x + (scaled_x >> 0x1F)) >> 1, 0, slot->anchor_z);
}
