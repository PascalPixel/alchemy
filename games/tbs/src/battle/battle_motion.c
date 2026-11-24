/// @file
/// The units' movements: returning, running, leaping and knocked back, and
/// their positions on the screen.
// Part of battle.c.

#include "battle_internal.h"

// The motion values below are per frame; gravity is taken from a hop's speed
// each frame.

/// Sends unitId back to its spot, landing exactly there (snapToTarget), and
/// turns it to face the centre at once.
void BattleMotion_ReturnToSpot(s32 unitId) {
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(8);
  actor->velocityY = 0;
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  actor->snapToTarget = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, battleObject->anchorX, 0,
                         battleObject->anchorZ);
  // As BattleObject_Make turns a new actor.
  actor->facing = Trig_Atan(battleObject->anchorZ / 8, battleObject->anchorX) -
                  DEGtoANG(180);
}

/// Sends unitId running off its side of the battlefield, three times as far
/// out as its spot, with a hop, in POSE_STANDING.
void BattleMotion_SetupEscapeObject(s32 unitId) {
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(8);
  actor->velocityY = toFIXED(5);
  actor->gravity = toFIXED(47) / 100;
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
  FIXED zStep;
  FIXED startX;
  FIXED x;
  FIXED startZ;
  FIXED endZ;
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
  start->acceleration = toFIXED(2);
  start->speedLimit = toFIXED(8);
  start->velocityY = toFIXED(4);
  start->gravity = BATTLE_UNIT_GRAVITY;
  start->bounce = 0;
  start->steeringFlags = 1;
  Actor_SetMoveTargetFar(start, x, 0, startZ + zStep);
  Actor_SelectAnimationFar(start, POSE_LEAP);
}

/// Unused: sends a unit's actor back to its spot.
void BattleMotion_ReturnObjectToSpot(struct BattleObject *battleObject) {
  struct Actor *actor = battleObject->actor;

  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(8);
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
  actor->acceleration = FIXED_ONE;
  actor->speedLimit = toFIXED(4);
  actor->velocityY = toFIXED(3);
  actor->gravity = toFIXED(3) / 5;
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

  actor->acceleration = toFIXED(1);
  actor->speedLimit = toFIXED(4);
  actor->velocityY = toFIXED(4);
  actor->gravity = toFIXED(3) / 5;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(
      actor, Iwram_MulFX(battleObject->anchorX, FIXED_ONE * 13 / 10), 0,
      battleObject->anchorZ);
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
    actor->gravity = toFIXED(3) / 5;
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
/// `frames` frames, with a hop of initialVelocityY.
void BattleMotion_ApproachTarget(s32 unitId, s32 targetId, s32 frames,
                                 s32 initialVelocityY) {
  struct BattleObject *actorSlot = BattleObject_Get(unitId);
  struct BattleObject *targetSlot = BattleObject_Get(targetId);
  struct Actor *actor = actorSlot->actor;
  struct Actor *target = targetSlot->actor;
  s32 percent = 75;
  FIXED dx = target->x.fixed - actor->x.fixed;
  FIXED startX = actor->x.fixed;
  FIXED stepX = percent * dx / 100;
  FIXED dz = target->z.fixed - actor->z.fixed;
  FIXED startZ = actor->z.fixed;
  FIXED stepZ = percent * dz / 100;
  FIXED x = startX + stepX;
  FIXED z = startZ + stepZ;
  // The distance is taken in 1/256 pixels, so its square fits 32 bits.
  s32 coarseX = stepX >> 8;
  s32 coarseZ = stepZ >> 8;
  s32 speed;

  speed = Iwram_Sqrt(coarseX * coarseX + coarseZ * coarseZ);
  speed = (speed << 8) / frames;
  actor->acceleration = speed;
  actor->speedLimit = speed;
  actor->snapToTarget = 1;

  // DEAD: the store under ACTOR_FLOATS is overwritten at once; the ROM keeps
  // the test and its store.
  if (actor->motionFlags & ACTOR_FLOATS)
    actor->velocityY = initialVelocityY;
  actor->velocityY = initialVelocityY;
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->steeringFlags = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, x, 0, z);
  Actor_SelectAnimationFar(actor, POSE_LEAP);
}

