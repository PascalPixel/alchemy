#include "types.h"
#include "motobj.h"
#include "battle_action.h"
#include "battle_unit.h"
#include "actor.h"

void *Unit_Get(s32 owner);
void Actor_ResetMotion(struct Actor *);
void Actor_SetTargetPosition(struct Actor *, s32, s32, s32);


void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent) {
  struct BattleObj *slot;
  struct MotionObject *object;
  u32 enemy;

  slot = GetBattleObjectSlotFar(id);
  object = slot->object;
  enemy = ((struct BattleUnit *)Unit_Get(id))->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
    if (force != 0 || object->velocityY != 0)
      object->velocityY = velocityY;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, slot->anchorX * percent / 100, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMode(object, 5);
}
