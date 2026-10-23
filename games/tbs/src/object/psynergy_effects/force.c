/// @file
/// Force.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Runs Force cast on a target, through FieldEffect_RunForce.
///
/// Abilities: Force (FIELD_KIND_FORCE).
void FieldEffect_RunForceOnTarget(s32 unused) { FieldEffect_RunForce(); }

/// Writes the position 16 pixels above the object event.
static __inline__ void
FieldEffect_GetPositionAboveObjectEvent(struct ObjectEvent *objectEvent,
                                        struct EffectVector *pos) {
  SET_POSITION(pos, objectEvent->x.fixed, objectEvent->y.fixed + ABOVE,
               objectEvent->z.fixed);
}

/// A shot appears in front of the caster, grows and flies forward with three
/// afterimages; what it hits (scene->child) is pushed 16 pixels.
///
/// Abilities: Force (FIELD_KIND_FORCE).
///
/// The shot is object 215, the hand of Move and Carry, flying at 4 pixels a
/// frame; where it lands it makes a heavy impact
/// (gFieldEffect_HeavyImpactScript, SpawnHeavyImpactEffect). Called by
/// FieldEffect_Run, and through FieldEffect_RunForceOnTarget.
void FieldEffect_RunForce(void) {
  struct ObjectEvent *target;
  s32 eventTarget;
  struct ObjectEvent *copies[4];
  struct EffectVector pos;
  struct ObjectEvent *projectile;
  struct ObjectEvent *copy;
  struct ObjectEvent *caster;
  struct FieldEffectWork *effectWork;
  struct EffectSlot *effect;
  struct AnimationObject *sharedAnimation;
  void *event;
  s32 scale;
  s32 i;
  u8 resourceSlot;

  effectWork = gFieldEffectWork;
  target = effectWork->child;
  FieldEffect_FadeToEffectColor();
  Sound_PlaySongFar(SE_130);
  effect = effectWork->effects;

  // 12 flashes burst from over the caster's head, one every 2 frames.
  for (i = 0; i <= 11; i++) {
    caster = effectWork->mainObject;
    FieldEffect_GetPositionAboveObjectEvent(caster, &pos);
    Camera_WorldToScreen(&pos);
    EffectSlot_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    EffectSlot_SetCallback(effect, FieldEffect_UpdateRadialBurst);
    EffectSlot_SetObjectMode(effect, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(effect->object,
                                              SPRITE_COLOR_BLUE_TINT);
    effect->scaleY = 0xb333; // 0.7
    effect->scaleX = 0xb333;
    WaitFrames(2);
    effect++;
  }

  // The shot: 8 pixels in front of the caster, growing from 0.25 to full
  // size by 0x500 a frame (39 frames).
  caster = effectWork->mainObject;
  pos.x = caster->x.fixed;
  pos.y = caster->y.fixed + ABOVE;
  pos.z = caster->z.fixed;
  Vector_AddPolarOffset(8 << 16, effectWork->angle, &pos);
  projectile = (struct ObjectEvent *)Object_Spawn(0xd7, pos.x, pos.y, pos.z);
  if (projectile == NULL) {
    FieldEffect_FadeFromEffectColor();
    return;
  }
  projectile->scaleY = 0x4000;
  projectile->scaleX = 0x4000;
  projectile->facing = effectWork->angle;
  projectile->speed = 4 << 16;
  projectile->acceleration = 4 << 16;
  projectile->motionFlags = 0;
  ObjectDispatch_SelectAnimationFar(projectile, 5);
  Animation_ApplyChildValuesFar((struct DispatchObject *)projectile,
                                SPRITE_COLOR_BGR);
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
    copy = copies[i] = (struct ObjectEvent *)Object_Spawn(
        0xd7, projectile->x.fixed, projectile->y.fixed, projectile->z.fixed);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = effectWork->angle;
      copy->speed = 4 << 16;
      copy->acceleration = 4 << 16;
      copy->motionFlags = 0;
      ObjectDispatch_SelectAnimationFar(copy, 5);
      Animation_ApplyChildValuesFar((struct DispatchObject *)copy,
                                    SPRITE_COLOR_RBG);
      sharedAnimation = Sprite_ShareVramCache(
          (struct AnimationObject *)copy->sprite, sharedAnimation);
    }
  }
  resourceSlot = sharedAnimation->slot;

  // Destination: 56 pixels in front of the caster with scene->enabled set
  // (no map event named the target), otherwise the effect's position.
  // The shot's script waits to arrive and makes the impact
  // (gFieldEffect_BurstParticleObjectScript + 4 is
  // gFieldEffect_HeavyImpactScript); each afterimage follows 3 frames apart and
  // is freed on arrival.
  if (effectWork->enabled != 0) {
    caster = effectWork->mainObject;
    pos.x = caster->x.fixed;
    pos.y = caster->y.fixed + ABOVE;
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(56 << 16, effectWork->angle, &pos);
  } else {
    pos.x = effectWork->x;
    pos.y = effectWork->y + ABOVE;
    pos.z = effectWork->z;
  }
  Object_SetMoveTargetFar((struct Object *)projectile, pos.x, pos.y, pos.z);
  ObjectDispatch_InitializeFar((struct DispatchObject *)projectile,
                               gFieldEffect_BurstParticleObjectScript + 4);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Object_SetMoveTargetFar((struct Object *)copy, pos.x, pos.y, pos.z);
      ObjectDispatch_InitializeFar((struct DispatchObject *)copy,
                                   gFieldEffect_CommonParticleScript + 1);
    }
  }

  // Wait, at most 60 frames, for the shot's script to end.
  for (i = 0;
       i <= 59 && ((struct ScriptMotionObject *)projectile)->script != NULL;
       i++)
    WaitFrames(1);

  // Push what was hit 16 pixels, at 1 pixel a frame, if nothing is in the
  // way: the ground there must be within a step (MapGrid_CheckObjectStep 0) and
  // free of objects. It hops (8 pixels a frame) with childOption; a map
  // script's childMode leaves it be.
  if (target != NULL && effectWork->childMode == 0) {
    if (effectWork->childOption != 0)
      target->velocityY = 8 << 16;
    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, effectWork->angle, &pos);
    if (MapGrid_CheckObjectStepFar((struct Object *)target,
                                   (struct FieldPosition *)&pos) ==
            MAPGRID_STEP_OPEN &&
        ScriptObject_CheckOverlapFar((struct ScriptObjectEntry *)target,
                                     (s32 *)&pos) == 0) {
      target->acceleration = 1 << 16;
      target->speed = 1 << 16;
      Object_SetMoveTargetFar((struct Object *)target, pos.x, pos.y, pos.z);
    }
  }

  // Run the map's event for it, if there is one.
  event = FieldEffect_FindMatchingEvent(PSYNERGY_EV(PSYNERGY_PHASE_EFFECT_ACTS),
                                        FIELD_KIND_FORCE, &eventTarget);
  if (event != NULL)
    Field_RunEventAction(event, gGameState.selectedLocalId, eventTarget);
  WaitFrames(10);
  FieldEffect_FadeFromEffectColor();
  WaitFrames(20);
  // 96 (0x60), the VRAM cache's entry count, means no entry to free.
  if (resourceSlot != 96)
    Resource_ResetEntry(resourceSlot);
}
