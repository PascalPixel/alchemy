#include "types.h"
#include "system.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_unit.h"

void *UnitGet(s32 owner);
void  Object_ResetMotion(struct MotionObject *object);
void  Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void  Object_SetMode(struct MotionObject *object, s32 mode);

void Func_08163d88(s32 id)
{
    struct BattleObj    *slot;
    struct MotionObject *object;
    u32                  enemy;

    slot = GetBattleObjectSlotFar(id);
    object = slot->object;
    enemy = ((struct BattleUnit *)UnitGet(id))->enemyId;
    if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376)
    {
        s32 percent = (Random16() & 63) + 130;

        object->acceleration = 0x80000;
        object->speedLimit = 0xa0000;
        object->velocityY = 0x18000;
        Object_ResetMotion(object);
        Object_SetPosition(object, slot->anchorX * percent / 100, 0, slot->anchorZ);
    }
    object->verticalMotionStrength = 0x9999;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 0;
    Object_SetMode(object, 5);
}
