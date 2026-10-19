/// @file
/// The units' movements: returning, running, leaping and knocked back, and
/// their positions on the screen.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

/// MotionObject.targetX and targetZ while the object has no move target.
#define NO_MOVE_TARGET 0x80000000

/// Sends unitId back to its spot, landing there, and turns it to face
/// the centre.
void ObjMotRst(s32 unitId) {
  s32 z;
  s32 stopped;
  struct BattleObj *battleObject;
  struct MotionObject *object;

  battleObject = BattleObjGet(unitId);
  object = battleObject->object;
  stopped = 0;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->velocityY = stopped;
  object->verticalMotionPhase = stopped;
  object->autoFaceMotion = stopped;
  object->snapToTarget = 1;
  ObjectDispatch_StartIdleFar(object);
  Object_SetMoveTargetFar(object, battleObject->anchorX, 0,
                          battleObject->anchorZ);
  z = battleObject->anchorZ;
  if (z < 0)
    z += 7;
  object->angle = ArcTan2(z >> 3, battleObject->anchorX) - 0x8000;
}

/// Sends unitId running off its side of the battlefield, three times
/// as far out as its spot, with a hop.
void BattleMotion_SetupEscapeObject(s32 unitId) {
  struct BattleObj *battleObject;
  struct MotionObject *object;

  battleObject = BattleObjGet(unitId);
  object = battleObject->object;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x50000;
  object->verticalMotionStrength = 0x7851;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_StartIdleFar(object);
  Object_SetMoveTargetFar(object, battleObject->anchorX * 3, 0,
                          battleObject->anchorZ);
  ObjectDispatch_SelectAnimationFar(object, 1);
}

/// Sends the unit of startSlot leaping progress percent of the way toward
/// the unit of endSlot.
void BattleMotion_InterpolatePosition(struct BattleObj *startSlot,
                                      struct BattleObj *endSlot, s32 progress) {
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
  Object_SetMoveTargetFar(start, x, 0, startZ + zStep);
  ObjectDispatch_SelectAnimationFar(start, 2);
}

/// Sends a unit's object back to its spot.
void BattleMotion_SetObjectPosition(struct BattleObj *battleObject) {
  struct MotionObject *object = battleObject->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_SetMoveTargetFar(object, battleObject->anchorX, 0,
                          battleObject->anchorZ);
}

/// Sends unitId hopping out to one and a half times its spot's
/// distance from the centre.
void BattleMotion_ResetObjectAtScaledAnchor(s32 unitId) {
  u32 scaledX;
  struct BattleObj *battleObject;
  struct MotionObject *object;

  battleObject = BattleObjGet(unitId);
  object = battleObject->object;
  object->acceleration = 0x10000;
  object->speedLimit = 0x40000;
  object->velocityY = 0x30000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_StartIdleFar(object);
  scaledX = battleObject->anchorX * 3;
  Object_SetMoveTargetFar(object, (s32)(scaledX + (scaledX >> 0x1F)) >> 1, 0,
                          battleObject->anchorZ);
}

/// Sends unit id leaping back to 1.3 times its spot's distance from the
/// centre, in pose 5.
void BattleMotion_InitializeObject(s32 unitId) {
  struct BattleObj *battleObject = BattleObjGet(unitId);
  struct MotionObject *object = battleObject->object;

  object->acceleration = 128 << 9;
  object->speedLimit = 128 << 11;
  object->velocityY = 128 << 11;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_StartIdleFar(object);
  Object_SetMoveTargetFar(object, Iwram_MulQ16(battleObject->anchorX, 0x14ccc),
                          0, battleObject->anchorZ);
  ObjectDispatch_SelectAnimationFar(object, 5);
}

#define CLASS_NO_KNOCKBACK 0x94 ///< a class that never moves when hit

/// Knocks unit id back, by the speed, hop and distance of motion variant
/// (gBattleMotion_Variant*), and puts it in pose 5. One class only takes
/// the pose.
void BattleMotion_ApplyVariantMotion(s32 unitId, s32 variant) {
  struct BattleObj *battleObject;
  struct MotionObject *object;
  s32 targetX;
  s32 anchorX;

  battleObject = BattleObjGet(unitId);
  object = battleObject->object;
  if (UnitGetFar(unitId)->classId != CLASS_NO_KNOCKBACK) {
    object->acceleration = gBattleMotion_VariantAcceleration[variant];
    object->speedLimit = gBattleMotion_VariantSpeedLimit[variant];
    if (object->y == 0 || variant > 4)
      object->velocityY = gBattleMotion_VariantVelocityY[variant];
    object->verticalMotionStrength = 0x9999;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 0;
    ObjectDispatch_StartIdleFar(object);
    anchorX = battleObject->anchorX;
    targetX = (anchorX * gBattleMotion_VariantDistancePercent[variant]) / 100;
    Object_SetMoveTargetFar(object, targetX, 0, battleObject->anchorZ);
  }
  ObjectDispatch_SelectAnimationFar(object, 5);
}

