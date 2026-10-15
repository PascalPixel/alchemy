#include "types.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_unit.h"

void *UnitGet(s32 owner);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

// BattleFx_StartActorMotion with fixed values: unless the unit is one of
// four enemies, the actor steps 15 pixels further from x = 0 on its
// anchor's side, its y velocity growing by 0x28000 while y is under 10.
void Func_0817b5d4(s32 id) {
  struct BattleObj *slot;
  struct MotionObject *object;
  u32 enemy;
  s32 x;

  slot = GetBattleObjectSlotFar(id);
  object = slot->object;
  enemy = ((struct BattleUnit *)UnitGet(id))->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    object->acceleration = 0x20000;
    /* FAKEMATCH: the one-pass block's loop note holds the speed limit's store
       ahead of the height test; plain, the test's 0xa0000 is started before
       object->y is loaded (one reordered pair, differ 2). */
    do {
      object->speedLimit = 0x80000;
    } while (0);
    if (object->y < 0xa0000)
      object->velocityY += 0x28000;
    Object_ResetMotion(object);
    if (slot->anchorX > 0)
      x = object->x + 0xf0000;
    else
      x = object->x - 0xf0000;
    Object_SetPosition(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_SetMode(object, 5);
}
