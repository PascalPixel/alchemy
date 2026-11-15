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
#include "field/field_bank.h"
#include "system/system_bank.h"
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

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

// What the ground check (FieldEffect_CheckCloakTile) requests when the caster
// does not stand on a tile it can hide on.
#define CLOAK_END 0x2092

/// Cloak the caster: flicker it ten times, leave it half transparent, then
/// check the ground under it every frame. When it cannot hide where it
/// stands, show it again at once.
void FieldPsynergy_StartCloak(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct CloakCaster *caster =
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  struct CloakSprite *sprite = caster->sprite;
  struct SpriteAnimation *entry = sprite->entries[0];
  u32 n;
  void (*check)(void);
  s32 order;

  Sound_PlayFar(SE_PSYNERGY_START);
  Actor_SelectAnimationFar((struct Actor *)caster, 0);
  caster->hook = 0;

  for (n = 0; n < 10; n++) {
    entry->colorMode = CLOAK_FLICKER_PALETTE;
    sprite->dirty = 1;
    sprite->blend = 2;
    wait(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    wait(2);
  }

  order = TASK_PASS_VBLANK;
  sprite->blend = 2;
  entry->colorMode = 0;
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
void BattleEffect_LandFallingObject(struct FallingObject *object) {
  if (object->y <= object->floor) {
    object->dispatchState = 2;
    Actor_StartScriptFar(object, (u32)gBattleEffect_SharedParticleScript);
    object->update = 0;
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
void FieldEffect_UpdateDriftingFallActor(struct Actor *object) {
  s32 r;

  object->y.fixed -= px(3) / 10;
  r = Random16();
  object->x.fixed += r - Random16();
  if (object->y.fixed <= object->groundHeight) {
    Actor_StartScriptFar(object, (u32)gBattleEffect_SharedParticleScript);
  }
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
void FieldPsynergy_DouseOnTarget(s32 unused) { FieldPsynergy_Douse(); }

/// The value step tenths of the way from start to end.
static __inline__ s32 Lerp10(s32 start, s32 end, s32 step) {
  return start + step * (end - start) / 10;
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
void FieldPsynergy_Douse(void) {
  struct EffectVector pos;
  struct EffectVector start;
  struct EffectVector end;
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *anchor = work->child;
  struct Actor *target = work->mainActor;
  s32 count;
  struct Actor *orb;
  s32 raised;
  struct EffectVector *from;
  struct EffectVector *to;
  s32 i;
  s32 steps = 11;
  s32 y;
  s32 n;
  struct Actor *mote;
  struct PendingEvent *ev;

  orb = Actor_Spawn(300, i = raised = 0, 0, 0);
  if (orb == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  if (work->child == 0) {
    work->x = target->x.fixed;
    work->z = target->z.fixed;
    Vector_AddPolarOffset(px(16), work->angle, (struct EffectVector *)&work->x);
  }
  from = &start;
  SET_POSITION(from, target->x.fixed, target->y.fixed + px(16),
               target->z.fixed);
  to = &end;
  to->x = work->x;
  y = work->y;
  to->y = y + px(32);
  to->z = work->z;
  if ((s8)work->childOption != 0)
    to->y = y + px(80);
  if (Actor_GetSpriteIdFar(anchor) == 380) {
    to->y += px(32);
    raised = 1;
  }

  for (; i < steps; i++) {
    s32 scale;

    {
      struct EffectVector *a = &start;
      struct EffectVector *b = &end;
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
    scale = Lerp10(Q16_ONE / 4, Q16_ONE, i);
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
      Vector_AddPolarOffset(Random16() * 5 + px(3), Random16(), &pos);
      if (n == count - 1) {
        wait(20);
        SET_POSITION(&pos, orb->x.fixed, orb->y.fixed, orb->z.fixed);
      }
      mote = Actor_Spawn(301, pos.x, pos.y, pos.z);
      if (mote != 0) {
        struct Actor *ground = work->child;

        if (ground != 0) {
          mote->groundHeight = ground->y.fixed;
          if (raised == 1)
            mote->groundHeight = ground->y.fixed + px(32);
        } else {
          mote->groundHeight = pos.y - px(32);
        }
        mote->update = (void (*)(void))BattleEffect_LandFallingObject;
        mote->motionFlags = 2;
        mote->priorityFlags = 5;
      }
      Sound_PlayFar(SE_WATER);
      wait(6);
      if (n == 12) {
        ev = Scene_FindPsynergyTrigger(0x50000005, 5,
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
      Vector_AddPolarOffset(Random16() * 5 + px(3), Random16(), &pos);
      mote = Actor_Spawn(672, pos.x, pos.y, pos.z);
      if (mote != 0) {
        mote->update = (void (*)(void))FieldEffect_UpdateDriftingFallActor;
        mote->motionFlags = 0;
        ((struct Effect05Sprite *)mote->sprite)->mode = 2;
        Actor_SelectAnimationFar(mote, 8);
        Actor_SetSpriteColorModeFar((struct Actor *)mote, 7);
      }
      wait(6);
    }
    wait(70);
  }

  i = 0;
  do {
    s32 scale;

    {
      struct EffectVector *a = &end;
      struct EffectVector *b = &start;
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
    scale = Lerp10(Q16_ONE, Q16_ONE / 4, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    i++;
  } while (i < steps);
  Actor_ReleaseFar((struct Actor *)orb);
  FieldPsynergy_FadeFromCastColor();
}

/// Each frame: one falling particle 8 to 24 pixels above the lifted object,
/// 0 to 48 pixels out in a random direction.
void FieldEffect_SpawnFallingParticles(void) {
  struct Actor *src;
  struct FallVector pos;
  struct Actor *obj;
  u32 dist;

  src = ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->child;
  SET_POSITION(&pos, src->x.fixed, src->y.fixed - Random16() * 16 + px(24),
               src->z.fixed);

  dist = Random16() * 3;
  Vector_AddPolarOffset(dist * 16, Random16(), &pos);

  obj = Actor_Spawn(673, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->motionFlags = 2;
    obj->gravity = 0x1999;
    Actor_SelectAnimationFar(obj, 0);
    obj->wait = 12;
    Actor_StartScriptFar((struct Actor *)obj,
                         gBattleEffect_SharedParticleScript);
  }
}

void FieldEffect_SpawnRadialParticleRing(struct Actor *object);

/// The collision at position for the actor standing 16 pixels higher.
static __inline__ s32 Actor_ProbeRaised(struct Actor *object,
                                        struct FxPosition *position) {
  s32 hit;

  object->groundHeight += px(16);
  hit = MapGrid_CheckCollisionFar(object, position);
  object->groundHeight -= px(16);
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
  struct FxPosition position;
  struct Actor *anchors[2];
  struct Actor *object;
  struct Actor *right;
  s32 i;
  s32 angle;
  s32 magnitude;
  struct FxPosition *p;

  work = Ram_HeapSlots->fieldEffectWork;
  source = work->mainActor;
  target = work->child;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  source->watchedActor = target;
  Actor_StartScriptFar(source, (u32)gFieldEffect_SourceHoldScript);
  /* FAKEMATCH(temporary): p holds &position everywhere but the steering loop's
   * first part. */
  p = &position;
  SET_POSITION(p, work->x, work->y + px(16), work->z);
  object = FieldEffect_SpawnCarryHand(p->x + px(32), p->y, p->z, 0x8000);
  anchors[0] = object;
  right = FieldEffect_SpawnCarryHand(p->x - px(32), p->y, p->z, 0);
  anchors[1] = right;
  if (anchors[0] == NULL || right == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  wait(15);
  SET_POSITION(p, target->x.fixed, target->y.fixed + px(16), target->z.fixed);
  Actor_SetMoveTargetFar(object, p->x + px(16), p->y, p->z);
  Actor_SetMoveTargetFar(right, p->x - px(16), p->y, p->z);
  Actor_WaitForMoveFar(object);
  Actor_WaitForMoveFar(right);
  object->x.fixed = p->x + px(16);
  object->velocityX = 0;
  right->x.fixed = p->x - px(16);
  right->velocityX = 0;
  target->update = Actor_ApplyRandomChildValues;
  Task_Add((s32)FieldEffect_SpawnFallingParticles, TASK_PASS_VBLANK);
  Sound_PlayFar(SE_PSYNERGY_START);
  target->motionFlags = 4;
  Actor_SetSpriteFlagsFar(target, 0);
  if (anchors[0] != NULL && anchors[1] != NULL) {
    while (target->y.fixed - target->groundHeight <= px(24)) {
      object->y.fixed += px(3) / 8;
      right->y.fixed += px(3) / 8;
      target->y.fixed += px(3) / 8;
      wait(1);
    }
  }
  anchors[0]->speedLimit = px(4);
  anchors[0]->acceleration = 0x8000;
  anchors[1]->speedLimit = px(4);
  anchors[1]->acceleration = 0x8000;
  target->speedLimit = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = 0;
  target->layer = 2;
  for (;;) {
    wait(1);
    if ((gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;
    angle = (u16)Input_GetDpadAngle(gInput.held);
    if (angle == 0xffff) {
      SET_POSITION(&position, target->x.fixed, target->y.fixed + px(16),
                   target->z.fixed);
      Actor_SetMoveTargetFar(anchors[0], position.x + px(16), position.y,
                             position.z);
      Actor_SetMoveTargetFar(anchors[1], position.x - px(16), position.y,
                             position.z);
      Actor_SelectAnimationFar(anchors[0], 1);
      Actor_SelectAnimationFar(anchors[1], 1);
      continue;
    }
    SET_POSITION(&position, target->x.fixed, target->y.fixed + px(16),
                 target->z.fixed);
    Vector_AddPolarOffset(px(2), angle, &position);
    Actor_SetMoveTargetFar(anchors[0], position.x + px(16), position.y,
                           position.z);
    Actor_SetMoveTargetFar(anchors[1], position.x - px(16), position.y,
                           position.z);
    Actor_WaitForMoveFar(anchors[0]);
    Actor_WaitForMoveFar(anchors[1]);
    SET_POSITION(&position, target->x.fixed, target->groundHeight,
                 target->z.fixed);
    Vector_AddPolarOffset(px(16), angle, &position);
    i = ActorScript_CheckOverlapFar(target, &position);
    // Both tests jump into the blocked path, which keeps it ahead of the drop
    if (i != 0)
      goto blocked;
    if (Actor_ProbeRaised(target, p) > 0) {
    blocked:
      Actor_SelectAnimationFar(anchors[0], 4);
      Actor_SelectAnimationFar(anchors[1], 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    {
      struct Actor *second;
      s32 x;
      s32 z;

      Sound_PlayFar(SE_NOTE_HIGH);
      x = p->x;
      z = p->z;
      Actor_SelectAnimationFar(anchors[0], 4);
      Actor_SelectAnimationFar(anchors[1], 4);
      wait(15);
      target->paused = i;
      target->speedLimit = 0x3333;
      target->acceleration = 0x3333;
      Actor_SetMoveTargetFar(target, p->x, p->y, p->z);
      anchors[0]->speedLimit = 0x3333;
      anchors[0]->acceleration = 0x3333;
      second = anchors[1];
      second->speedLimit = 0x3333;
      second->acceleration = 0x3333;
      Actor_SetMoveTargetFar(anchors[0], p->x + px(16), p->y, p->z);
      Actor_SetMoveTargetFar(second, p->x - px(16), p->y, p->z);
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
  SET_POSITION(p, target->x.fixed, target->y.fixed + px(16), target->z.fixed);
  {
    struct Actor **walk = shards;

    for (i = 0; i < 20; i++) {
      object = Actor_Spawn(0x2a1, p->x, p->y, p->z);
      *walk++ = object;
      if (object != NULL) {
        Actor_StartScriptFar(object, (u32)gFieldEffect_FragmentScript);
        object->speedLimit = Random16() + px(2);
        object->acceleration = 0x20000;
        object->motionFlags = 0;
        magnitude = Random16() * 24 + 0x80000;
        Actor_SetTargetPositionFromMagnitudeAngle(object, magnitude,
                                                  Random16());
      }
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(anchors[0]);
  Actor_ReleaseFar(anchors[1]);
  Actor_SetSpriteColorModeFar(target, work->savedPalette);
  Actor_StartScriptFar(target, (u32)(work->savedScript));
  target->update = work->savedCallback;
  target->motionFlags = 3;
  target->velocityY = px(10);
  target->bounce = px(1) / 5;
  target->layer = 0;
  source->update = NULL;
  Actor_SetSpriteColorModeFar(source, 0);
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
