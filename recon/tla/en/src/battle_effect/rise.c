#include "motobj.h"
#include "b5_context.h"
#include "types.h"
#include "actor.h"
#include "iwram_call.h"

void BattleEffect_InitializeRisingObject(s32 slot) {
  struct BattleObj *context = GetBattleObjectSlotFar(slot);
  struct Actor *object = context->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x40000;
  object->gravity = 0xAB85;
  object->bounce = 0;
  object->actionFlags = 0;
  object->snapToTarget = 1;
  Actor_ResetMotion(object);
  Actor_SetTargetPosition(object, context->anchorX, 0, context->anchorZ);
  Actor_SetMode(object, 1);
}

/// Moves the object rate percent of the way to the target's object in the
/// given frames.
// The Broken Seal's BattleEffect_SetApproachMotion always moves 80 percent.
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate) {
  struct BattleObj *slot = GetBattleObjectSlotFar(id);
  struct BattleObj *targetSlot = GetBattleObjectSlotFar(targetId);
  struct Actor *obj = (struct Actor *)slot->object;
  struct Actor *target = (struct Actor *)targetSlot->object;
  s32 diffX = target->x - obj->x;
  s32 startX = obj->x;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z - obj->z;
  s32 startZ = obj->z;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 sx = dx >> 8; // the distance in 1/256 pixels
  s32 sz = dz >> 8;
  s32 dist;

  dist = Iwram_Sqrt(sx * sx + sz * sz);
  dist = (dist << 8) / frames;
  obj->acceleration = dist;
  obj->speedLimit = dist;
  obj->snapToTarget = 1;
  obj->gravity = 0xab85;
  obj->velocityY = 0;
  obj->bounce = 0;
  obj->actionFlags = 1;
  Actor_ResetMotion(obj);
  Actor_SetTargetPosition(obj, x, 0, z);
  Actor_SetMode(obj, 2);
}
