/// @file
/// Force.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Runs Force cast on a target, through FieldFx_RunForce.
///
/// Abilities: Force (field kind 4), cast on a target.
void FieldFx_RunForceOnTarget(s32 unused) { FieldFx_RunForce(); }

/// Writes the position 16 pixels above the actor.
static __inline__ void
BattleFx_GetPositionAboveActor(struct FieldActor *obj,
                               struct EffectVector *pos) {
  SET_POSITION(pos, obj->x.fixed, obj->y.fixed + ABOVE, obj->z.fixed);
}

/// A shot appears in front of the caster, grows and flies forward with three
/// afterimages; what it hits (scene->child) is pushed 16 pixels.
///
/// Abilities: Force (field kind 4).
///
/// The shot is object 215, the hand of Move and Carry, flying at 4 pixels a
/// frame; where it lands it makes a heavy impact (gBattleFx_HeavyImpactScript,
/// SpawnHeavyImpactEffect). Called by BattleFx_Run, and through
/// FieldFx_RunForceOnTarget.
void FieldFx_RunForce(void) {
  struct FieldActor *target;
  s32 eventTarget;
  struct FieldActor *copies[4];
  struct EffectVector pos;
  struct FieldActor *projectile;
  struct FieldActor *copy;
  struct FieldActor *caster;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  struct AnimationObject *sharedAnimation;
  void *event;
  s32 scale;
  s32 i;
  u8 resourceSlot;

  scene = gEffectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(SE_130);
  slot = scene->slots;

  // 12 flashes burst from over the caster's head, one every 2 frames.
  for (i = 0; i <= 11; i++) {
    caster = scene->mainObject;
    BattleFx_GetPositionAboveActor(caster, &pos);
    Camera_WorldToScreen(&pos);
    EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 9);
    slot->scaleY = 0xb333; // 0.7
    slot->scaleX = 0xb333;
    WaitFrames(2);
    slot++;
  }

  // The shot: 8 pixels in front of the caster, growing from 0.25 to full
  // size by 0x500 a frame (39 frames).
  caster = scene->mainObject;
  pos.x = caster->x.fixed;
  pos.y = caster->y.fixed + ABOVE;
  pos.z = caster->z.fixed;
  Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
  projectile = (struct FieldActor *)Object_Spawn(0xd7, pos.x, pos.y, pos.z);
  if (projectile == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }
  projectile->scaleY = 0x4000;
  projectile->scaleX = 0x4000;
  projectile->facing = scene->angle;
  projectile->speed = 4 << 16;
  projectile->acceleration = 4 << 16;
  projectile->motionFlags = 0;
  ObjectDispatch_SelectAnimationFar(projectile, 5);
  Animation_ApplyChildValuesFar((struct DispatchObject *)projectile, 3);
  scale = projectile->scaleX;
  if (scale < 0x10000) {
    do {
      scale += 0x500;
      projectile->scaleY = scale;
      projectile->scaleX = scale;
      WaitFrames(1);
      scale = projectile->scaleX;
    } while (scale <= 0xffff);
  }
  WaitFrames(3);

  // Three afterimages, sharing one set of graphics.
  sharedAnimation = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] = (struct FieldActor *)Object_Spawn(
        0xd7, projectile->x.fixed, projectile->y.fixed, projectile->z.fixed);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = scene->angle;
      copy->speed = 4 << 16;
      copy->acceleration = 4 << 16;
      copy->motionFlags = 0;
      ObjectDispatch_SelectAnimationFar(copy, 5);
      Animation_ApplyChildValuesFar((struct DispatchObject *)copy, 2);
      sharedAnimation = Sprite_ShareVramCache(
          (struct AnimationObject *)copy->sprite, sharedAnimation);
    }
  }
  resourceSlot = sharedAnimation->slot;

  // Destination: 56 pixels in front of the caster with scene->enabled set
  // (no scene event named the target), otherwise the effect's position.
  // The shot's script waits to arrive and makes the impact
  // (gBattleFx_BurstParticleObjectScript + 4 is gBattleFx_HeavyImpactScript);
  // each afterimage follows 3 frames apart and is freed on arrival.
  if (scene->enabled != 0) {
    caster = scene->mainObject;
    pos.x = caster->x.fixed;
    pos.y = caster->y.fixed + ABOVE;
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(56 << 16, scene->angle, &pos);
  } else {
    pos.x = scene->x;
    pos.y = scene->y + ABOVE;
    pos.z = scene->z;
  }
  Object_SetMoveTargetFar((struct ObjectRuntime *)projectile, pos.x, pos.y,
                          pos.z);
  ObjectDispatch_InitializeFar((struct DispatchObject *)projectile,
                               gBattleFx_BurstParticleObjectScript + 4);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Object_SetMoveTargetFar((struct ObjectRuntime *)copy, pos.x, pos.y,
                              pos.z);
      ObjectDispatch_InitializeFar((struct DispatchObject *)copy,
                                   gBattleFx_CommonParticleScript + 1);
    }
  }

  // Wait, at most 60 frames, for the shot's script to end.
  for (i = 0;
       i <= 59 && ((struct ScriptMotionObject *)projectile)->script != NULL;
       i++)
    WaitFrames(1);

  // Push what was hit 16 pixels, at 1 pixel a frame, if nothing is in the
  // way: the ground there must be within a step (Map_CheckObjectStep 0) and
  // free of objects. It hops (8 pixels a frame) with childOption; a scene's
  // childMode leaves it be.
  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = 8 << 16;
    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, scene->angle, &pos);
    if (Map_CheckObjectStepFar((struct ObjectRuntime *)target,
                               (struct FieldPosition *)&pos) == 0 &&
        ScriptObject_CheckOverlapFar((struct ScriptObjectEntry *)target,
                                     (s32 *)&pos) == 0) {
      target->acceleration = 1 << 16;
      target->speed = 1 << 16;
      Object_SetMoveTargetFar((struct ObjectRuntime *)target, pos.x, pos.y,
                              pos.z);
    }
  }

  // Run the map's event for it, if there is one.
  event = BattleFx_FindMatchingEvent(0x50000005, 4, &eventTarget);
  if (event != NULL)
    Field_RunEventAction(event, gGameState.selectedActor, eventTarget);
  WaitFrames(10);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(20);
  // 96 (0x60), the VRAM cache's entry count, means no entry to free.
  if (resourceSlot != 96)
    Resource_ResetEntry(resourceSlot);
}
