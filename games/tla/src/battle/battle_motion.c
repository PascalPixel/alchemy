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
void BattleMotion_ReturnToSpot(s32 unitId) {
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(unitId);
  object = slot->object;
  object->acceleration = toFIXED(2);
  object->speedLimit = toFIXED(8);
  object->velocityY = 0;
  object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  object->snapToTarget = 1;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
  // As BattleObject_Make turns a new actor.
  object->angle =
      (s16)(Trig_Atan(slot->anchorZ / 8, slot->anchorX) - DEGtoANG(180));
}

/// Sends unitId running off its side of the battlefield, three times as far
/// out as its spot, with a hop, in POSE_STANDING.
void BattleMotion_SetupEscapeObject(s32 unitId) {
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(unitId);
  object = slot->object;
  object->acceleration = toFIXED(2);
  object->speedLimit = toFIXED(8);
  object->velocityY = toFIXED(5);
  object->verticalMotionStrength = toFIXED(47) / 100;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, slot->anchorX * 3, 0, slot->anchorZ);
  Actor_SelectAnimationFar(object, POSE_STANDING);
}

/// Unused: sends one unit leaping progress percent of the way toward another,
/// and turns off the colour effects.
void BattleMotion_LeapToward(struct Actor **startSlot, struct Actor **endSlot,
                             s32 progress) {
  struct Actor *start = *startSlot;
  struct Actor *end = *endSlot;
  FIXED x;
  FIXED z;

  x = start->x.fixed + (end->x.fixed - start->x.fixed) * progress / 100;
  z = start->z.fixed + (end->z.fixed - start->z.fixed) * progress / 100;
  REG_BLDCNT = 0;
  start->acceleration = toFIXED(2);
  start->speedLimit = toFIXED(8);
  start->velocityY = toFIXED(4);
  start->gravity = BATTLE_UNIT_GRAVITY;
  start->bounce = 0;
  start->steeringFlags = 1;
  Actor_SetMoveTargetFar(start, x, 0, z);
  Actor_SelectAnimationFar(start, POSE_LEAP);
}

/// Unused: sends a unit back to its spot.
void BattleMotion_ReturnObjectToSpot(struct BattleObject *slot) {
  struct MotionObject *object = slot->object;

  object->acceleration = toFIXED(2);
  object->speedLimit = toFIXED(8);
  object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
}

/// Sends unitId hopping out to one and a half times its spot's distance from
/// the centre.
void BattleMotion_HopBack(s32 unitId) {
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(unitId);
  object = slot->object;
  object->acceleration = FIXED_ONE;
  object->speedLimit = toFIXED(4);
  object->velocityY = toFIXED(3);
  object->verticalMotionStrength = toFIXED(3) / 5;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, slot->anchorX * 3 / 2, 0, slot->anchorZ);
}

/// Unused: sends unitId leaping back to 1.3 times its spot's distance from the
/// centre, in POSE_DAMAGE.
void BattleMotion_LeapBack(s32 unitId) {
  struct BattleObject *slot;
  struct MotionObject *object;

  slot = BattleObject_Get(unitId);
  object = slot->object;
  object->acceleration = FIXED_ONE;
  object->speedLimit = toFIXED(4);
  object->velocityY = toFIXED(4);
  object->verticalMotionStrength = toFIXED(3) / 5;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_StartIdleFar(object);
  {
    s32 (*mul)(s32 a, s32 b);
    s32 anchor = slot->anchorX;

    mul = Iwram_MulFX;
    Actor_SetMoveTargetFar(object, mul(anchor, FIXED_ONE * 13 / 10), 0,
                           slot->anchorZ);
  }
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

/// Knocks a unit back as the knockback variant (enum BattleKnockback) says, in
/// its damage pose; the Fusion Dragon and the Doom Dragon's three forms only
/// take the pose.
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
    // A unit already in the air keeps its hop, except from KNOCKBACK_SKID on.
    if (object->y == 0 || variant > 4)
      object->velocityY = gBattleKnockback_VelocityY[variant];
    Actor_StartIdleFar(object);
    scale = slot->anchorX;
    x = (scale * gBattleKnockback_DistancePercent[variant]) / 100;
    Actor_SetMoveTargetFar(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = toFIXED(3) / 5;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

/// Sends one unit leaping three quarters of the way to another, arriving in
/// travelFrames frames.
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelFrames,
                                 s32 initialVelocityY) {
  struct BattleObject *actorSlot = BattleObject_Get(actorId);
  struct BattleObject *targetSlot = BattleObject_Get(targetId);
  struct MotionObject *object = actorSlot->object;
  struct MotionObject *target = targetSlot->object;
  s32 percent = 75;
  s32 dx = target->x - object->x;
  s32 startX = object->x;
  s32 stepX = percent * dx / 100;
  s32 dz = target->z - object->z;
  s32 startZ = object->z;
  s32 stepZ = percent * dz / 100;
  s32 x = startX + stepX;
  s32 z = startZ + stepZ;
  // The distance is taken in 1/256 pixels, so its square fits 32 bits.
  s32 coarseX = stepX >> 8;
  s32 coarseZ = stepZ >> 8;
  s32 sum = coarseX * coarseX + coarseZ * coarseZ;
  s32 dist;

  if (sum != 0)
    dist = Iwram_Sqrt(sum) << 8;
  else
    dist = 0;
  dist = dist / travelFrames;
  object->acceleration = dist;
  object->speedLimit = dist;
  object->snapToTarget = 1;

  // DEAD: the store under ACTOR_FLOATS is overwritten at once; the ROM keeps
  // the test and its store.
  if (object->motionFlags & ACTOR_FLOATS)
    object->velocityY = initialVelocityY;
  object->velocityY = initialVelocityY;
  object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
  object->autoFaceMotion = 1;
  Actor_StartIdleFar(object);
  if (dist != 0)
    Actor_SetMoveTargetFar(object, x, 0, z);
  Actor_SelectAnimationFar(object, POSE_LEAP);
}

/// Unused: restarts a unit's motion in POSE_LEAP.
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
  if (x1 == ACTOR_NO_TARGET) {
    x1 = first->x;
  }
  z1 = first->targetZ;
  if (z1 == ACTOR_NO_TARGET) {
    z1 = first->z;
  }
  x2 = second->targetX;
  if (x2 == ACTOR_NO_TARGET) {
    x2 = second->x;
  }
  z2 = second->targetZ;
  if (z2 == ACTOR_NO_TARGET) {
    z2 = second->z;
  }

  SET_POSITION(&pos, (x1 + x2) / 2, 0, (z1 + z2) / 2);

  BattleMotion_MidpointNoOp(&pos, toFIXED(1) / 16);
}

