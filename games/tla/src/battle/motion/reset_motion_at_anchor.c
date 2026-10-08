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
#include "btlwork.h"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)

s32 ArcTan2(s32, s32);


struct BtlObj *BtlObjGet(s32 object_id)
{
    struct BtlWork *work = Ram_HeapSlots->battle_work;
    struct BtlObj *slots = work->slots;
    s32 index;

    if (object_id > 7) {
        object_id -= 0x78;
    }
    index = object_id;
    if (work->placement[index] == 0xFF) {
        return NULL;
    }
    return &slots[work->placement[index]];
}

void ObjMotRst(s32 slot_id)
{
    s32 z;
    s32 zero;
    struct BtlObj *slot;
    struct MotionObject *object;

    slot = BtlObjGet(slot_id);
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
    struct BtlObj *slot;
    struct MotionObject *object;

    slot = BtlObjGet(object_id);
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
