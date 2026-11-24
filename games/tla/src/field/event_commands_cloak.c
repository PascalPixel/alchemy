/// @file
/// Cloak, Douse, falling particles and steering a lifted target.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/field_psynergy_state.h"
#include "system/main_routine_state.h"
#include "field/actor.h"
#include "field/sprite.h"
#include "system/task.h"
#include "field/event_commands.h"
#include "system/system.h"
#include "field/object.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/objdisp.h"
#include "battle/object_effect_scripts.h"
#include "field/field_sequence_event.h"
#include "field/actor_view.h"
#include "system/input.h"
#include "battle/battle_effect.h"
#include "system/frame.h"

/// The Psynergy request that ends Cloak, without paying PP: what the ground
/// check (FieldEffect_CheckCloakTile) makes when the caster does not stand on a
/// tile it can hide on.
#define CLOAK_END (ABILITY_CLOAK | ACT_PACKED)
/// The times the caster blinks as Cloak hides it.
#define CLOAK_BLINKS 10

/// The drops Douse rains: cast at nothing, cast on a target, and cast on a
/// target where they sway down (FieldEffectState.lit).
#define DOUSE_DROPS_AT_NOTHING 10
#define DOUSE_DROPS_ON_TARGET 24
#define DOUSE_SWAYING_DROPS_ON_TARGET 30
/// The drop after which the target's Douse trigger runs.
#define DOUSE_TARGET_DROP 12
/// The sparkles Carry throws out as it drops its target.
#define CARRY_SHARDS 20

/// Cloak the caster: flicker it CLOAK_BLINKS times, leave it half transparent,
/// then check the ground under it every frame. When it cannot hide where it
/// stands, show it again at once.
void FieldPsynergy_StartCloak(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct Actor *caster =
      ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->caster;
  struct Sprite *sprite = caster->sprite;
  struct SpriteAnimation *entry = sprite->entries[0];
  u32 n;
  void (*check)(void);
  s32 order;

  Sound_PlayFar(SE_PSYNERGY_START);
  Actor_SelectAnimationFar(caster, ANIM_STILL);
  caster->update = 0;

  for (n = 0; n < CLOAK_BLINKS; n++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    sprite->dirty = 1;
    sprite->flags = 0;
    wait(2);
  }

  order = TASK_PASS_FRAME;
  sprite->flags = SPRITE_FLAG_OUTLINED;
  entry->colorMode = SPRITE_COLOR_NORMAL;
  sprite->dirty = 1;
  check = FieldEffect_CheckCloakTile;
  Task_Add(check, order);
  gMainRoutineState.cloaked = 1;
  check();
  if (event->psynergy == CLOAK_END) {
    FieldPsynergy_EndCloak();
    event->psynergy = 0;
  }
}

