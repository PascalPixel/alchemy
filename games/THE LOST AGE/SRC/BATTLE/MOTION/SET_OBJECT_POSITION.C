#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_ACTOR.H"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)

s32 ArcTan2(s32, s32);


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
