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

#define Actor_ResetMotion ((void (*)(struct MotionObject *))Actor_ResetMotion)
#define Actor_SetTargetPosition                                                     \
  ((void (*)(struct MotionObject *, s32, s32, s32))Actor_SetTargetPosition)
#define Actor_SetMode ((void (*)(struct MotionObject *, s32))Actor_SetMode)

s32 ArcTan2(s32, s32);

void BattleMotion_SetObjectPosition(struct BattleObj *slot) {
  struct MotionObject *object = slot->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetTargetPosition(object, slot->anchorX, 0, slot->anchorZ);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId) {
  u32 scaledX;
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(objectId);
  object = slot->object;
  object->acceleration = Q16_ONE;
  object->speedLimit = 0x40000;
  object->velocityY = 0x30000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_ResetMotion(object);
  scaledX = slot->anchorX * 3;
  Actor_SetTargetPosition(object, (s32)(scaledX + (scaledX >> 31)) >> 1, 0,
                          slot->anchorZ);
}

void Func_0811c01c(s32 objectId) {
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(objectId);
  object = slot->object;
  object->acceleration = Q16_ONE;
  object->speedLimit = 0x40000;
  object->velocityY = 0x40000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_ResetMotion(object);
  {
    s32 (*mul)(s32 a, s32 b);
    s32 anchor = slot->anchorX;

    mul = Iwram_MulQ16;
    Actor_SetTargetPosition(object, mul(anchor, 0x14ccc), 0, slot->anchorZ);
  }
  Actor_SetMode(object, 5);
}

// The Broken Seal's gBattleMotion_Variant* tables, indexed by the variant.
extern const s32 Data_0812cbb0[]; ///< acceleration
extern const s32 Data_0812cbd8[]; ///< speed limit
extern const s32 Data_0812cc00[]; ///< upward velocity
extern const s32 Data_0812cc28[]; ///< distance, in percent of the anchor's x

/// Sends a battle object back by its variant's distance and speed, with a hop
/// when it stands on the ground or for the strong variants (5 and up). Four
/// enemies stay where they stand, and every object gets its vertical motion
/// before it takes mode 5.
// The Broken Seal's BattleMotion_ApplyVariantMotion.
void Func_0811c074(s32 id, s32 variant) {
  struct BattleObj *slot;
  struct MotionObject *object;
  s32 x;
  s32 scale;
  s32 enemyId;

  slot = BattleObject_Get(id);
  object = slot->object;
  enemyId = ((struct BattleUnit *)Unit_Get(id))->enemyId;
  if (enemyId != 357 && enemyId != 369 && enemyId != 373 && enemyId != 376) {
    object->acceleration = Data_0812cbb0[variant];
    object->speedLimit = Data_0812cbd8[variant];
    if (object->y == 0 || variant > 4)
      object->velocityY = Data_0812cc00[variant];
    Actor_ResetMotion(object);
    scale = slot->anchorX;
    x = (scale * Data_0812cc28[variant]) / 100;
    Actor_SetTargetPosition(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMode(object, 5);
}
