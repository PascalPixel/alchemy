#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "fixed_math.h"
#include "motobj.h"
#include "btlobj.h"
#include "iwram_call.h"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)

s32 ArcTan2(s32, s32);

void BattleMotion_SetObjectPosition(struct BtlObj *slot)
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
    struct BtlObj *slot;
    struct MotionObject *object;

    slot = BtlObjGet(object_id);
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

void Func_0811c01c(s32 object_id)
{
    struct BtlObj *slot;
    struct MotionObject *object;

    slot = BtlObjGet(object_id);
    object = slot->object;
    object->acceleration = 0x10000;
    object->speed_limit = 0x40000;
    object->velocity_y = 0x40000;
    object->vertical_motion_strength = 0x9999;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    {
        s32 (*mul)(s32 a, s32 b);
        s32 anchor = slot->anchor_x;

        /* FAKEMATCH: ⚓️ loads the multiplier only after the anchor */
        asm("" : "+l"(anchor));
        mul = Iwram_MulQ16;
        Object_SetPosition(object, mul(anchor, 0x14ccc), 0, slot->anchor_z);
    }
    Object_SetMode(object, 5);
}
