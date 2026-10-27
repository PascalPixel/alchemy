/// @file
/// Force.
// Part of field_services.c.

#include "../field_services_internal.h"

/// Runs Force cast on a target, through FieldEffect_RunForce.
///
/// Abilities: Force (FIELD_PSYNERGY_FORCE).
void FieldEffect_RunForceOnTarget(s32 unused) { FieldEffect_RunForce(); }

/// Writes the position 16 pixels above the actor.
static __inline__ void FieldEffect_GetPositionAboveActor(struct Actor *actor,
                                                         struct Vector3 *pos) {
  SET_POSITION(pos, actor->x.fixed, actor->y.fixed + ABOVE, actor->z.fixed);
}

/// A shot appears in front of the caster, grows and flies forward with three
/// afterimages; what it hits (effectWork->child) is pushed 16 pixels.
///
/// Abilities: Force (FIELD_PSYNERGY_FORCE).
///
/// The shot is object 215, the hand of Move and Carry, flying at 4 pixels a
/// frame; where it lands it makes a heavy impact
/// (gFieldEffect_HeavyImpactScript, SpawnHeavyImpactEffect). Called by
/// FieldEffect_Run, and through FieldEffect_RunForceOnTarget.
void FieldEffect_RunForce(void) {
  struct Actor *target;
  s32 eventTarget;
  struct Actor *copies[4];
  struct Vector3 pos;
  struct Actor *projectile;
  struct Actor *copy;
  struct Actor *caster;
  struct FieldEffectWork *effectWork;
  struct FieldEffectSprite *effect;
  struct Sprite *sharedAnimation;
  void *event;
  s32 scale;
  s32 i;
  u8 resourceSlot;

  effectWork = gFieldEffectWork;
  target = effectWork->child;
  FieldEffect_FadeToEffectColor();
  Sound_PlaySongFar(SE_PSYNERGY_START);
  effect = effectWork->sprites;

  // 12 flashes burst from over the caster's head, one every 2 frames.
  for (i = 0; i <= 11; i++) {
    caster = effectWork->mainObject;
    FieldEffect_GetPositionAboveActor(caster, &pos);
    Camera_WorldToScreen(&pos);
    FieldEffectSprite_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldEffectSprite_SetCallback(effect, FieldEffect_UpdateRadialBurst);
    FieldEffectSprite_SetObjectMode(effect, 7);
    Sprite_SetAnimColorModesFar(effect->object, SPRITE_COLOR_BLUE_TINT);
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
  projectile = Object_Spawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
  if (projectile == NULL) {
    FieldEffect_FadeFromEffectColor();
    return;
  }
  projectile->scaleY = 0x4000;
  projectile->scaleX = 0x4000;
  projectile->facing = effectWork->angle;
  projectile->speedLimit = 4 << 16;
  projectile->acceleration = 4 << 16;
  projectile->motionFlags = 0;
  Object_SelectAnimationFar(projectile, 5);
  Anim_ApplyChildValuesFar(projectile, SPRITE_COLOR_BGR);
  scale = projectile->scaleX;
  if (scale < Q16_ONE) {
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
    copy = copies[i] = Object_Spawn(SPRITE_MOVE_HAND, projectile->x.fixed,
                                    projectile->y.fixed, projectile->z.fixed);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = effectWork->angle;
      copy->speedLimit = 4 << 16;
      copy->acceleration = 4 << 16;
      copy->motionFlags = 0;
      Object_SelectAnimationFar(copy, 5);
      Anim_ApplyChildValuesFar(copy, SPRITE_COLOR_RBG);
      sharedAnimation = Sprite_ShareVramCacheEntry(
          (struct Sprite *)copy->sprite, sharedAnimation);
    }
  }
  resourceSlot = sharedAnimation->slot;

  // Destination: 56 pixels in front of the caster with effectWork->enabled set
  // (no trigger named the target), otherwise the effect's position.
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
  Object_SetMoveTargetFar(projectile, pos.x, pos.y, pos.z);
  Object_InitializeFar(projectile, gFieldEffect_BurstParticleObjectScript + 4);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Object_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      Object_InitializeFar(copy, gFieldEffect_CommonParticleScript + 1);
    }
  }

  // Wait, at most 60 frames, for the shot's script to end.
  for (i = 0; i <= 59 && (projectile)->script != NULL; i++)
    WaitFrames(1);

  // Push what was hit 16 pixels, at 1 pixel a frame, if nothing is in the
  // way: the ground there must be within a step (MapGrid_CheckCollision 0) and
  // free of objects. It hops (8 pixels a frame) with childOption; a map
  // script's childMode leaves it be.
  if (target != NULL && effectWork->childMode == 0) {
    if (effectWork->childOption != 0)
      target->velocityY = 8 << 16;
    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, effectWork->angle, &pos);
    if (MapGrid_CheckCollisionFar(target, (struct FieldPosition *)&pos) ==
            COLLISION_NONE &&
        ObjectScript_CheckOverlapFar(target, (s32 *)&pos) == 0) {
      target->acceleration = 1 << 16;
      target->speedLimit = 1 << 16;
      Object_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  // Run the map's event for it, if there is one.
  event =
      Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_EFFECT_ACTS),
                                FIELD_PSYNERGY_FORCE, &eventTarget);
  if (event != NULL)
    Field_RunEventAction(event, gSaveBlock.leaderActorId, eventTarget);
  WaitFrames(10);
  FieldEffect_FadeFromEffectColor();
  WaitFrames(20);
  // 96 (0x60), the VRAM cache's entry count, means no entry to free.
  if (resourceSlot != 96)
    VramCache_FreeEntry(resourceSlot);
}
