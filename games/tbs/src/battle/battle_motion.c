/// @file
/// The units' movements: returning, running, leaping and knocked back, and
/// their positions on the screen.
// Part of battle.c.

#include "battle_internal.h"

/// Actor.targetX and targetZ while the actor has no move target.
#define NO_MOVE_TARGET 0x80000000

// The motion values below are per frame; verticalMotionStrength is the gravity
// taken from a hop's speed.

/// Sends unitId back to its spot, landing exactly there (snapToTarget), and
/// turns it to face the centre at once.
void BattleMotion_ReturnToSpot(s32 unitId) {
  s32 z;
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  {
    s32 stopped = 0;

    actor->acceleration = px(2);
    actor->speedLimit = px(8);
    actor->gravity = BATTLE_UNIT_GRAVITY;
    actor->velocityY = stopped;
    actor->bounce = stopped;
    actor->steeringFlags = stopped;
    actor->snapToTarget = 1;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, battleObject->anchorX, 0,
                           battleObject->anchorZ);
    // z / 8, rounded toward zero, as BattleObject_Make turns a new actor.
    z = battleObject->anchorZ;
    if (z < 0)
      z += 7;
    actor->facing = ArcTan2(z >> 3, battleObject->anchorX) - TRIG_TURN / 2;
  }
}

/// Sends unitId running off its side of the battlefield, three times as far
/// out as its spot, with a hop, in POSE_STANDING.
void BattleMotion_SetupEscapeObject(s32 unitId) {
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  actor->acceleration = px(2);
  actor->speedLimit = px(8);
  actor->velocityY = px(5);
  actor->gravity = px(47) / 100;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, battleObject->anchorX * 3, 0,
                         battleObject->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_STANDING);
}

/// Unused: sends one unit leaping progress percent of the way toward another.
void BattleMotion_LeapToward(struct BattleObject *startSlot,
                             struct BattleObject *endSlot, s32 progress) {
  s32 zStep;
  s32 startX;
  s32 x;
  s32 startZ;
  s32 endZ;
  struct Actor *start;
  struct Actor *end;

  start = startSlot->actor;
  end = endSlot->actor;
  startX = start->x.fixed;
  x = startX + progress * (end->x.fixed - startX) / 100;
  endZ = end->z.fixed;
  startZ = start->z.fixed;
  zStep = progress * (endZ - startZ) / 100;
  REG_BLDCNT = 0;
  start->acceleration = px(2);
  start->speedLimit = px(8);
  start->velocityY = px(4);
  start->gravity = BATTLE_UNIT_GRAVITY;
  start->bounce = 0;
  start->steeringFlags = 1;
  Actor_SetMoveTargetFar(start, x, 0, startZ + zStep);
  Actor_SelectAnimationFar(start, POSE_LEAP);
}

/// Sends a unit's actor back to its spot. Nothing in the source calls it.
void BattleMotion_ReturnObjectToSpot(struct BattleObject *battleObject) {
  struct Actor *actor = battleObject->actor;

  actor->acceleration = px(2);
  actor->speedLimit = px(8);
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_SetMoveTargetFar(actor, battleObject->anchorX, 0,
                         battleObject->anchorZ);
}

/// Sends unitId hopping out to one and a half times its spot's distance from
/// the centre.
void BattleMotion_HopBack(s32 unitId) {
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  actor->acceleration = Q16_ONE;
  actor->speedLimit = px(4);
  actor->velocityY = px(3);
  actor->gravity = px(3) / 5;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, battleObject->anchorX * 3 / 2, 0,
                         battleObject->anchorZ);
}

/// Unused: sends a unit leaping back from the centre in its damage pose.
void BattleMotion_LeapBack(s32 unitId) {
  struct BattleObject *battleObject = BattleObject_Get(unitId);
  struct Actor *actor = battleObject->actor;

  actor->acceleration = 128 << 9;
  actor->speedLimit = 128 << 11;
  actor->velocityY = 128 << 11;
  actor->gravity = px(3) / 5;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor,
                         Iwram_MulQ16(battleObject->anchorX, Q16_ONE * 13 / 10),
                         0, battleObject->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_DAMAGE);
}

