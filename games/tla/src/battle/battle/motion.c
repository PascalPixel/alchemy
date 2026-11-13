/// @file
/// The units' movements: returning to their spots, running and leaping, and
/// their positions on the screen.
// Part of battle.c.

#include "../battle_internal.h"
#include "module/battle/battle.h"
#include "gamepak/far_call/field.h"

#define Actor_ResetMotion ((void (*)(struct MotionObject *))Actor_ResetMotion)
#define Actor_SetTargetPosition                                                \
  ((void (*)(struct MotionObject *, s32, s32, s32))Actor_SetTargetPosition)
#define Actor_SetMode ((void (*)(struct MotionObject *, s32))Actor_SetMode)

void BattleMotion_ReturnToSpot(s32 slotId) {
  s32 z;
  s32 zero;
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(slotId);
  object = slot->object;
  zero = 0;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->velocityY = zero;
  object->verticalMotionPhase = zero;
  object->autoFaceMotion = zero;
  object->snapToTarget = 1;
  Actor_ResetMotion(object);
  Actor_SetTargetPosition(object, slot->anchorX, 0, slot->anchorZ);
  z = slot->anchorZ;
  if (z < 0)
    z += 7;
  object->angle = (s16)(ArcTan2(z >> 3, slot->anchorX) - 0x8000);
}

void BattleMotion_SetupEscapeObject(s32 objectId) {
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(objectId);
  object = slot->object;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x50000;
  object->verticalMotionStrength = 0x7851;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_ResetMotion(object);
  Actor_SetTargetPosition(object, slot->anchorX * 3, 0, slot->anchorZ);
  Actor_SetMode(object, 1);
}

/// Sends the actor of startSlot leaping progress percent of the way toward
/// the actor of endSlot, in actor mode 2. Also clears REG_BLDCNT. Nothing in
/// the source calls it.
void BattleMotion_LeapToward(struct Actor **startSlot, struct Actor **endSlot,
                             s32 progress) {
  struct Actor *start = *startSlot;
  struct Actor *end = *endSlot;
  s32 x;
  s32 z;

  x = start->x + (end->x - start->x) * progress / 100;
  z = start->z + (end->z - start->z) * progress / 100;
  REG_BLDCNT = 0;
  start->acceleration = 0x20000;
  start->speedLimit = 0x80000;
  start->velocityY = 0x40000;
  start->gravity = 0xab85;
  start->bounce = 0;
  start->actionFlags = 1;
  Actor_SetTargetPosition(start, x, 0, z);
  Actor_SetMode(start, 2);
}

void BattleMotion_ReturnObjectToSpot(struct BattleObj *slot) {
  struct MotionObject *object = slot->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetTargetPosition(object, slot->anchorX, 0, slot->anchorZ);
}

