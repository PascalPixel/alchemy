/// @file
/// Force: a fist flies out from the caster and knocks what it hits back a
/// cell.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Runs Force cast on a target, through FieldPsynergy_Force.
void FieldPsynergy_ForceOnTarget(s32 unused) { FieldPsynergy_Force(); }

/// The flashes that burst from over the caster's head, one every 2 frames.
#define FORCE_FLASHES 12

/// Writes the position 16 pixels above the actor.
static __inline__ void FieldEffect_GetPositionAboveActor(struct Actor *actor,
                                                         struct Vector3 *pos) {
  SET_POSITION(pos, actor->x.fixed, actor->y.fixed + PSYNERGY_EFFECT_HEIGHT,
               actor->z.fixed);
}

/// Flashes burst from over the caster's head, then a fist (SPRITE_MOVE_HAND)
/// appears in front of it, grows and flies forward with three afterimages;
/// what it hits (effectState->target) is pushed 16 pixels.
void FieldPsynergy_Force(void) {
  struct Actor *target;
  s32 eventTarget;
  struct Actor *copies[4];
  struct Vector3 pos;
  struct Actor *projectile;
  struct Actor *copy;
  struct Actor *caster;
  struct FieldEffectState *effectState;
  struct FieldParticle *effect;
  struct Sprite *sharedSprite;
  void *event;
  s32 scale;
  s32 i;
  u8 resourceSlot;

  effectState = gFieldEffectState;
  target = effectState->target;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_PSYNERGY_START);
  effect = effectState->particles;

  for (i = 0; i < FORCE_FLASHES; i++) {
    caster = effectState->caster;
    FieldEffect_GetPositionAboveActor(caster, &pos);
    Camera_WorldToScreen(&pos);
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRadialBurst);
    FieldParticle_SetAnimation(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, SPRITE_COLOR_BLUE_TINT);
    effect->scaleY = FIXED_ONE * 7 / 10;
    effect->scaleX = FIXED_ONE * 7 / 10;
    wait(2);
    effect++;
  }

  // The fist: 8 pixels in front of the caster, growing from 0.25 to full
  // size by 5/256 a frame (39 frames).
  caster = effectState->caster;
  pos.x = caster->x.fixed;
  pos.y = caster->y.fixed + PSYNERGY_EFFECT_HEIGHT;
  pos.z = caster->z.fixed;
  Vector_AddPolarOffset(toFIXED(8), effectState->angle, &pos);
  projectile = Actor_Spawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
  if (projectile == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  projectile->scaleY = FIXED_ONE / 4;
  projectile->scaleX = FIXED_ONE / 4;
  projectile->facing = effectState->angle;
  projectile->speedLimit = toFIXED(4);
  projectile->acceleration = toFIXED(4);
  projectile->motionFlags = 0;
  Actor_SelectAnimationFar(projectile, MOVE_HAND_ANIMATION_FIST);
  Actor_SetSpriteColorModeFar(projectile, SPRITE_COLOR_BGR);
  scale = projectile->scaleX;
  if (scale < FIXED_ONE) {
    do {
      scale += FIXED_ONE * 5 / 256;
      projectile->scaleY = scale;
      projectile->scaleX = scale;
      wait(1);
      scale = projectile->scaleX;
    } while (scale < FIXED_ONE);
  }
  wait(3);

  // Three afterimages, sharing one set of graphics.
  sharedSprite = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] = Actor_Spawn(SPRITE_MOVE_HAND, projectile->x.fixed,
                                   projectile->y.fixed, projectile->z.fixed);
    if (copy != NULL) {
      copy->scaleY = AFTERIMAGE_SCALE;
      copy->scaleX = AFTERIMAGE_SCALE;
      copy->facing = effectState->angle;
      copy->speedLimit = toFIXED(4);
      copy->acceleration = toFIXED(4);
      copy->motionFlags = 0;
      Actor_SelectAnimationFar(copy, MOVE_HAND_ANIMATION_FIST);
      Actor_SetSpriteColorModeFar(copy, SPRITE_COLOR_RBG);
      sharedSprite = Sprite_ShareVramCacheEntry((struct Sprite *)copy->sprite,
                                                sharedSprite);
    }
  }
  resourceSlot = sharedSprite->slot;

  // Destination: 56 pixels in front of the caster with effectState->aimAhead
  // set (no trigger named the target), otherwise the Psynergy's position.
  if (effectState->aimAhead != 0) {
    caster = effectState->caster;
    pos.x = caster->x.fixed;
    pos.y = caster->y.fixed + PSYNERGY_EFFECT_HEIGHT;
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(toFIXED(56), effectState->angle, &pos);
  } else {
    pos.x = effectState->x;
    pos.y = effectState->y + PSYNERGY_EFFECT_HEIGHT;
    pos.z = effectState->z;
  }
  Actor_SetMoveTargetFar(projectile, pos.x, pos.y, pos.z);
  Actor_StartScriptFar(projectile, gForce_FistScript);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      wait(3);
      Actor_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      // From the shared script's SC_SYNC: released once it arrives.
      Actor_StartScriptFar(copy, gFieldEffect_SharedParticleScript + 1);
    }
  }

  // Wait, at most 60 frames, for the fist's script to end.
  for (i = 0; i < 60 && projectile->script != NULL; i++)
    wait(1);

  // Push what was hit 16 pixels, at 1 pixel a frame, if nothing is in the way:
  // the ground there must be within a step (MapGrid_CheckCollision 0) and free
  // of actors.
  if (target != NULL && effectState->leaveTarget == 0) {
    if (effectState->sceneVariant != 0)
      target->velocityY = toFIXED(8);
    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), effectState->angle, &pos);
    if (MapGrid_CheckCollisionFar(target, (struct FieldPosition *)&pos) ==
            COLLISION_NONE &&
        ActorScript_CheckOverlapFar(target, (s32 *)&pos) == 0) {
      target->acceleration = toFIXED(1);
      target->speedLimit = toFIXED(1);
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                    FIELD_PSYNERGY_FORCE, &eventTarget);
  if (event != NULL)
    Field_RunPsynergyEvent(event, gMainRoutineState.leaderActorId, eventTarget);
  wait(10);
  FieldPsynergy_FadeFromCastColor();
  wait(20);
  // VRAM_CACHE_ENTRY_COUNT means no entry to free.
  if (resourceSlot != VRAM_CACHE_ENTRY_COUNT)
    VramCache_FreeEntry(resourceSlot);
}

#undef FORCE_FLASHES
