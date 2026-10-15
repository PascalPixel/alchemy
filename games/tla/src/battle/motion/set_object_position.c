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
#include "battle_obj.h"
#include "iwram_call.h"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition                                                     \
  ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)

s32 ArcTan2(s32, s32);

void BattleMotion_SetObjectPosition(struct BattleObj *slot) {
  struct MotionObject *object = slot->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_SetPosition(object, slot->anchorX, 0, slot->anchorZ);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId) {
  u32 scaledX;
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObjGet(objectId);
  object = slot->object;
  object->acceleration = 0x10000;
  object->speedLimit = 0x40000;
  object->velocityY = 0x30000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_ResetMotion(object);
  scaledX = slot->anchorX * 3;
  Object_SetPosition(object, (s32)(scaledX + (scaledX >> 0x1F)) >> 1, 0,
                     slot->anchorZ);
}

void Func_0811c01c(s32 objectId) {
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObjGet(objectId);
  object = slot->object;
  object->acceleration = 0x10000;
  object->speedLimit = 0x40000;
  object->velocityY = 0x40000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_ResetMotion(object);
  {
    s32 (*mul)(s32 a, s32 b);
    s32 anchor = slot->anchorX;

    mul = Iwram_MulQ16;
    Object_SetPosition(object, mul(anchor, 0x14ccc), 0, slot->anchorZ);
  }
  Object_SetMode(object, 5);
}
