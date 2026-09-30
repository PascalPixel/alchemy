#include "TYPES.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 ArcTan2(s32, s32);

void Actor_ResetMotionAtAnchor(s32 slot_id);
void BattleMotion_SetupEscapeObject(s32 object_id);
void BattleMotion_InterpolatePosition(struct BattleObjectSlot *start_slot, struct BattleObjectSlot *end_slot, s32 progress);
void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id);

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
