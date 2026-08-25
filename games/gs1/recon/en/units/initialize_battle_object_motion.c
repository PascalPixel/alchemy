#include "types.h"
#include "motion_object.h"

typedef s32 (*BattleFixedMultiply)(s32, s32);

void Object_SetMode(struct MotionObject *, s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);

#define BattleMotion_InitializeObject Func_080b81c8

void BattleMotion_InitializeObject(s32 id)
{
    struct BattleObjectSlot *slot = GetBattleObjectSlot(id);
    struct MotionObject *object = slot->object;

    object->acceleration = 128 << 9;
    object->speed_limit = 128 << 11;
    object->velocity_y = 128 << 11;
    object->vertical_motion_strength = 0x9999;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object,
                       ((BattleFixedMultiply)0x03000118)(slot->anchor_x,
                                                         0x14ccc),
                       0, slot->anchor_z);
    Object_SetMode(object, 5);
}
