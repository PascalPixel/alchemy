/// @file
/// The units' movements: returning, running, leaping and knocked back, and
/// their positions on the screen.
// Part of battle.c.

#include "../battle_internal.h"

/// MotionObject.targetX and targetZ while the object has no move target.
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
  struct MotionObject *object;

  battleObject = BattleObject_Get(unitId);
  object = battleObject->object;
  stopped = 0;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->velocityY = stopped;
  object->verticalMotionPhase = stopped;
  object->autoFaceMotion = stopped;
  object->snapToTarget = 1;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  Object_SetMoveTargetFar((struct Object *)object, battleObject->anchorX, 0,
                          battleObject->anchorZ);
  // z / 8, rounded toward zero, as BattleObject_Make turns a new object.
  z = battleObject->anchorZ;
  if (z < 0)
    z += 7;
  object->angle = ArcTan2(z >> 3, battleObject->anchorX) - 0x8000;
}

/// Sends unitId running off its side of the battlefield, three times
/// as far out as its spot, with a hop, in POSE_STANDING. Called by
/// BattleFlee_PlayRun and BattleStaging_RunFlee.
void BattleMotion_SetupEscapeObject(s32 unitId) {
  struct BattleObject *battleObject;
  struct MotionObject *object;

  battleObject = BattleObject_Get(unitId);
  object = battleObject->object;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x50000;
  object->verticalMotionStrength = 0x7851;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  Object_SetMoveTargetFar((struct Object *)object, battleObject->anchorX * 3, 0,
                          battleObject->anchorZ);
  ObjectDispatch_SelectAnimationFar(object, POSE_STANDING);
}

/// Sends the unit of startSlot leaping progress percent of the way toward
/// the unit of endSlot, in POSE_LEAP. Also clears REG_BLDCNT. Nothing in the
/// source calls it.
void BattleMotion_InterpolatePosition(struct BattleObject *startSlot,
                                      struct BattleObject *endSlot,
                                      s32 progress) {
  s32 zStep;
  s32 startX;
  s32 x;
  s32 startZ;
  s32 endZ;
  struct MotionObject *start;
  struct MotionObject *end;

  start = startSlot->object;
  end = endSlot->object;
  startX = start->x;
  x = startX + progress * (end->x - startX) / 100;
  endZ = end->z;
  startZ = start->z;
  zStep = progress * (endZ - startZ) / 100;
  REG_BLDCNT = 0;
  start->acceleration = 0x20000;
  start->speedLimit = 0x80000;
  start->velocityY = 0x40000;
  start->verticalMotionStrength = 0xab85;
  start->verticalMotionPhase = 0;
  start->autoFaceMotion = 1;
  Object_SetMoveTargetFar((struct Object *)start, x, 0, startZ + zStep);
  ObjectDispatch_SelectAnimationFar(start, POSE_LEAP);
}

/// Sends a unit's object back to its spot. Nothing in the source calls it.
void BattleMotion_SetObjectPosition(struct BattleObject *battleObject) {
  struct MotionObject *object = battleObject->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_SetMoveTargetFar((struct Object *)object, battleObject->anchorX, 0,
                          battleObject->anchorZ);
}

/// Sends unitId hopping out to one and a half times its spot's
/// distance from the centre. Called by BattleStaging_RunAttack and
/// BattlePresentation_RunPlainAttack.
void BattleMotion_ResetObjectAtScaledAnchor(s32 unitId) {
  u32 scaledX;
  struct BattleObject *battleObject;
  struct MotionObject *object;

  battleObject = BattleObject_Get(unitId);
  object = battleObject->object;
  object->acceleration = 0x10000;
  object->speedLimit = 0x40000;
  object->velocityY = 0x30000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  // anchorX * 3 / 2, rounded toward zero.
  scaledX = battleObject->anchorX * 3;
  Object_SetMoveTargetFar((struct Object *)object,
                          (s32)(scaledX + (scaledX >> 0x1F)) >> 1, 0,
                          battleObject->anchorZ);
}