void BattleMotion_HopBack(s32 objectId) {
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

/// Sends unit id leaping back to 1.3 times its spot's distance from the
/// centre, in POSE_DAMAGE.
void BattleMotion_LeapBack(s32 objectId) {
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

/// Sends a battle object back by its variant's distance and speed, with a hop
/// when it stands on the ground or for the strong variants (5 and up). Four
/// enemies stay where they stand, and every object gets its vertical motion
/// before it takes mode 5.
// The Broken Seal's BattleMotion_ApplyVariantMotion.
/// Knocks unit id back by the speed, hop and distance of the knockback
/// variant (enum BattleKnockback), and puts it in POSE_DAMAGE.
/// The Fusion Dragon only takes the pose. Reached through its far stub from
/// the battle effects, by the EFFECT_STRIKE_TARGET macro of
/// battle_effect_internal.h and directly.
void BattleMotion_ApplyKnockback(s32 id, s32 variant) {
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

/// The actor walks three quarters of the way to its target, at a speed that
/// covers the distance in about travelDivisor frames.
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY) {
  struct BattleObj *actorSlot = BattleObject_Get(actorId);
  struct BattleObj *targetSlot = BattleObject_Get(targetId);
  struct MotionObject *object = actorSlot->object;
  struct MotionObject *target = targetSlot->object;
  s32 scale = 75;
  s32 dx = target->x - object->x;
  s32 startX = object->x;
  s32 stepX = scale * dx / 100;
  s32 dz = target->z - object->z;
  s32 startZ = object->z;
  s32 stepZ = scale * dz / 100;
  s32 x = startX + stepX;
  s32 z = startZ + stepZ;
  s32 cellX = stepX >> 8;
  s32 cellZ = stepZ >> 8;
  s32 sum = cellX * cellX + cellZ * cellZ;
  s32 dist;

  if (sum != 0)
    dist = Iwram_Sqrt(sum) << 8;
  else
    dist = 0;
  dist = dist / travelDivisor;
  object->acceleration = dist;
  object->speedLimit = dist;
  object->snapToTarget = 1;

  if (object->motionFlags & 4)
    object->velocityY = initialVelocityY;
  object->velocityY = initialVelocityY;
  object->verticalMotionStrength = 0xab85;
  object->autoFaceMotion = 1;
  Actor_ResetMotion(object);
  if (dist != 0)
    Actor_SetTargetPosition(object, x, 0, z);
  Actor_SetMode(object, 2);
}

void BattleMotion_RestartLeap(s32 id) {
  struct MotionObject *object;

  object = BattleObject_Get(id)->object;
  Actor_ResetMotion(object);
  Actor_SetMode(object, 2);
}

#undef Actor_ResetMotion
#undef Actor_SetTargetPosition
#undef Actor_SetMode

void BattleMotion_MidpointNoOp(void *position, s32 speed) {}

void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId) {
  struct BattleObj *secondSlot;
  struct MotionObject *first;
  struct MotionObject *second;
  s32 x1;
  s32 x2;
  s32 z1;
  s32 z2;
  struct {
    s32 x;
    s32 y;
    s32 z;
  } pos;

  first = BattleObject_Get(firstId)->object;
  secondSlot = BattleObject_Get(secondId);

  x1 = first->targetX;
  second = secondSlot->object;
  if (x1 == (s32)0x80000000) {
    x1 = first->x;
  }
  z1 = first->targetZ;
  if (z1 == (s32)0x80000000) {
    z1 = first->z;
  }
  x2 = second->targetX;
  if (x2 == (s32)0x80000000) {
    x2 = second->x;
  }
  z2 = second->targetZ;
  if (z2 == (s32)0x80000000) {
    z2 = second->z;
  }

  pos.x = (x1 + x2) / 2;
  pos.y = 0;
  pos.z = (z1 + z2) / 2;

  BattleMotion_MidpointNoOp(&pos, 0x1000);
}

void BattleMotion_PlayHitFlash(s32 slot) {
  struct SlotEffectEntry *entry =
      (struct SlotEffectEntry *)BattleObject_Get(slot);
  u8 *effect;

  if (entry->object != NULL) {
    if ((entry->object->kind & 0xf) == 1) {
      effect =
          (u8 *)ResourceMetadata_RegisterFar(entry->object->resource, 0x29f);
      if (effect != NULL) {
        SpriteAnimation_StartFar(effect, 1);
        effect[6] = 3;
      }
      wait(10);
    }
  }
}

/// Projects the point above unit id where its battle effects play (its class's
/// height, BattleUnit_GetEffectHeight) onto the screen, into projected; the
/// height is scaled by the sprite's projected scale. Returns 0. Called by
/// ElementGlow_Play, and through its far stub by
/// Projection_ScaledUnit and SelectionMenu_Draw.
s32 BattleMotion_ProjectScaledPosition(s32 slot, s32 *screen) {
  struct MotionObject *object = BattleObject_Get(slot)->object;
  struct SlotMotionRecord *record = GetMotionRecord(object, 0);
  s32 scale;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  BattleCamera_ApplyTransform();
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), record->height);
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeight(slot) >> 16);
  return 0;
}

/// Projects the point above unit id onto the screen, into projected: 24
/// pixels up for a small enemy, 48 for others, scaled as the sprite is drawn.
/// Returns 0. Reached through its far stub from BattleTarget_RunSelection, to
/// place the target cursor.
s32 BattleMotion_ProjectConditionalPosition(s32 slot, s32 *screen) {
  struct MotionObject *object = BattleObject_Get(slot)->object;
  struct SlotMotionRecord *record = GetMotionRecord(object, 0);
  s32 scale;

  BattleCamera_ApplyTransform();
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), record->height);
  if (BattleSprite_IsSmall(*(u16 *)((u8 *)Unit_Get(slot) + 0x14a)) != 0)
    screen[1] -= Iwram_MulQ16(scale, 24);
  else
    screen[1] -= Iwram_MulQ16(scale, 48);
  return 0;
}

u8 BattleSprite_GetEffectHeight(s32 index);

s32 BattleUnit_GetEffectHeight(s32 unitId) {
  s32 value = BattleSprite_GetEffectHeight(
                  ((struct SummonOwnerState *)Unit_Get(unitId))->summon)
              << 16;

  if (value == 0) {
    if (BattleSprite_IsSmall(
            ((struct SummonOwnerState *)Unit_Get(unitId))->summon) != 0)
      value = 0x180000;
    else
      value = 0x300000;
  }
  return value;
}