/// Sends unit actorId three quarters of the way to unit targetId, arriving
/// in travelDivisor steps, leaping up at initialVelocityY.
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY) {
  struct BattleObj *actorSlot = BattleObjGet(actorId);
  struct BattleObj *targetSlot = BattleObjGet(targetId);
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
  s32 cellX = stepX >> 8;
  s32 cellZ = stepZ >> 8;
  s32 speed;

  speed = Iwram_Sqrt(cellX * cellX + cellZ * cellZ);
  speed = (speed << 8) / travelDivisor;
  object->acceleration = speed;
  object->speedLimit = speed;
  object->snapToTarget = 1;

  // Removing the repeated store shortens the matching EN function by 16 bytes.
  if (object->motionFlags & 4)
    object->velocityY = initialVelocityY;
  object->velocityY = initialVelocityY;
  object->verticalMotionStrength = 0xab85;
  object->autoFaceMotion = 1;
  ObjectDispatch_StartIdleFar(object);
  Object_SetMoveTargetFar(object, x, 0, z);
  ObjectDispatch_SelectAnimationFar(object, 2);
}

/// Restarts unit id's motion in pose 2.
void BattleMotion_ResetSlotObjectMode2(s32 unitId) {
  struct MotionObject *object;

  object = BattleObjGet(unitId)->object;
  ObjectDispatch_StartIdleFar(object);
  ObjectDispatch_SelectAnimationFar(object, 2);
}

/// Does nothing; BattleMotion_ApplyPairMidpoint passes it a position and a
/// distance.
void BattleMotion_ReservedNoOp83B0() {}

/// Works out the point halfway between two units, at their move targets
/// where they have one, and hands it to BattleMotion_ReservedNoOp83B0,
/// which does nothing.
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId) {
  struct BattleObj *secondSlot;
  struct MotionObject *first;
  struct MotionObject *second;
  s32 x1;
  s32 x2;
  s32 z1;
  s32 z2;
  struct FixedPointPosition pos;

  first = BattleObjGet(firstId)->object;
  secondSlot = BattleObjGet(secondId);
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

  pos.x = (x1 + x2) / 2;
  pos.y = 0;
  pos.z = (z1 + z2) / 2;

  BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}

#define HIT_FLASH_ANIM 0x11b

/// Plays the hit flash on unit id's sprite and waits ten frames.
void BattleMotion_SpawnSlotEffectAndWait(s32 unitId) {
  struct MotionObject *object;
  struct AnimationEntry *effect;
  s32 effectHandle;

  object = BattleObjGet(unitId)->object;
  if ((object != NULL) &&
      ((RECORDS_KIND_MASK & object->recordStorageKind) == RECORDS_SINGLE)) {
    effectHandle =
        ResourceMetadata_RegisterFar(object->records, HIT_FLASH_ANIM);
    if (effectHandle != 0) {
      effect = (struct AnimationEntry *)effectHandle;
      Animation_SetWorkEntryFar(effect, 1);
      effect->priority = 3;
    }
    WaitFrames(0xA);
  }
}

/// Projects the point above unit id where its effects play (its class's
/// height, BattleUnit_GetEffectHeight) onto the screen, into projected.
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected) {
  struct MotionObject *object = BattleObjGet(unitId)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 projectionScale;
  s32 spriteScale;
  /* FAKEMATCH(dead): an unused position for the frame's 12 bytes; without it,
   * 2 of 42 instructions differ in tbs-en. */
  struct EffectPosition input;

  Camera_ApplyTransformByFlag();
  projectionScale = Render_ProjectPoint(&object->x, projected);
  spriteScale = Iwram_MulQ16(projectionScale, record->scale);
  projectionScale =
      Iwram_MulQ16(spriteScale, (s32)BattleUnit_GetEffectHeight(unitId) >> 16);
  projected[1] -= projectionScale;
  return 0;
}

/// Projects the point above unit id onto the screen, into projected: 24
/// pixels up for a small enemy, 48 for others, as drawn.
s32 BattleMotion_ProjectConditionalPosition(s32 unitId, s32 *projected) {
  struct MotionObject *object = BattleObjGet(unitId)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 projectedHeight;
  s32 spriteScale;

  Camera_ApplyTransformByFlag();
  projectedHeight = Render_ProjectPoint(&object->x, projected);
  spriteScale = Iwram_MulQ16(projectedHeight, record->scale);
  if (BattleClass_IsSmall(UnitGetFar(unitId)->classId) != 0)
    projectedHeight = Iwram_MulQ16(spriteScale, 24);
  else
    projectedHeight = Iwram_MulQ16(spriteScale, 48);
  projected[1] -= projectedHeight;
  return 0;
}

/// Returns the height above unit id where its effects play, 16.16: its
/// class's own (BattleClass_GetEffectHeight), else 24 for a small enemy and 48
/// for others.
s32 BattleUnit_GetEffectHeight(s32 unitId) {
  u32 height;
  u8 classId;

  classId = UnitGetFar(unitId)->classId;
  height = (u8)BattleClass_GetEffectHeight(classId) << 16;
  if (height == 0) {
    classId = UnitGetFar(unitId)->classId;
    if (BattleClass_IsSmall(classId) != 0) {
      height = 0x180000;
    } else {
      height = 0x300000;
    }
  }
  return height;
}