/// Sends unit id leaping back to 1.3 times its spot's distance from the
/// centre, in POSE_DAMAGE. Only an unnamed overlay veneer in
/// BATTLE.S reaches it, and nothing in the source calls that.
void BattleMotion_InitializeObject(s32 unitId) {
  struct BattleObject *battleObject = BattleObject_Get(unitId);
  struct MotionObject *object = battleObject->object;

  object->acceleration = 128 << 9;
  object->speedLimit = 128 << 11;
  object->velocityY = 128 << 11;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  Object_SetMoveTargetFar((struct Object *)object,
                          Iwram_MulQ16(battleObject->anchorX, 0x14ccc), 0,
                          battleObject->anchorZ);
  ObjectDispatch_SelectAnimationFar(object, POSE_DAMAGE);
}

/// Knocks unit id back by the speed, hop and distance of the knockback
/// variant (enum BattleKnockback), and puts it in POSE_DAMAGE.
/// The Fusion Dragon only takes the pose. Reached through its far stub from
/// the effects, by the ANIM_STRIKE_TARGET macro of effect_internal.h and
/// directly.
void BattleMotion_ApplyKnockback(s32 unitId, s32 variant) {
  struct BattleObject *battleObject;
  struct MotionObject *object;
  s32 targetX;
  s32 anchorX;

  battleObject = BattleObject_Get(unitId);
  object = battleObject->object;
  if (Unit_GetFar(unitId)->id != ENEMY_FUSION_DRAGON) {
    object->acceleration = gBattleKnockback_Acceleration[variant];
    object->speedLimit = gBattleKnockback_SpeedLimit[variant];
    // A unit already in the air keeps its hop, except from KNOCKBACK_SKID on.
    if (object->y == 0 || variant > KNOCKBACK_HURL)
      object->velocityY = gBattleKnockback_VelocityY[variant];
    object->verticalMotionStrength = 0x9999;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    anchorX = battleObject->anchorX;
    targetX = (anchorX * gBattleKnockback_DistancePercent[variant]) / 100;
    Object_SetMoveTargetFar((struct Object *)object, targetX, 0,
                            battleObject->anchorZ);
  }
  ObjectDispatch_SelectAnimationFar(object, POSE_DAMAGE);
}

/// Sends unit actorId three quarters of the way to unit targetId, arriving
/// in travelDivisor frames at a steady speed, leaping up at initialVelocityY
/// (16.16 pixels a frame), in POSE_LEAP.
/// Called by BattleStaging_RunAttack, BattlePresentation_RunPlainAttack
/// and BattlePresentation_RunApproachAction, and through its far stub by the
/// Bind, burst scene, Annihilation, target motes and Thrash effects.
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
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
  s32 cellX = stepX >> 8;
  s32 cellZ = stepZ >> 8;
  s32 speed;

  speed = Iwram_Sqrt(cellX * cellX + cellZ * cellZ);
  speed = (speed << 8) / travelDivisor;
  object->acceleration = speed;
  object->speedLimit = speed;
  object->snapToTarget = 1;

  // Removing the repeated store shortens the matching EN function by 16 bytes.
  // The test is for a floating unit (OBJ_EVENT_FLOATS); both paths store
  // the same hop.
  if (object->motionFlags & OBJ_EVENT_FLOATS)
    object->velocityY = initialVelocityY;
  object->velocityY = initialVelocityY;
  object->verticalMotionStrength = 0xab85;
  object->autoFaceMotion = 1;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  Object_SetMoveTargetFar((struct Object *)object, x, 0, z);
  ObjectDispatch_SelectAnimationFar(object, POSE_LEAP);
}

/// Restarts unit id's motion in POSE_LEAP. Nothing in the source calls it.
void BattleMotion_ResetSlotObjectMode2(s32 unitId) {
  struct MotionObject *object;

  object = BattleObject_Get(unitId)->object;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  ObjectDispatch_SelectAnimationFar(object, POSE_LEAP);
}

/// Does nothing; BattleMotion_ApplyPairMidpoint, its only caller, passes it
/// a position and a distance.
void BattleMotion_ReservedNoOp83B0() {}