/// Knocks a unit back as the knockback variant says, in its damage pose; the
/// Fusion Dragon only takes the pose.
void BattleMotion_ApplyKnockback(s32 unitId, s32 variant) {
  struct BattleObject *battleObject;
  struct Actor *actor;
  s32 targetX;
  s32 anchorX;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  if (Unit_GetFar(unitId)->id != ENEMY_FUSION_DRAGON) {
    actor->acceleration = gBattleKnockback_Acceleration[variant];
    actor->speedLimit = gBattleKnockback_SpeedLimit[variant];
    // A unit already in the air keeps its hop, except from KNOCKBACK_SKID on.
    if (actor->y.fixed == 0 || variant > KNOCKBACK_HURL)
      actor->velocityY = gBattleKnockback_VelocityY[variant];
    actor->gravity = px(3) / 5;
    actor->bounce = 0;
    actor->steeringFlags = 0;
    Actor_StartIdleFar(actor);
    anchorX = battleObject->anchorX;
    targetX = (anchorX * gBattleKnockback_DistancePercent[variant]) / 100;
    Actor_SetMoveTargetFar(actor, targetX, 0, battleObject->anchorZ);
  }
  Actor_SelectAnimationFar(actor, POSE_DAMAGE);
}

/// Sends one unit leaping three quarters of the way to another, arriving in
/// travelDivisor frames.
void BattleMotion_ApproachTarget(s32 unitId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY) {
  struct BattleObject *actorSlot = BattleObject_Get(unitId);
  struct BattleObject *targetSlot = BattleObject_Get(targetId);
  struct Actor *actor = actorSlot->actor;
  struct Actor *target = targetSlot->actor;
  s32 percent = 75;
  s32 dx = target->x.fixed - actor->x.fixed;
  s32 startX = actor->x.fixed;
  s32 stepX = percent * dx / 100;
  s32 dz = target->z.fixed - actor->z.fixed;
  s32 startZ = actor->z.fixed;
  s32 stepZ = percent * dz / 100;
  s32 x = startX + stepX;
  s32 z = startZ + stepZ;
  // The distance is taken in 1/256 pixels, so its square fits 32 bits.
  s32 cellX = stepX >> 8;
  s32 cellZ = stepZ >> 8;
  s32 speed;

  speed = Iwram_Sqrt(cellX * cellX + cellZ * cellZ);
  speed = (speed << 8) / travelDivisor;
  actor->acceleration = speed;
  actor->speedLimit = speed;
  actor->snapToTarget = 1;

  // The test is for a floating unit (ACTOR_FLOATS); both paths store the same
  // hop.
  if (actor->motionFlags & ACTOR_FLOATS)
    actor->velocityY = initialVelocityY;
  actor->velocityY = initialVelocityY;
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->steeringFlags = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, x, 0, z);
  Actor_SelectAnimationFar(actor, POSE_LEAP);
}

/// Restarts unit id's motion in POSE_LEAP. Nothing in the source calls it.
void BattleMotion_RestartLeap(s32 unitId) {
  struct Actor *actor;

  actor = BattleObject_Get(unitId)->actor;
  Actor_StartIdleFar(actor);
  Actor_SelectAnimationFar(actor, POSE_LEAP);
}

/// Does nothing; BattleMotion_ApplyPairMidpoint, its only caller, passes it
/// a position and a distance.
void BattleMotion_MidpointNoOp() {}