/// A Douse drop's per-frame update: once it reaches its ground it starts the
/// shared particle script and stops updating.
void FieldEffect_LandDouseDrop(struct Actor *actor) {
  if (actor->y.fixed <= actor->groundHeight) {
    actor->wait = 2;
    Actor_StartScriptFar(actor, (u32)gFieldEffect_ReleaseScript);
    actor->update = 0;
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
void FieldEffect_UpdateDriftingFallActor(struct Actor *particle) {
  s32 drift;

  particle->y.fixed -= toFIXED(3) / 10;
  drift = Random16();
  particle->x.fixed += drift - Random16();
  if (particle->y.fixed <= particle->groundHeight) {
    Actor_StartScriptFar(particle, gFieldEffect_ReleaseScript);
  }
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
void FieldPsynergy_DouseOnTarget(s32 unused) { FieldPsynergy_Douse(); }

/// A rain cloud rises from 16 pixels above the caster to 32 pixels above the
/// destination (80 with childOption, 32 more over a Moai) over GROW_STEPS
/// frames, growing as it goes, rains drops, and returns.
void FieldPsynergy_Douse(void) {
  struct Vector3 pos;
  struct Vector3 start;
  struct Vector3 end;
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;
  struct Actor *target = state->target;
  struct Actor *caster = state->caster;
  s32 count;
  struct Actor *cloud;
  s32 raised;
  s32 i;
  s32 steps = GROW_STEPS + 1;
  s32 y;
  s32 n;
  struct Actor *particle;
  struct PendingEvent *trigger;

  cloud = Actor_Spawn(SPRITE_RAIN_CLOUD, i = raised = 0, 0, 0);
  if (cloud == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  if (state->target == 0) {
    state->x = caster->x.fixed;
    state->z = caster->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), state->angle,
                          (struct Vector3 *)&state->x);
  }
  SET_POSITION(&start, caster->x.fixed, caster->y.fixed + toFIXED(16),
               caster->z.fixed);
  end.x = state->x;
  y = state->y;
  end.y = y + toFIXED(32);
  end.z = state->z;
  if ((s8)state->sceneVariant != 0)
    end.y = y + toFIXED(80);
  if (Actor_GetSpriteIdFar(target) == SPRITE_MOAI) {
    end.y += toFIXED(32);
    raised = 1;
  }

  for (; i < steps; i++) {
    s32 scale;

    SET_ACTOR_POSITION(cloud, start.x + (end.x - start.x) * i / GROW_STEPS,
                       start.y + (end.y - start.y) * i / GROW_STEPS,
                       start.z + (end.z - start.z) * i / GROW_STEPS);
    scale = FieldEffect_Interpolate(FIXED_ONE, FIXED_ONE / 4, i);
    cloud->scaleX = scale;
    cloud->scaleY = scale;
    wait(1);
  }
  wait(10);

  if ((s8)state->lit == 0) {
    count = DOUSE_DROPS_AT_NOTHING;
    if ((s8)state->aimAhead == 0)
      count = DOUSE_DROPS_ON_TARGET;
    for (n = 0; n < count; n++) {
      SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
      Vector_AddPolarOffset(Random16() * 5 + toFIXED(3), Random16(), &pos);
      if (n == count - 1) {
        wait(20);
        SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
      }
      particle = Actor_Spawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
      if (particle != 0) {
        struct Actor *ground = state->target;

        if (ground != 0) {
          particle->groundHeight = ground->y.fixed;
          if (raised == 1)
            particle->groundHeight = ground->y.fixed + toFIXED(32);
        } else {
          particle->groundHeight = pos.y - toFIXED(32);
        }
        particle->update = (ObjectUpdate)FieldEffect_LandDouseDrop;
        particle->motionFlags = ACTOR_FALLS;
        particle->priorityFlags =
            ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_OVERHEAD;
      }
      Sound_PlayFar(SE_WATER);
      wait(6);
      if (n == DOUSE_TARGET_DROP) {
        trigger = Scene_FindPsynergyTrigger(
            PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_DOUSE,
            Scene_SelectPsynergyTarget(FIELD_PSYNERGY_DOUSE));
        if (trigger != 0)
          Field_RunPsynergyEvent(trigger, state->casterId, state->targetId);
      }
    }
    wait(10);
  } else {
    if ((s8)state->aimAhead != 0)
      count = DOUSE_DROPS_AT_NOTHING;
    else
      count = DOUSE_SWAYING_DROPS_ON_TARGET;
    for (n = 0; n < count; n++) {
      SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
      Vector_AddPolarOffset(Random16() * 5 + toFIXED(3), Random16(), &pos);
      particle = Actor_Spawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = (ObjectUpdate)FieldEffect_UpdateDriftingFallActor;
        particle->motionFlags = 0;
        ((struct Sprite *)particle->sprite)->image.priority = 2;
        Actor_SelectAnimationFar(particle, 8);
        Actor_SetSpriteColorModeFar((struct Actor *)particle,
                                    SPRITE_COLOR_WHITE);
      }
      wait(6);
    }
    wait(70);
  }

  i = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(cloud, end.x + (start.x - end.x) * i / GROW_STEPS,
                       end.y + (start.y - end.y) * i / GROW_STEPS,
                       end.z + (start.z - end.z) * i / GROW_STEPS);
    scale = FieldEffect_Interpolate(FIXED_ONE / 4, FIXED_ONE, i);
    cloud->scaleX = scale;
    cloud->scaleY = scale;
    wait(1);
    i++;
  } while (i < steps);
  Actor_ReleaseFar((struct Actor *)cloud);
  FieldPsynergy_FadeFromCastColor();
}