/// Works out the point halfway between two units, at their move targets
/// where they have one, and hands it to BattleMotion_ReservedNoOp83B0,
/// which does nothing. Nothing in the source calls it.
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId) {
  struct BattleObject *secondSlot;
  struct MotionObject *first;
  struct MotionObject *second;
  s32 x1;
  s32 x2;
  s32 z1;
  s32 z2;
  struct FixedPointPosition position;

  first = BattleObject_Get(firstId)->object;
  secondSlot = BattleObject_Get(secondId);
  second = secondSlot->object;

  x1 = first->targetX;
  if (x1 == NO_MOVE_TARGET) {
    x1 = first->x;
  }
  z1 = first->targetZ;
  if (z1 == NO_MOVE_TARGET) {
    z1 = first->z;
  }
  x2 = second->targetX;
  if (x2 == NO_MOVE_TARGET) {
    x2 = second->x;
  }
  z2 = second->targetZ;
  if (z2 == NO_MOVE_TARGET) {
    z2 = second->z;
  }

  position.x = (x1 + x2) / 2;
  position.y = 0;
  position.z = (z1 + z2) / 2;

  BattleMotion_ReservedNoOp83B0(&position, 0x1000);
}

/// The hit flash's sprite layer, sprite 283: a star burst that grows and
/// breaks apart over ten frames (graphics/sprite/spr_283.png).
#define HIT_FLASH_ANIMATION 0x11b

/// Plays the hit flash on unit id's sprite and waits ten frames. Only a
/// single-sprite object takes it. Nothing in the source calls it.
void BattleMotion_SpawnSlotEffectAndWait(s32 unitId) {
  struct MotionObject *object;
  struct AnimationEntry *effect;
  s32 effectHandle;

  object = BattleObject_Get(unitId)->object;
  if ((object != NULL) && ((OBJECT_SPRITES_KIND_MASK & object->spritesKind) ==
                           OBJECT_SPRITES_ONE)) {
    effectHandle =
        ResourceMetadata_RegisterFar(object->records, HIT_FLASH_ANIMATION);
    if (effectHandle != 0) {
      effect = (struct AnimationEntry *)effectHandle;
      Animation_SetWorkEntryFar(effect, 1);
      effect->priority = 3;
    }
    WaitFrames(0xA);
  }
}

/// Projects the point above unit id where its effects play (its class's
/// height, BattleUnit_GetEffectHeight) onto the screen, into projected; the
/// height is scaled by the sprite's projected scale. Returns 0. Called by
/// ElementGlow_Play, and through its far stub by
/// Projection_ScaledUnit and SelectionScreen_Draw.
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected) {
  struct MotionObject *object = BattleObject_Get(unitId)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 projectionScale;
  s32 spriteScale;
  /* FAKEMATCH(dead): an unused position for the frame's 12 bytes; without it,
   * 2 of 42 instructions differ in tbs-en. */
  UNUSED struct ScreenPoint input;

  Camera_ApplyTransformByFlag();
  projectionScale = Render_ProjectPoint(&object->x, projected);
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
  struct MotionObject *object = BattleObject_Get(unitId)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 projectedHeight;
  s32 spriteScale;

  Camera_ApplyTransformByFlag();
  projectedHeight = Render_ProjectPoint(&object->x, projected);
  spriteScale = Iwram_MulQ16(projectedHeight, record->scale);
  if (BattleSprite_IsSmall(Unit_GetFar(unitId)->id) != 0)
    projectedHeight = Iwram_MulQ16(spriteScale, 24);
  else
    projectedHeight = Iwram_MulQ16(spriteScale, 48);
  projected[1] -= projectedHeight;
  return 0;
}

/// Returns the height above unit id where its effects play, 16.16: its
/// class's own (BattleSprite_GetAnimHeight), else 24 for a small enemy and 48
/// for others. Called by BattleMotion_ProjectScaledPosition, and through its
/// far stub by the effects that play at a unit's body.
s32 BattleUnit_GetEffectHeight(s32 unitId) {
  u32 height;
  u8 classId;

  classId = Unit_GetFar(unitId)->id;
  height = (u8)BattleSprite_GetAnimHeight(classId) << 16;
  if (height == 0) {
    classId = Unit_GetFar(unitId)->id;
    if (BattleSprite_IsSmall(classId) != 0) {
      height = 0x180000;
    } else {
      height = 0x300000;
    }
  }
  return height;
}
