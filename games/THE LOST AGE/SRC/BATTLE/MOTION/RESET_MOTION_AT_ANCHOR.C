#include "TYPES.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 ArcTan2(s32, s32);

void BattleMotion_SetupEscapeObject(s32 object_id);
void BattleMotion_InterpolatePosition(struct BattleObjectSlot *start_slot, struct BattleObjectSlot *end_slot, s32 progress);
void BattleMotion_SetObjectPosition(struct BattleObjectSlot *slot);
void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id);

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
