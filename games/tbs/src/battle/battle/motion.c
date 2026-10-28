/// @file
/// The units' movements: returning, running, leaping and knocked back, and
/// their positions on the screen.
// Part of battle.c.

#include "../battle_internal.h"

/// Actor.targetX and targetZ while the actor has no move target.
#define NO_MOVE_TARGET 0x80000000

// The motion values below are 16.16 fixed point, in pixels and frames:
// acceleration and speedLimit per frame, velocityY the hop's upward speed,
// and verticalMotionStrength the gravity taken from it each frame (0xab85
// about 0.67, 0x9999 0.6, 0x7851 about 0.47).

/// Sends unitId back to its spot, landing exactly there (snapToTarget), and
/// turns it to face the centre at once. Called after an action by the
/// staging code, the auto battle, BattleAbility_ApplyToTarget and the Quake
/// effect.
void BattleMotion_ReturnToSpot(s32 unitId) {
  s32 z;
  s32 stopped;
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  stopped = 0;
  actor->acceleration = 2 << 16;
  actor->speedLimit = 8 << 16;
  actor->gravity = 0xab85;
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
  actor->facing = ArcTan2(z >> 3, battleObject->anchorX) - 0x8000;
}

/// Sends unitId running off its side of the battlefield, three times
/// as far out as its spot, with a hop, in POSE_STANDING. Called by
/// BattleFlee_PlayRun and BattleStaging_RunFlee.
void BattleMotion_SetupEscapeObject(s32 unitId) {
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  actor->acceleration = 2 << 16;
  actor->speedLimit = 8 << 16;
  actor->velocityY = 5 << 16;
  actor->gravity = 0x7851;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, battleObject->anchorX * 3, 0,
                         battleObject->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_STANDING);
}

/// Sends the unit of startSlot leaping progress percent of the way toward
/// the unit of endSlot, in POSE_LEAP. Also clears REG_BLDCNT. Nothing in the
/// source calls it.
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
  start->acceleration = 2 << 16;
  start->speedLimit = 8 << 16;
  start->velocityY = 4 << 16;
  start->gravity = 0xab85;
  start->bounce = 0;
  start->steeringFlags = 1;
  Actor_SetMoveTargetFar(start, x, 0, startZ + zStep);
  Actor_SelectAnimationFar(start, POSE_LEAP);
}

/// Sends a unit's actor back to its spot. Nothing in the source calls it.
void BattleMotion_ReturnObjectToSpot(struct BattleObject *battleObject) {
  struct Actor *actor = battleObject->actor;

  actor->acceleration = 2 << 16;
  actor->speedLimit = 8 << 16;
  actor->gravity = 0xab85;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_SetMoveTargetFar(actor, battleObject->anchorX, 0,
                         battleObject->anchorZ);
}

/// Sends unitId hopping out to one and a half times its spot's
/// distance from the centre. Called by BattleStaging_RunAttack and
/// BattleStaging_RunPlainAttack.
void BattleMotion_HopBack(s32 unitId) {
  u32 scaledX;
  struct BattleObject *battleObject;
  struct Actor *actor;

  battleObject = BattleObject_Get(unitId);
  actor = battleObject->actor;
  actor->acceleration = Q16_ONE;
  actor->speedLimit = 4 << 16;
  actor->velocityY = 3 << 16;
  actor->gravity = 0x9999;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  // anchorX * 3 / 2, rounded toward zero.
  scaledX = battleObject->anchorX * 3;
  Actor_SetMoveTargetFar(actor, (s32)(scaledX + (scaledX >> 31)) >> 1, 0,
                         battleObject->anchorZ);
}

/// Sends unit id leaping back to 1.3 times its spot's distance from the
/// centre, in POSE_DAMAGE. Only an unnamed overlay veneer in
/// BATTLE.S reaches it, and nothing in the source calls that.
void BattleMotion_LeapBack(s32 unitId) {
  struct BattleObject *battleObject = BattleObject_Get(unitId);
  struct Actor *actor = battleObject->actor;

  actor->acceleration = 128 << 9;
  actor->speedLimit = 128 << 11;
  actor->velocityY = 128 << 11;
  actor->gravity = 0x9999;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, Iwram_MulQ16(battleObject->anchorX, 0x14ccc), 0,
                         battleObject->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_DAMAGE);
}

