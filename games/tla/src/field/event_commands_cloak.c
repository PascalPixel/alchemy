/// @file
/// Cloak, Douse, falling particles and steering a lifted target.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/field_psynergy_work.h"
#include "system/main_routine_work.h"
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
#include "graphics/graphics.h"
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

/// The end of Cloak, and Douse.

// What the ground check (FieldEffect_CheckCloakTile) requests when the caster
// does not stand on a tile it can hide on.
#define CLOAK_END 0x2092

/// Cloak the caster: flicker it ten times, leave it half transparent, then
/// check the ground under it every frame. When it cannot hide where it
/// stands, show it again at once.
void FieldPsynergy_StartCloak(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *caster =
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  struct Sprite *sprite = caster->sprite;
  struct SpriteAnimation *entry = sprite->entries[0];
  u32 n;
  void (*check)(void);
  s32 order;

  Sound_PlayFar(SE_PSYNERGY_START);
  Actor_SelectAnimationFar(caster, ANIM_STILL);
  caster->update = 0;

  for (n = 0; n < 10; n++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    sprite->dirty = 1;
    sprite->flags = 0;
    wait(2);
  }

  order = TASK_PASS_VBLANK;
  sprite->flags = SPRITE_FLAG_OUTLINED;
  entry->colorMode = SPRITE_COLOR_NORMAL;
  sprite->dirty = 1;
  check = FieldEffect_CheckCloakTile;
  Task_Add(check, order);
  gMainRoutineWork.cloaked = 1;
  check();
  if (event->psynergy == CLOAK_END) {
    FieldPsynergy_EndCloak();
    event->psynergy = 0;
  }
}

