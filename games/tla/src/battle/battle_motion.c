/// @file
/// The units' movements: returning to their spots, running and leaping, and
/// their positions on the screen.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "battle/battle.h"
#include "gamepak/far_call/field.h"
#include "macros.h"

#define Actor_StartIdleFar ((void (*)(struct MotionObject *))Actor_StartIdleFar)
#define Actor_SetMoveTargetFar                                                 \
  ((void (*)(struct MotionObject *, s32, s32, s32))Actor_SetMoveTargetFar)
#define Actor_SelectAnimationFar                                               \
  ((void (*)(struct MotionObject *, s32))Actor_SelectAnimationFar)

/// Sends unitId back to its spot, landing exactly there (snapToTarget), and
/// turns it to face the centre at once.
void BattleMotion_ReturnToSpot(s32 slotId) {
  s32 z;
  s32 zero;
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(slotId);
  object = slot->object;
  zero = 0;
  object->acceleration = px(2);
  object->speedLimit = px(8);
  object->verticalMotionStrength = 0xab85;
  object->velocityY = zero;
  object->verticalMotionPhase = zero;
  object->autoFaceMotion = zero;
  object->snapToTarget = 1;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
  z = slot->anchorZ;
  if (z < 0)
    z += 7;
  object->angle = (s16)(ArcTan2(z >> 3, slot->anchorX) - TRIG_TURN / 2);
}

/// Sends unitId running off its side of the battlefield, three times as far
/// out as its spot, with a hop, in POSE_STANDING.
void BattleMotion_SetupEscapeObject(s32 objectId) {
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(objectId);
  object = slot->object;
  object->acceleration = px(2);
  object->speedLimit = px(8);
  object->velocityY = px(5);
  object->verticalMotionStrength = 0x7851;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, slot->anchorX * 3, 0, slot->anchorZ);
  Actor_SelectAnimationFar(object, POSE_STANDING);
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

  x = start->x.fixed + (end->x.fixed - start->x.fixed) * progress / 100;
  z = start->z.fixed + (end->z.fixed - start->z.fixed) * progress / 100;
  REG_BLDCNT = 0;
  start->acceleration = px(2);
  start->speedLimit = px(8);
  start->velocityY = px(4);
  start->gravity = 0xab85;
  start->bounce = 0;
  start->steeringFlags = 1;
  Actor_SetMoveTargetFar(start, x, 0, z);
  Actor_SelectAnimationFar(start, POSE_LEAP);
}

/// Sends a unit's actor back to its spot. Nothing in the source calls it.
void BattleMotion_ReturnObjectToSpot(struct BattleObject *slot) {
  struct MotionObject *object = slot->object;

  object->acceleration = px(2);
  object->speedLimit = px(8);
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
}

/// Sends unitId hopping out to one and a half times its spot's distance from
/// the centre.
void BattleMotion_HopBack(s32 objectId) {
  u32 scaledX;
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(objectId);
  object = slot->object;
  object->acceleration = Q16_ONE;
  object->speedLimit = px(4);
  object->velocityY = px(3);
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_StartIdleFar(object);
  scaledX = slot->anchorX * 3;
  Actor_SetMoveTargetFar(object, (s32)(scaledX + (scaledX >> 31)) >> 1, 0,
                         slot->anchorZ);
}