/// Knocks unit id back by the speed, hop and distance of the knockback
/// variant (enum BattleKnockback), and puts it in POSE_DAMAGE.
/// The Fusion Dragon only takes the pose. Reached through its far stub from
/// the battle effects, by the EFFECT_STRIKE_TARGET macro of
/// battle_effect_internal.h and directly.
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
    actor->gravity = 0x9999;
    actor->bounce = 0;
    actor->steeringFlags = 0;
    Actor_StartIdleFar(actor);
    anchorX = battleObject->anchorX;
    targetX = (anchorX * gBattleKnockback_DistancePercent[variant]) / 100;
    Actor_SetMoveTargetFar(actor, targetX, 0, battleObject->anchorZ);
  }
  Actor_SelectAnimationFar(actor, POSE_DAMAGE);
}

/// Sends unit unitId three quarters of the way to unit targetId, arriving
/// in travelDivisor frames at a steady speed, leaping up at initialVelocityY
/// (16.16 pixels a frame), in POSE_LEAP.
/// Called by BattleStaging_RunAttack, BattleStaging_RunPlainAttack
/// and AutoBattle_RunApproachAttack, and through its far stub by the
/// Bind, flurry, Annihilation, target motes and Thrash effects.
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

  // Removing the repeated store shortens the matching EN function by 16 bytes.
  // The test is for a floating unit (ACTOR_FLOATS); both paths store
  // the same hop.
  if (actor->motionFlags & ACTOR_FLOATS)
    actor->velocityY = initialVelocityY;
  actor->velocityY = initialVelocityY;
  actor->gravity = 0xab85;
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

/// Works out the point halfway between two units, at their move targets
/// where they have one, and hands it to BattleMotion_MidpointNoOp,
/// which does nothing. Nothing in the source calls it.
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

  BattleMotion_MidpointNoOp(&position, 0x1000);
}

/// The hit flash's sprite layer, sprite 283: a star burst that grows and
/// breaks apart over ten frames (graphics/sprite/spr_283.png).
#define HIT_FLASH_ANIMATION 283

/// Plays the hit flash on unit id's sprite and waits ten frames. Only a
/// single-sprite actor takes it. Nothing in the source calls it.
void BattleMotion_PlayHitFlash(s32 unitId) {
  struct Actor *actor;
  struct SpriteAnimation *effect;
  s32 effectHandle;

  actor = BattleObject_Get(unitId)->actor;
  if ((actor != NULL) && ((ACTOR_SPRITE_KIND_MASK & actor->spriteStorageKind) ==
                          ACTOR_SPRITE_ONE)) {
    effectHandle = Sprite_AddAnimationFar(actor->sprite, HIT_FLASH_ANIMATION);
    if (effectHandle != 0) {
      effect = (struct SpriteAnimation *)effectHandle;
      SpriteAnimation_StartFar(effect, 1);
      effect->priority = 3;
    }
    wait(10);
  }
}

/// Projects the point above unit id where its battle effects play (its class's
/// height, BattleUnit_GetEffectHeight) onto the screen, into projected; the
/// height is scaled by the sprite's projected scale. Returns 0. Called by
/// ElementGlow_Play, and through its far stub by
/// Projection_ScaledUnit and SelectionMenu_Draw.
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected) {
  struct Actor *actor = BattleObject_Get(unitId)->actor;
  struct Sprite *record = GetMotionRecord(actor, 0);
  s32 projectionScale;
  s32 spriteScale;
  /* FAKEMATCH(dead): an unused position for the stack frame's 12 bytes; without
   * it, 2 of 42 instructions differ in tbs-en. */
  UNUSED struct ScreenPoint input;

  BattleCamera_ApplyTransform();
  projectionScale = Transform_ProjectPoint(&actor->x.fixed, projected);
  spriteScale = Iwram_MulQ16(projectionScale, record->scale);
  projectionScale =
      Iwram_MulQ16(spriteScale, (s32)BattleUnit_GetEffectHeight(unitId) >> 16);
  projected[1] -= projectionScale;
  return 0;
}

/// Projects the point above unit id onto the screen, into projected: 24
/// pixels up for a small enemy, 48 for others, scaled as the sprite is drawn.
/// Returns 0. Reached through its far stub from BattleTarget_RunSelection, to
/// place the target cursor.
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

/// Returns the height above unit id where its battle effects play, 16.16: its
/// class's own (BattleSprite_GetEffectHeight), else 24 for a small enemy and 48
/// for others. Called by BattleMotion_ProjectScaledPosition, and through its
/// far stub by the battle effects that play at a unit's body.
s32 BattleUnit_GetEffectHeight(s32 unitId) {
  u32 height;
  u8 classId;

  classId = Unit_GetFar(unitId)->id;
  height = (u8)BattleSprite_GetEffectHeight(classId) << 16;
  if (height == 0) {
    classId = Unit_GetFar(unitId)->id;
    if (BattleSprite_IsSmall(classId) != 0) {
      height = 24 << 16;
    } else {
      height = 48 << 16;
    }
  }
  return height;
}