/// Per-frame update of a falling mote: once it reaches its floor it starts the
/// shared particle script and stops updating.
void BattleEffect_LandFallingObject(struct Actor *object) {
  if (object->y.fixed <= object->groundHeight) {
    object->wait = 2;
    Actor_StartScriptFar(object, (u32)gBattleEffect_SharedParticleScript);
    object->update = 0;
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
    Actor_StartScriptFar(particle, gBattleEffect_SharedParticleScript);
  }
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
void FieldPsynergy_DouseOnTarget(s32 unused) { FieldPsynergy_Douse(); }

/// The value step tenths of the way from start to end.
static __inline__ s32 InterpolateTenths(s32 start, s32 end, s32 step) {
  return start + step * (end - start) / 10;
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
void FieldPsynergy_Douse(void) {
  struct Vector3 pos;
  struct Vector3 start;
  struct Vector3 end;
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *anchor = work->child;
  struct Actor *target = work->mainActor;
  s32 count;
  struct Actor *orb;
  s32 raised;
  struct Vector3 *from;
  struct Vector3 *to;
  s32 i;
  s32 steps = 11;
  s32 y;
  s32 n;
  struct Actor *mote;
  struct PendingEvent *ev;

  orb = Actor_Spawn(SPRITE_RAIN_CLOUD, i = raised = 0, 0, 0);
  if (orb == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  if (work->child == 0) {
    work->x = target->x.fixed;
    work->z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), work->angle, (struct Vector3 *)&work->x);
  }
  from = &start;
  SET_POSITION(from, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  to = &end;
  to->x = work->x;
  y = work->y;
  to->y = y + toFIXED(32);
  to->z = work->z;
  if ((s8)work->childOption != 0)
    to->y = y + toFIXED(80);
  if (Actor_GetSpriteIdFar(anchor) == SPRITE_MOAI) {
    to->y += toFIXED(32);
    raised = 1;
  }

  for (; i < steps; i++) {
    s32 scale;

    {
      struct Vector3 *a = &start;
      struct Vector3 *b = &end;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += (e - t) * i / 10;
      orb->x.fixed = t;
      e = b->y;
      t = a->y;
      t += (e - t) * i / 10;
      orb->y.fixed = t;
      e = b->z;
      t = a->z;
      t += (e - t) * i / 10;
      orb->z.fixed = t;
    }
    scale = InterpolateTenths(FIXED_ONE / 4, FIXED_ONE, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
  }
  wait(10);

  if ((s8)work->lit == 0) {
    count = 10;
    if ((s8)work->enabled == 0)
      count = 24;
    for (n = 0; n < count; n++) {
      SET_POSITION(&pos, orb->x.fixed, orb->y.fixed, orb->z.fixed);
      Vector_AddPolarOffset(Random16() * 5 + toFIXED(3), Random16(), &pos);
      if (n == count - 1) {
        wait(20);
        SET_POSITION(&pos, orb->x.fixed, orb->y.fixed, orb->z.fixed);
      }
      mote = Actor_Spawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
      if (mote != 0) {
        struct Actor *ground = work->child;

        if (ground != 0) {
          mote->groundHeight = ground->y.fixed;
          if (raised == 1)
            mote->groundHeight = ground->y.fixed + toFIXED(32);
        } else {
          mote->groundHeight = pos.y - toFIXED(32);
        }
        mote->update = (void (*)(void))BattleEffect_LandFallingObject;
        mote->motionFlags = ACTOR_FALLS;
        mote->priorityFlags =
            ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_OVERHEAD;
      }
      Sound_PlayFar(SE_WATER);
      wait(6);
      if (n == 12) {
        ev = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), 5,
                                       Scene_SelectPsynergyTarget(5));
        if (ev != 0)
          Field_RunPsynergyEvent(ev, work->firstActorId, work->secondActorId);
      }
    }
    wait(10);
  } else {
    if ((s8)work->enabled != 0)
      count = 10;
    else
      count = 30;
    for (n = 0; n < count; n++) {
      SET_POSITION(&pos, orb->x.fixed, orb->y.fixed, orb->z.fixed);
      Vector_AddPolarOffset(Random16() * 5 + toFIXED(3), Random16(), &pos);
      mote = Actor_Spawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
      if (mote != 0) {
        mote->update = (void (*)(void))FieldEffect_UpdateDriftingFallActor;
        mote->motionFlags = 0;
        ((struct Sprite *)mote->sprite)->image.priority = 2;
        Actor_SelectAnimationFar(mote, 8);
        Actor_SetSpriteColorModeFar((struct Actor *)mote, SPRITE_COLOR_WHITE);
      }
      wait(6);
    }
    wait(70);
  }

  i = 0;
  do {
    s32 scale;

    {
      struct Vector3 *a = &end;
      struct Vector3 *b = &start;
      s32 e;
      FIXED t;

      e = b->x;
      t = a->x;
      t += (e - t) * i / 10;
      orb->x.fixed = t;
      e = b->y;
      t = a->y;
      t += (e - t) * i / 10;
      orb->y.fixed = t;
      e = b->z;
      t = a->z;
      t += (e - t) * i / 10;
      orb->z.fixed = t;
    }
    scale = InterpolateTenths(FIXED_ONE, FIXED_ONE / 4, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    i++;
  } while (i < steps);
  Actor_ReleaseFar((struct Actor *)orb);
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
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->child;
  position.x = liftedActor->x.fixed;
  position.y = liftedActor->y.fixed - Random16() * 16 + toFIXED(24);
  position.z = liftedActor->z.fixed;

  radius = Random16() * 3;
  Vector_AddPolarOffset(radius * 16, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

void FieldEffect_SpawnRadialParticleRing(struct Actor *object);

/// The collision at position for the actor standing 16 pixels higher.
static __inline__ s32 Actor_ProbeRaised(struct Actor *object,
                                        struct Vector3 *position) {
  s32 hit;

  object->groundHeight += toFIXED(16);
  hit = MapGrid_CheckCollisionFar(object, position);
  object->groundHeight -= toFIXED(16);
  return hit;
}

/// When Carry is cast on a target, two hands lift it to 24 pixels above the
/// ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it
/// where it is.
void FieldPsynergy_SteerLiftedTarget(s32 targetId) {
  struct FieldEffectWork *work;
  struct Actor *source;
  struct Actor *target;
  struct Actor *shards[20];
  struct Vector3 position;
  struct Actor *anchors[2];
  struct Actor *object;
  struct Actor *right;
  s32 i;
  s32 angle;
  s32 magnitude;

  work = Ram_HeapSlots->fieldEffectWork;
  source = work->mainActor;
  target = work->child;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  source->watchedActor = target;
  Actor_StartScriptFar(source, (u32)gFieldEffect_SourceHoldScript);
  SET_POSITION(&position, work->x, work->y + toFIXED(16), work->z);
  object = FieldEffect_SpawnCarryHand(position.x + toFIXED(32), position.y,
                                      position.z, DEGtoANG(180));
  anchors[0] = object;
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
  Actor_SetMoveTargetFar(object, position.x + toFIXED(16), position.y,
                         position.z);
  Actor_SetMoveTargetFar(right, position.x - toFIXED(16), position.y,
                         position.z);
  Actor_WaitForMoveFar(object);
  Actor_WaitForMoveFar(right);
  object->x.fixed = position.x + toFIXED(16);
  object->velocityX = 0;
  right->x.fixed = position.x - toFIXED(16);
  right->velocityX = 0;
  target->update = Actor_CycleSpriteColors;
  Task_Add((s32)FieldEffect_SpawnFallingParticles, TASK_PASS_VBLANK);
  Sound_PlayFar(SE_PSYNERGY_START);
  target->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlagsFar(target, 0);
  if (anchors[0] != NULL && anchors[1] != NULL) {
    while (target->y.fixed - target->groundHeight <= toFIXED(24)) {
      object->y.fixed += toFIXED(3) / 8;
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
  target->layer = 2;
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
    // Both tests jump into the blocked path, which keeps it ahead of the drop
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
      target->paused = i;
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
      target->velocityX = i;
      target->velocityZ = i;
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
  for (i = 0; i < 20; i++) {
    object = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
    // DEAD: nothing reads the shards back; the ROM keeps the stores.
    shards[i] = object;
    if (object != NULL) {
      Actor_StartScriptFar(object, (u32)gFieldEffect_FragmentScript);
      object->speedLimit = Random16() + toFIXED(2);
      object->acceleration = toFIXED(2);
      object->motionFlags = 0;
      magnitude = Random16() * 24 + toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(object, magnitude, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(anchors[0]);
  Actor_ReleaseFar(anchors[1]);
  Actor_SetSpriteColorModeFar(target, work->savedPalette);
  Actor_StartScriptFar(target, (u32)(work->savedScript));
  target->update = work->savedCallback;
  target->motionFlags = 3;
  target->velocityY = toFIXED(10);
  target->bounce = toFIXED(1) / 5;
  target->layer = 0;
  source->update = NULL;
  Actor_SetSpriteColorModeFar(source, SPRITE_COLOR_NORMAL);
  if ((s8)work->childOption != 0) {
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
