#include "TYPES.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 ArcTan2(s32, s32);

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
