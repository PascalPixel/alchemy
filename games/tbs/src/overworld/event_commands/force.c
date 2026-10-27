/// @file
/// Force.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Runs Force cast on a target, through FieldPsynergy_Force.
///
/// Abilities: Force (FIELD_PSYNERGY_FORCE).
void FieldPsynergy_ForceOnTarget(s32 unused) { FieldPsynergy_Force(); }

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
/// FieldPsynergy_Run, and through FieldPsynergy_ForceOnTarget.
void FieldPsynergy_Force(void) {
  struct Actor *target;
  s32 eventTarget;
  struct Actor *copies[4];
  struct Vector3 pos;
  struct Actor *projectile;
  struct Actor *copy;
  struct Actor *caster;
  struct FieldEffectWork *effectWork;
  struct FieldParticle *effect;
  struct Sprite *sharedAnimation;
  void *event;
  s32 scale;
  s32 i;
  u8 resourceSlot;

  effectWork = gFieldEffectWork;
  target = effectWork->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlaySongFar(SE_PSYNERGY_START);
  effect = effectWork->particles;

  // 12 flashes burst from over the caster's head, one every 2 frames.
  for (i = 0; i <= 11; i++) {
    caster = effectWork->mainActor;
    FieldEffect_GetPositionAboveActor(caster, &pos);
    Camera_WorldToScreen(&pos);
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRadialBurst);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimColorModesFar(effect->actor, SPRITE_COLOR_BLUE_TINT);
    effect->scaleY = 0xb333; // 0.7
    effect->scaleX = 0xb333;
    WaitFrames(2);
    effect++;
  }

  // The shot: 8 pixels in front of the caster, growing from 0.25 to full
  // size by 0x500 a frame (39 frames).
  caster = effectWork->mainActor;
  pos.x = caster->x.fixed;
  pos.y = caster->y.fixed + ABOVE;
  pos.z = caster->z.fixed;
  Vector_AddPolarOffset(8 << 16, effectWork->angle, &pos);
  projectile = Actor_Spawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
  if (projectile == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  projectile->scaleY = 0x4000;
  projectile->scaleX = 0x4000;
  projectile->facing = effectWork->angle;
  projectile->speedLimit = 4 << 16;
  projectile->acceleration = 4 << 16;
  projectile->motionFlags = 0;
  Actor_SelectAnimationFar(projectile, 5);
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
    copy = copies[i] = Actor_Spawn(SPRITE_MOVE_HAND, projectile->x.fixed,
                                   projectile->y.fixed, projectile->z.fixed);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = effectWork->angle;
      copy->speedLimit = 4 << 16;
      copy->acceleration = 4 << 16;
      copy->motionFlags = 0;
      Actor_SelectAnimationFar(copy, 5);
      Anim_ApplyChildValuesFar(copy, SPRITE_COLOR_RBG);
      sharedAnimation = Sprite_ShareVramCacheEntry(
          (struct Sprite *)copy->sprite, sharedAnimation);
    }
  }
  resourceSlot = sharedAnimation->slot;

  // Destination: 56 pixels in front of the caster with effectWork->enabled set
  // (no trigger named the target), otherwise the effect's position.
  // The shot's script waits to arrive and makes the impact
  // (gFieldEffect_BurstParticleActorScript + 4 is
  // gFieldEffect_HeavyImpactScript); each afterimage follows 3 frames apart and
  // is freed on arrival.
  if (effectWork->enabled != 0) {
    caster = effectWork->mainActor;
    pos.x = caster->x.fixed;
    pos.y = caster->y.fixed + ABOVE;
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(56 << 16, effectWork->angle, &pos);
  } else {
    pos.x = effectWork->x;
    pos.y = effectWork->y + ABOVE;
    pos.z = effectWork->z;
  }
  Actor_SetMoveTargetFar(projectile, pos.x, pos.y, pos.z);
  Actor_StartScriptFar(projectile, gFieldEffect_BurstParticleActorScript + 4);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Actor_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      Actor_StartScriptFar(copy, gFieldEffect_CommonParticleScript + 1);
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
        ActorScript_CheckOverlapFar(target, (s32 *)&pos) == 0) {
      target->acceleration = 1 << 16;
      target->speedLimit = 1 << 16;
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  // Run the map's event for it, if there is one.
  event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                    FIELD_PSYNERGY_FORCE, &eventTarget);
  if (event != NULL)
    Field_RunPsynergyEvent(event, gSaveBlock.leaderActorId, eventTarget);
  WaitFrames(10);
  FieldPsynergy_FadeFromCastColor();
  WaitFrames(20);
  // 96 (0x60), the VRAM cache's entry count, means no entry to free.
  if (resourceSlot != 96)
    VramCache_FreeEntry(resourceSlot);
}