/// Unused: restarts unit id's motion in POSE_LEAP.
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
  FIXED firstX;
  FIXED secondX;
  FIXED firstZ;
  FIXED secondZ;
  struct Point position;

  first = BattleObject_Get(firstId)->actor;
  secondSlot = BattleObject_Get(secondId);
  second = secondSlot->actor;

  firstX = first->targetX;
  if (firstX == ACTOR_NO_TARGET) {
    firstX = first->x.fixed;
  }
  firstZ = first->targetZ;
  if (firstZ == ACTOR_NO_TARGET) {
    firstZ = first->z.fixed;
  }
  secondX = second->targetX;
  if (secondX == ACTOR_NO_TARGET) {
    secondX = second->x.fixed;
  }
  secondZ = second->targetZ;
  if (secondZ == ACTOR_NO_TARGET) {
    secondZ = second->z.fixed;
  }

  position.x = (firstX + secondX) / 2;
  position.y = 0;
  position.z = (firstZ + secondZ) / 2;

  BattleMotion_MidpointNoOp(&position, toFIXED(1) / 16);
}

/// Unused: plays the hit flash on unit id's sprite and waits ten frames. Only a
/// single-sprite actor takes it.
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
  struct Sprite *sprite = Actor_GetSprite(actor, 0);
  s32 projectionScale;
  FIXED spriteScale;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED struct ScreenPoint input;

  BattleCamera_ApplyTransform();
  projectionScale = Matrix_ProjectPoint(&actor->x.fixed, projected);
  spriteScale = Iwram_MulFX(projectionScale, sprite->scale);
  projectionScale = Iwram_MulFX(
      spriteScale, FIXEDtoINT((s32)BattleUnit_GetEffectHeight(unitId)));
  projected[1] -= projectionScale;
  return 0;
}

/// The pixels above a unit where its battle effects play when its battle sprite
/// row gives none: a small enemy's, and everyone else's.
#define EFFECT_HEIGHT_SMALL 24
#define EFFECT_HEIGHT_LARGE 48

/// Projects the point above a unit onto the screen: the target cursor's place,
/// EFFECT_HEIGHT_SMALL up for a small enemy and EFFECT_HEIGHT_LARGE for others,
/// scaled as the sprite is. Returns 0.
s32 BattleMotion_ProjectCursorPoint(s32 unitId, s32 *projected) {
  struct Actor *actor = BattleObject_Get(unitId)->actor;
  struct Sprite *sprite = Actor_GetSprite(actor, 0);
  s32 projectedHeight;
  FIXED spriteScale;

  BattleCamera_ApplyTransform();
  projectedHeight = Matrix_ProjectPoint(&actor->x.fixed, projected);
  spriteScale = Iwram_MulFX(projectedHeight, sprite->scale);
  if (BattleSprite_IsSmall(Unit_GetFar(unitId)->id) != 0)
    projectedHeight = Iwram_MulFX(spriteScale, EFFECT_HEIGHT_SMALL);
  else
    projectedHeight = Iwram_MulFX(spriteScale, EFFECT_HEIGHT_LARGE);
  projected[1] -= projectedHeight;
  return 0;
}

/// Returns the height above a unit where its battle effects play, in 16.16
/// pixels: its battle sprite row's own, else EFFECT_HEIGHT_SMALL for a small
/// enemy and
/// EFFECT_HEIGHT_LARGE for others.
s32 BattleUnit_GetEffectHeight(s32 unitId) {
  u32 height;
  u8 spriteRow;

  spriteRow = Unit_GetFar(unitId)->id;
  height = toFIXED((u8)BattleSprite_GetEffectHeight(spriteRow));
  if (height == 0) {
    spriteRow = Unit_GetFar(unitId)->id;
    if (BattleSprite_IsSmall(spriteRow) != 0) {
      height = toFIXED(EFFECT_HEIGHT_SMALL);
    } else {
      height = toFIXED(EFFECT_HEIGHT_LARGE);
    }
  }
  return height;
}

#undef EFFECT_HEIGHT_SMALL
#undef EFFECT_HEIGHT_LARGE
