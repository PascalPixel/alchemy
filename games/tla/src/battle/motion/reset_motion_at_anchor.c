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
#include "battle_work.h"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)

s32 ArcTan2(s32, s32);

struct BattleObj *BattleObjGet(s32 objectId)
{
    struct BattleWork *work = Ram_HeapSlots->battleWork;
    struct BattleObj  *slots = work->slots;
    s32                index;

    if (objectId > 7)
    {
        objectId -= 0x78;
    }
    index = objectId;
    if (work->placement[index] == 0xFF)
    {
        return NULL;
    }
    return &slots[work->placement[index]];
}

void ObjMotRst(s32 slotId)
{
    s32                  z;
    s32                  zero;
    struct BattleObj    *slot;
    struct MotionObject *object;

    slot = BattleObjGet(slotId);
    object = slot->object;
    zero = 0;
    object->acceleration = 0x20000;
    object->speedLimit = 0x80000;
    object->verticalMotionStrength = 0xab85;
    object->velocityY = zero;
    object->verticalMotionPhase = zero;
    object->autoFaceMotion = zero;
    object->snapToTarget = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchorX, 0, slot->anchorZ);
    z = slot->anchorZ;
    if (z < 0)
        z += 7;
    object->angle = (s16)(ArcTan2(z >> 3, slot->anchorX) - 0x8000);
}

void BattleMotion_SetupEscapeObject(s32 objectId)
{
    struct BattleObj    *slot;
    struct MotionObject *object;

    slot = BattleObjGet(objectId);
    object = slot->object;
    object->acceleration = 0x20000;
    object->speedLimit = 0x80000;
    object->velocityY = 0x50000;
    object->verticalMotionStrength = 0x7851;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchorX * 3, 0, slot->anchorZ);
    Object_SetMode(object, 1);
}
