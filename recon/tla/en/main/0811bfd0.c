#include "TYPES.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 ArcTan2(s32, s32);

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