/// Unused: plays the hit flash on a unit's sprite and waits ten frames. Only a
/// single-sprite actor takes it.
void BattleMotion_PlayHitFlash(s32 unitId) {
  struct BattleObject *battleObject = BattleObject_Get(unitId);
  struct SpriteAnimation *effect;

  if (battleObject->object != NULL) {
    if ((battleObject->object->recordStorageKind & ACTOR_SPRITE_KIND_MASK) ==
        ACTOR_SPRITE_ONE) {
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

/// The pixels above a unit where its battle effects play when its class gives
/// none: a small enemy's, and everyone else's.
#define EFFECT_HEIGHT_SMALL 24
#define EFFECT_HEIGHT_LARGE 48

/// Projects the point above a unit where its battle effects play
/// (BattleUnit_GetEffectHeight) onto the screen, into screen, scaled as its
/// sprite is. Returns 0.
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *screen) {
  struct MotionObject *object = BattleObject_Get(unitId)->object;
  struct Sprite *record = Actor_GetSprite(object, 0);
  FIXED scale;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 unused[3];

  BattleCamera_ApplyTransform();
  scale = Iwram_MulFX(Matrix_ProjectPoint(&object->x, screen), record->scale);
  screen[1] -=
      Iwram_MulFX(scale, FIXEDtoINT(BattleUnit_GetEffectHeight(unitId)));
  return 0;
}

/// Projects the point above a unit onto the screen, into screen: the target
/// cursor's place, EFFECT_HEIGHT_SMALL up for a small enemy and
/// EFFECT_HEIGHT_LARGE for others, scaled as the sprite is. Returns 0.
s32 BattleMotion_ProjectCursorPoint(s32 unitId, s32 *screen) {
  struct MotionObject *object = BattleObject_Get(unitId)->object;
  struct Sprite *record = Actor_GetSprite(object, 0);
  FIXED scale;

  BattleCamera_ApplyTransform();
  scale = Iwram_MulFX(Matrix_ProjectPoint(&object->x, screen), record->scale);
  if (BattleSprite_IsSmall(Unit_Get(unitId)->enemyId) != 0)
    screen[1] -= Iwram_MulFX(scale, EFFECT_HEIGHT_SMALL);
  else
    screen[1] -= Iwram_MulFX(scale, EFFECT_HEIGHT_LARGE);
  return 0;
}

/// Returns the height above a unit where its battle effects play, in 16.16
/// pixels: its class's own, else EFFECT_HEIGHT_SMALL for a small enemy and
/// EFFECT_HEIGHT_LARGE for others.
FIXED BattleUnit_GetEffectHeight(s32 unitId) {
  FIXED value =
      toFIXED((u8)BattleSprite_GetEffectHeight(Unit_Get(unitId)->enemyId));

  if (value == 0) {
    if (BattleSprite_IsSmall(Unit_Get(unitId)->enemyId) != 0)
      value = toFIXED(EFFECT_HEIGHT_SMALL);
    else
      value = toFIXED(EFFECT_HEIGHT_LARGE);
  }
  return value;
}