/// Sends unit id leaping back to 1.3 times its spot's distance from the
/// centre, in POSE_DAMAGE.
void BattleMotion_LeapBack(s32 objectId) {
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(objectId);
  object = slot->object;
  object->acceleration = Q16_ONE;
  object->speedLimit = px(4);
  object->velocityY = px(4);
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_StartIdleFar(object);
  {
    s32 (*mul)(s32 a, s32 b);
    s32 anchor = slot->anchorX;

    mul = Iwram_MulQ16;
    Actor_SetMoveTargetFar(object, mul(anchor, 0x14ccc), 0, slot->anchorZ);
  }
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

/// Sends a battle object back by its variant's distance and speed, with a hop
/// when it stands on the ground or for the strong variants (5 and up). Four
/// enemies stay where they stand, and every object gets its vertical motion
/// before it takes mode 5.
/// Knocks unit id back by the speed, hop and distance of the knockback
/// variant (enum BattleKnockback), and puts it in POSE_DAMAGE.
/// The Fusion Dragon only takes the pose. Reached through its far stub from
/// the battle effects, by the EFFECT_STRIKE_TARGET macro of
/// battle_effect_internal.h and directly.
void BattleMotion_ApplyKnockback(s32 id, s32 variant) {
  struct BattleObject *slot;
  struct MotionObject *object;
  s32 x;
  s32 scale;
  s32 enemyId;

  slot = BattleObject_Get(id);
  object = slot->object;
  enemyId = Unit_Get(id)->enemyId;
  if (enemyId != ENEMY_FUSION_DRAGON && enemyId != ENEMY_DOOM_DRAGON_A &&
      enemyId != ENEMY_DOOM_DRAGON_E && enemyId != ENEMY_DOOM_DRAGON_H) {
    object->acceleration = gBattleKnockback_Acceleration[variant];
    object->speedLimit = gBattleKnockback_SpeedLimit[variant];
    if (object->y == 0 || variant > 4)
      object->velocityY = gBattleKnockback_VelocityY[variant];
    Actor_StartIdleFar(object);
    scale = slot->anchorX;
    x = (scale * gBattleKnockback_DistancePercent[variant]) / 100;
    Actor_SetMoveTargetFar(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

/// The actor walks three quarters of the way to its target, at a speed that
/// covers the distance in about travelDivisor frames.
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY) {
  struct BattleObject *actorSlot = BattleObject_Get(actorId);
  struct BattleObject *targetSlot = BattleObject_Get(targetId);
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

  if (object->motionFlags & ACTOR_FLOATS)
    object->velocityY = initialVelocityY;
  object->velocityY = initialVelocityY;
  object->verticalMotionStrength = 0xab85;
  object->autoFaceMotion = 1;
  Actor_StartIdleFar(object);
  if (dist != 0)
    Actor_SetMoveTargetFar(object, x, 0, z);
  Actor_SelectAnimationFar(object, POSE_LEAP);
}

/// Restarts unit id's motion in its leap (animation 2). Nothing in the source
/// calls it.
void BattleMotion_RestartLeap(s32 id) {
  struct MotionObject *object;

  object = BattleObject_Get(id)->object;
  Actor_StartIdleFar(object);
  Actor_SelectAnimationFar(object, POSE_LEAP);
}

#undef Actor_StartIdleFar
#undef Actor_SetMoveTargetFar
#undef Actor_SelectAnimationFar

/// Does nothing; BattleMotion_ApplyPairMidpoint, its only caller, passes it a
/// position and a distance.
void BattleMotion_MidpointNoOp(void *position, s32 speed) {}

/// Unused: works out the point halfway between two units and hands it to a
/// routine that does nothing.
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId) {
  struct BattleObject *secondSlot;
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

  SET_POSITION(&pos, (x1 + x2) / 2, 0, (z1 + z2) / 2);

  BattleMotion_MidpointNoOp(&pos, 0x1000);
}

/// Plays the hit flash on unit id's sprite and waits ten frames. Only a
/// single-sprite actor takes it. Nothing in the source calls it.
void BattleMotion_PlayHitFlash(s32 slot) {
  struct BattleObject *battleObject = BattleObject_Get(slot);
  struct SpriteAnimation *effect;

  if (battleObject->object != NULL) {
    if ((battleObject->object->recordStorageKind & 0xf) == ACTOR_SPRITE_ONE) {
      effect = (struct SpriteAnimation *)Sprite_AddAnimationFar(
          battleObject->object->records, SPRITE_STAR_BURST);
      if (effect != NULL) {
        SpriteAnimation_StartFar(effect, 1);
        effect->priority = 3;
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
  struct Sprite *record = GetMotionRecord(object, 0);
  s32 scale;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  BattleCamera_ApplyTransform();
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), record->scale);
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeight(slot) >> 16);
  return 0;
}

/// Projects the point above unit id onto the screen, into projected: 24
/// pixels up for a small enemy, 48 for others, scaled as the sprite is drawn.
/// Returns 0. Reached through its far stub from BattleTarget_RunSelection, to
/// place the target cursor.
s32 BattleMotion_ProjectConditionalPosition(s32 slot, s32 *screen) {
  struct MotionObject *object = BattleObject_Get(slot)->object;
  struct Sprite *record = GetMotionRecord(object, 0);
  s32 scale;

  BattleCamera_ApplyTransform();
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), record->scale);
  if (BattleSprite_IsSmall(*(u16 *)((u8 *)Unit_Get(slot) + 0x14a)) != 0)
    screen[1] -= Iwram_MulQ16(scale, 24);
  else
    screen[1] -= Iwram_MulQ16(scale, 48);
  return 0;
}

/// Returns the height above a unit where its battle effects play: its class's
/// own, else 24 pixels for a small enemy and 48 for others.
s32 BattleUnit_GetEffectHeight(s32 unitId) {
  s32 value = px((u8)BattleSprite_GetEffectHeight(Unit_Get(unitId)->enemyId));

  if (value == 0) {
    if (BattleSprite_IsSmall(Unit_Get(unitId)->enemyId) != 0)
      value = px(24);
    else
      value = px(48);
  }
  return value;
}