/// Unused: works out the point halfway between two units and hands it to a
/// routine that does nothing.
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId) {
  struct BattleObject *secondSlot;
  struct Actor *first;
  struct Actor *second;
  s32 firstX;
  s32 secondX;
  s32 firstZ;
  s32 secondZ;
  struct FixedPointPosition position;

  first = BattleObject_Get(firstId)->actor;
  secondSlot = BattleObject_Get(secondId);
  second = secondSlot->actor;

  firstX = first->targetX;
  if (firstX == NO_MOVE_TARGET) {
    firstX = first->x.fixed;
  }
  firstZ = first->targetZ;
  if (firstZ == NO_MOVE_TARGET) {
    firstZ = first->z.fixed;
  }
  secondX = second->targetX;
  if (secondX == NO_MOVE_TARGET) {
    secondX = second->x.fixed;
  }
  secondZ = second->targetZ;
  if (secondZ == NO_MOVE_TARGET) {
    secondZ = second->z.fixed;
  }

  position.x = (firstX + secondX) / 2;
  position.y = 0;
  position.z = (firstZ + secondZ) / 2;

  BattleMotion_MidpointNoOp(&position, 4096);
}

/// Plays the hit flash on unit id's sprite and waits ten frames. Only a
/// single-sprite actor takes it. Nothing in the source calls it.
void BattleMotion_PlayHitFlash(s32 unitId) {
  struct Actor *actor;
  struct SpriteAnimation *effect;
  s32 effectHandle;

  actor = BattleObject_Get(unitId)->actor;
  if ((actor != NULL) && ((ACTOR_SPRITE_KIND_MASK & actor->spriteStorageKind) ==
                          ACTOR_SPRITE_ONE)) {
    effectHandle = Sprite_AddAnimationFar(actor->sprite, SPRITE_STAR_BURST);
    if (effectHandle != 0) {
      effect = (struct SpriteAnimation *)effectHandle;
      SpriteAnimation_StartFar(effect, 1);
      effect->priority = 3;
    }
    wait(10);
  }
}

/// Projects the point above a unit where its battle effects play onto the
/// screen, scaled as its sprite is. Returns 0.
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected) {
  struct Actor *actor = BattleObject_Get(unitId)->actor;
  struct Sprite *record = GetMotionRecord(actor, 0);
  s32 projectionScale;
  s32 spriteScale;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: no instruction addresses any of the 12 bytes it reserves (sub sp,
   * #12); as without it, 4 of 42 instructions differ in tbs-en. */
  UNUSED struct ScreenPoint input;

  BattleCamera_ApplyTransform();
  projectionScale = Transform_ProjectPoint(&actor->x.fixed, projected);
  spriteScale = Iwram_MulQ16(projectionScale, record->scale);
  projectionScale =
      Iwram_MulQ16(spriteScale, (s32)BattleUnit_GetEffectHeight(unitId) >> 16);
  projected[1] -= projectionScale;
  return 0;
}

/// Projects the point above a unit onto the screen, 24 pixels up for a small
/// enemy and 48 for others. Returns 0.
s32 BattleMotion_ProjectConditionalPosition(s32 unitId, s32 *projected) {
  struct Actor *actor = BattleObject_Get(unitId)->actor;
  struct Sprite *record = GetMotionRecord(actor, 0);
  s32 projectedHeight;
  s32 spriteScale;

  BattleCamera_ApplyTransform();
  projectedHeight = Transform_ProjectPoint(&actor->x.fixed, projected);
  spriteScale = Iwram_MulQ16(projectedHeight, record->scale);
  if (BattleSprite_IsSmall(Unit_GetFar(unitId)->id) != 0)
    projectedHeight = Iwram_MulQ16(spriteScale, 24);
  else
    projectedHeight = Iwram_MulQ16(spriteScale, 48);
  projected[1] -= projectedHeight;
  return 0;
}

/// Returns the height above a unit where its battle effects play: its class's
/// own, else 24 pixels for a small enemy and 48 for others.
s32 BattleUnit_GetEffectHeight(s32 unitId) {
  u32 height;
  u8 classId;

  classId = Unit_GetFar(unitId)->id;
  height = px((u8)BattleSprite_GetEffectHeight(classId));
  if (height == 0) {
    classId = Unit_GetFar(unitId)->id;
    if (BattleSprite_IsSmall(classId) != 0) {
      height = px(24);
    } else {
      height = px(48);
    }
  }
  return height;
}