/// Task while Carry holds its target: drops one particle a frame round the
/// lifted actor, 8 to 24 pixels above it and 0 to 48 pixels away in a random
/// direction (FieldEffect_InitializeFallingFragment).
void FieldEffect_SpawnFallingParticles(void) {
  struct Actor *liftedActor;
  struct Vector3 position;
  struct Actor *fragment;
  u32 radius;

  liftedActor =
      ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->target;
  position.x = liftedActor->x.fixed;
  position.y = liftedActor->y.fixed - Random16() * 16 + toFIXED(24);
  position.z = liftedActor->z.fixed;

  radius = Random16() * 3;
  Vector_AddPolarOffset(radius * 16, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

void FieldEffect_SpawnRadialParticleRing(struct Actor *actor);

/// The collision at position for the actor standing 16 pixels higher.
static __inline__ s32 Actor_ProbeRaised(struct Actor *actor,
                                        struct Vector3 *position) {
  s32 hit;

  actor->groundHeight += toFIXED(16);
  hit = MapGrid_CheckCollisionFar(actor, position);
  actor->groundHeight -= toFIXED(16);
  return hit;
}

/// When Carry is cast on a target, two hands lift it to 24 pixels above the
/// ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it
/// where it is.
void FieldPsynergy_SteerLiftedTarget(s32 targetId) {
  struct FieldEffectState *state;
  struct Actor *source;
  struct Actor *target;
  struct Actor *shards[CARRY_SHARDS];
  struct Vector3 position;
  struct Actor *anchors[2];
  struct Actor *actor;
  struct Actor *right;
  s32 i;
  s32 angle;
  s32 magnitude;

  state = Ram_HeapSlots->fieldEffectState;
  source = state->caster;
  target = state->target;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  source->watchedActor = target;
  Actor_StartScriptFar(source, (u32)gFieldEffect_SourceHoldScript);
  SET_POSITION(&position, state->x, state->y + toFIXED(16), state->z);
  actor = FieldEffect_SpawnCarryHand(position.x + toFIXED(32), position.y,
                                     position.z, DEGtoANG(180));
  anchors[0] = actor;
  right = FieldEffect_SpawnCarryHand(position.x - toFIXED(32), position.y,
                                     position.z, 0);
  anchors[1] = right;
  if (anchors[0] == NULL || right == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  wait(15);
  SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  Actor_SetMoveTargetFar(actor, position.x + toFIXED(16), position.y,
                         position.z);
  Actor_SetMoveTargetFar(right, position.x - toFIXED(16), position.y,
                         position.z);
  Actor_WaitForMoveFar(actor);
  Actor_WaitForMoveFar(right);
  actor->x.fixed = position.x + toFIXED(16);
  actor->velocityX = 0;
  right->x.fixed = position.x - toFIXED(16);
  right->velocityX = 0;
  target->update = Actor_CycleSpriteColors;
  Task_Add((s32)FieldEffect_SpawnFallingParticles, TASK_PASS_FRAME);
  Sound_PlayFar(SE_PSYNERGY_START);
  target->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlagsFar(target, 0);
  if (anchors[0] != NULL && anchors[1] != NULL) {
    while (target->y.fixed - target->groundHeight <= toFIXED(24)) {
      actor->y.fixed += toFIXED(3) / 8;
      right->y.fixed += toFIXED(3) / 8;
      target->y.fixed += toFIXED(3) / 8;
      wait(1);
    }
  }
  anchors[0]->speedLimit = toFIXED(4);
  anchors[0]->acceleration = toFIXED(1) / 2;
  anchors[1]->speedLimit = toFIXED(4);
  anchors[1]->acceleration = toFIXED(1) / 2;
  target->speedLimit = toFIXED(2) / 5;
  target->acceleration = toFIXED(1) / 5;
  target->steeringFlags = 0;
  target->layer = MAPGRID_LAYER_OVERHEAD;
  for (;;) {
    wait(1);
    if ((gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;
    angle = (u16)Input_GetDpadAngle(gInput.held);
    if (angle == DPAD_ANGLE_NONE) {
      SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
                   target->z.fixed);
      Actor_SetMoveTargetFar(anchors[0], position.x + toFIXED(16), position.y,
                             position.z);
      Actor_SetMoveTargetFar(anchors[1], position.x - toFIXED(16), position.y,
                             position.z);
      Actor_SelectAnimationFar(anchors[0], 1);
      Actor_SelectAnimationFar(anchors[1], 1);
      continue;
    }
    SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
                 target->z.fixed);
    Vector_AddPolarOffset(toFIXED(2), angle, &position);
    Actor_SetMoveTargetFar(anchors[0], position.x + toFIXED(16), position.y,
                           position.z);
    Actor_SetMoveTargetFar(anchors[1], position.x - toFIXED(16), position.y,
                           position.z);
    Actor_WaitForMoveFar(anchors[0]);
    Actor_WaitForMoveFar(anchors[1]);
    SET_POSITION(&position, target->x.fixed, target->groundHeight,
                 target->z.fixed);
    Vector_AddPolarOffset(toFIXED(16), angle, &position);
    i = ActorScript_CheckOverlapFar(target, &position);
    // Blocked by an actor or by the ground: the hands strain and a buzz
    // sounds.
    if (i != 0)
      goto blocked;
    if (Actor_ProbeRaised(target, &position) > 0) {
    blocked:
      Actor_SelectAnimationFar(anchors[0], 4);
      Actor_SelectAnimationFar(anchors[1], 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    {
      struct Actor *second;
      FIXED x;
      FIXED z;

      Sound_PlayFar(SE_NOTE_HIGH);
      x = position.x;
      z = position.z;
      Actor_SelectAnimationFar(anchors[0], 4);
      Actor_SelectAnimationFar(anchors[1], 4);
      wait(15);
      target->paused = 0;
      target->speedLimit = toFIXED(1) / 5;
      target->acceleration = toFIXED(1) / 5;
      Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
      anchors[0]->speedLimit = toFIXED(1) / 5;
      anchors[0]->acceleration = toFIXED(1) / 5;
      second = anchors[1];
      second->speedLimit = toFIXED(1) / 5;
      second->acceleration = toFIXED(1) / 5;
      Actor_SetMoveTargetFar(anchors[0], position.x + toFIXED(16), position.y,
                             position.z);
      Actor_SetMoveTargetFar(second, position.x - toFIXED(16), position.y,
                             position.z);
      Actor_WaitForMoveFar(target);
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = 0;
      target->velocityZ = 0;
      wait(10);
      break;
    }
  }
  Actor_SelectAnimationFar(anchors[0], 4);
  Actor_SelectAnimationFar(anchors[1], 4);
  Task_Remove((u32)FieldEffect_SpawnFallingParticles);
  Sound_PlayFar(SE_CLICK);
  wait(15);
  Sound_PlayFar(SE_CLICK);
  wait(15);
  SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  for (i = 0; i < CARRY_SHARDS; i++) {
    actor = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
    // DEAD: nothing reads the shards back; the ROM keeps the stores.
    shards[i] = actor;
    if (actor != NULL) {
      Actor_StartScriptFar(actor, (u32)gFieldEffect_FragmentScript);
      actor->speedLimit = Random16() + toFIXED(2);
      actor->acceleration = toFIXED(2);
      actor->motionFlags = 0;
      magnitude = Random16() * 24 + toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(actor, magnitude, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(anchors[0]);
  Actor_ReleaseFar(anchors[1]);
  Actor_SetSpriteColorModeFar(target, state->savedPalette);
  Actor_StartScriptFar(target, (u32)(state->savedScript));
  target->update = state->savedCallback;
  target->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  target->velocityY = toFIXED(10);
  target->bounce = toFIXED(1) / 5;
  target->layer = MAPGRID_LAYER_GROUND;
  source->update = NULL;
  Actor_SetSpriteColorModeFar(source, SPRITE_COLOR_NORMAL);
  if ((s8)state->sceneVariant != 0) {
    for (i = 0; i < 90 && target->velocityY >= 0; i++)
      wait(1);
    wait(1);
    for (i = 0; i < 90 && target->velocityY < 0; i++)
      wait(1);
    FieldEffect_SpawnRadialParticleRing(target);
    FieldPsynergy_FadeFromCastColor();
    wait(30);
  } else {
    FieldPsynergy_FadeFromCastColor();
  }
}

#undef CLOAK_END
#undef CLOAK_BLINKS
#undef DOUSE_DROPS_AT_NOTHING
#undef DOUSE_DROPS_ON_TARGET
#undef DOUSE_SWAYING_DROPS_ON_TARGET
#undef DOUSE_TARGET_DROP
#undef CARRY_SHARDS
