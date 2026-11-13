/// @file
/// The end of Cloak, and Douse.

#include "types.h"
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "main_routine_work.h"
#include "actor.h"
#include "sprite.h"
#include "system/task.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "system.h"
#include "module/field/object.h"

#define SE_CLOAK 130

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

// What the ground check (FieldEffect_CheckCloakTile) requests when the caster
// does not stand on a tile it can hide on.
#define CLOAK_END 0x2092

// Cloak the caster: flicker it ten times, leave it half transparent, then
// check the ground under it every frame. When it cannot hide where it
// stands, show it again at once.
void FieldPsynergy_StartCloak(void) {
  struct CloakEvent *event = Ram_HeapSlots->eventWork;
  struct CloakCaster *caster =
      ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->mainObject;
  struct CloakSprite *sprite = caster->sprite;
  struct AnimationEntry *entry = sprite->entries[0];
  u32 n;
  void (*check)(void);
  s32 order;

  Sound_PlayFar(SE_CLOAK);
  Actor_SelectAnimationFar((struct Actor *)caster, 0);
  caster->hook = 0;

  for (n = 0; n < 10; n++) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->dirty = 1;
    sprite->blend = 2;
    wait(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    wait(2);
  }

  // The task priority is set before the stores; as TASK_PASS_VBLANK in the
  // call, 18 of 91 instructions differ in tla-en.
  order = TASK_PASS_VBLANK;
  sprite->blend = 2;
  entry->param = 0;
  sprite->dirty = 1;
  check = FieldEffect_CheckCloakTile;
  Task_Add(check, order);
  gMainRoutineWork.cloaked = 1;
  check();
  if (event->psynergyRequest == CLOAK_END) {
    FieldPsynergy_EndCloak();
    event->psynergyRequest = 0;
  }
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "objdisp.h"
#include "object_effect_scripts.h"

void BattleEffect_LandFallingObject(struct FallingObject *object) {
  if (object->y <= object->floor) {
    object->dispatchState = 2;
    Actor_StartScriptFar((struct Actor *)object,
                         (u32)gBattleEffect_SharedParticleScript);
    object->update = 0;
  }
}

void FieldEffect_UpdateDriftingFallActor(struct Actor *object) {
  s32 r;

  object->y -= 0x4ccc;
  r = Random16();
  object->x += r - Random16();
  if (object->y <= object->terrainHeight) {
    Actor_StartScriptFar((struct Actor *)object,
                         (u32)gBattleEffect_SharedParticleScript);
  }
}

void FieldPsynergy_DouseOnTarget(void) { FieldPsynergy_Douse(); }

#include "event_work.h"

static __inline__ s32 Lerp10(s32 start, s32 end, s32 step) {
  return start + step * (end - start) / 10;
}

void FieldPsynergy_Douse(void) {
  struct EffectVector pos;
  struct EffectVector start;
  struct EffectVector end;
  struct Effect05State *work = Ram_HeapSlots->effectWork;
  struct Actor *anchor = work->anchor;
  struct Actor *target = work->target;
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
  Sound_PlayFar(138);
  if (work->anchor == 0) {
    work->x = target->x;
    work->z = target->z;
    Vector_AddPolarOffset(16 << 16, work->angle,
                          (struct EffectVector *)&work->x);
  }
  from = &start;
  from->x = target->x;
  from->y = target->y + (16 << 16);
  from->z = target->z;
  to = &end;
  to->x = work->x;
  y = work->y;
  to->y = y + (32 << 16);
  to->z = work->z;
  if ((s8)work->variant != 0)
    to->y = y + (80 << 16);
  if (Func_08020330(anchor) == 380) {
    to->y += 32 << 16;
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
      orb->x = t;
      e = b->y;
      t = a->y;
      t += (e - t) * i / 10;
      orb->y = t;
      e = b->z;
      t = a->z;
      t += (e - t) * i / 10;
      orb->z = t;
    }
    scale = Lerp10(0x4000, 0x10000, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
  }
  wait(10);

  if ((s8)work->alternate == 0) {
    count = 10;
    if ((s8)work->highArc == 0)
      count = 24;
    for (n = 0; n < count; n++) {
      pos.x = orb->x;
      pos.y = orb->y;
      pos.z = orb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      if (n == count - 1) {
        wait(20);
        pos.x = orb->x;
        pos.y = orb->y;
        pos.z = orb->z;
      }
      mote = Actor_Spawn(301, pos.x, pos.y, pos.z);
      if (mote != 0) {
        struct Actor *ground = work->anchor;

        if (ground != 0) {
          mote->terrainHeight = ground->y;
          if (raised == 1)
            mote->terrainHeight = ground->y + (32 << 16);
        } else {
          mote->terrainHeight = pos.y - (32 << 16);
        }
        mote->update = (void (*)(void))BattleEffect_LandFallingObject;
        mote->motionFlags = 2;
        mote->priorityFlags = 5;
      }
      Sound_PlayFar(132);
      wait(6);
      if (n == 12) {
        ev = Scene_FindPsynergyTrigger(0x50000005, 5, Func_080ce31c(5));
        if (ev != 0)
          Field_RunPsynergyEvent(ev, work->firstObjectId, work->secondObjectId);
      }
    }
    wait(10);
  } else {
    if ((s8)work->highArc != 0)
      count = 10;
    else
      count = 30;
    for (n = 0; n < count; n++) {
      pos.x = orb->x;
      pos.y = orb->y;
      pos.z = orb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
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
      orb->x = t;
      e = b->y;
      t = a->y;
      t += (e - t) * i / 10;
      orb->y = t;
      e = b->z;
      t = a->z;
      t += (e - t) * i / 10;
      orb->z = t;
    }
    scale = Lerp10(0x10000, 0x4000, i);
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

  src = ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->child;
  pos.x = src->x;
  pos.y = src->y - Random16() * 16 + (24 << 16);
  pos.z = src->z;

  dist = Random16() * 3;
  Vector_AddPolarOffset(dist * 16, Random16(), &pos);

  obj = Actor_Spawn(673, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->motionFlags = 2;
    obj->gravity = 0x1999;
    Actor_SelectAnimationFar(obj, 0);
    ((struct ParticleTimer *)obj)->timer = 12;
    Actor_StartScriptFar((struct Actor *)obj,
                         gBattleEffect_SharedParticleScript);
  }
}

#include "actor_view.h"
#include "input.h"
#include "battle_effect.h"
#include "frame.h"

/* VIEW(f0decb8): tag struct EffectWork as struct EffectWork__f0decb8;
 * module/tla.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/cloak.c:351: dereferencing pointer to
 * incomplete type. */
struct EffectWork__f0decb8 {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
  struct FxObject *source;
  struct FxObject *target;
  u8 unk18[0x1c];
  u8 falls;
  u8 unk35[3];
  void *callback;
  const u8 *script;
  u8 palette;
};

void Actor_ApplyRandomChildValues(void);
void FieldEffect_SpawnRadialParticleRing(struct FxObject *object);

static __inline__ s32 Actor_ProbeRaised(struct FxObject *object,
                                        struct FxPosition *position) {
  s32 hit;

  object->ground += 0x100000;
  hit = MapGrid_CheckCollisionFar(object, position);
  object->ground -= 0x100000;
  return hit;
}

void FieldPsynergy_SteerLiftedTarget(s32 targetId) {
  struct EffectWork__f0decb8 *work;
  struct FxObject *source;
  struct FxObject *target;
  struct FxObject *shards[20];
  struct FxPosition position;
  struct FxObject *anchors[2];
  struct FxObject *object;
  struct FxObject *right;
  s32 i;
  s32 angle;
  s32 magnitude;
  struct FxPosition *p;

  work = Ram_HeapSlots->effectWork;
  source = work->source;
  target = work->target;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  source->linked = target;
  Actor_StartScriptFar((struct Actor *)source,
                       (u32)gFieldEffect_SourceHoldScript);
  /* FAKEMATCH(temporary): p holds &position everywhere but the steering
     loop's first part, which gives the shards loop's 19 r8 and its 0x20000
     r9; as position used directly, 10 of 551 instructions differ in tla-en. */
  p = &position;
  p->x = work->x;
  p->y = work->y + 0x100000;
  p->z = work->z;
  object = FieldEffect_SpawnCarryHand(p->x + 0x200000, p->y, p->z, 0x8000);
  anchors[0] = object;
  right = FieldEffect_SpawnCarryHand(p->x - 0x200000, p->y, p->z, 0);
  anchors[1] = right;
  if (anchors[0] == NULL || right == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  wait(15);
  p->x = target->x;
  p->y = target->y + 0x100000;
  p->z = target->z;
  Actor_SetMoveTargetFar(object, p->x + 0x100000, p->y, p->z);
  Actor_SetMoveTargetFar(right, p->x - 0x100000, p->y, p->z);
  Actor_WaitForMoveFar(object);
  Actor_WaitForMoveFar(right);
  object->x = p->x + 0x100000;
  object->velocityX = 0;
  right->x = p->x - 0x100000;
  right->velocityX = 0;
  target->callback = Actor_ApplyRandomChildValues;
  Task_Add((s32)FieldEffect_SpawnFallingParticles, TASK_PASS_VBLANK);
  Sound_PlayFar(130);
  target->flag = 4;
  Actor_SetSpriteFlagsFar((struct Actor *)target, 0);
  if (anchors[0] != NULL && anchors[1] != NULL) {
    while (target->y - target->ground <= 0x180000) {
      object->y += 0x6000;
      right->y += 0x6000;
      target->y += 0x6000;
      wait(1);
    }
  }
  anchors[0]->speed = 0x40000;
  anchors[0]->lift = 0x8000;
  anchors[1]->speed = 0x40000;
  anchors[1]->lift = 0x8000;
  target->speed = 0x6666;
  target->lift = 0x3333;
  target->unk5a = 0;
  target->mode = 2;
  for (;;) {
    wait(1);
    if ((gInput.pressed & 0x303) != 0)
      break;
    angle = (u16)BattleEffect_GetCycledTableWord(gInput.held);
    if (angle == 0xffff) {
      position.x = target->x;
      position.y = target->y + 0x100000;
      position.z = target->z;
      Actor_SetMoveTargetFar(anchors[0], position.x + 0x100000, position.y,
                              position.z);
      Actor_SetMoveTargetFar(anchors[1], position.x - 0x100000, position.y,
                              position.z);
      Actor_SelectAnimationFar(anchors[0], 1);
      Actor_SelectAnimationFar(anchors[1], 1);
      continue;
    }
    position.x = target->x;
    position.y = target->y + 0x100000;
    position.z = target->z;
    Vector_AddPolarOffset(0x20000, angle, &position);
    Actor_SetMoveTargetFar(anchors[0], position.x + 0x100000, position.y,
                            position.z);
    Actor_SetMoveTargetFar(anchors[1], position.x - 0x100000, position.y,
                            position.z);
    Actor_WaitForMoveFar(anchors[0]);
    Actor_WaitForMoveFar(anchors[1]);
    position.x = target->x;
    position.y = target->ground;
    position.z = target->z;
    Vector_AddPolarOffset(0x100000, angle, &position);
    i = ActorScript_CheckOverlapFar(target, &position);
    // Both tests jump into the blocked path, which keeps it ahead of the drop
    // path as the ROM lays them out; written as one || test, 389 of 551
    // instructions differ in tla-en.
    if (i != 0)
      goto blocked;
    if (Actor_ProbeRaised(target, p) > 0) {
    blocked:
      Actor_SelectAnimationFar(anchors[0], 4);
      Actor_SelectAnimationFar(anchors[1], 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(114);
      continue;
    }
    {
      struct FxObject *second;
      s32 x;
      s32 z;

      Sound_PlayFar(175);
      x = p->x;
      z = p->z;
      Actor_SelectAnimationFar(anchors[0], 4);
      Actor_SelectAnimationFar(anchors[1], 4);
      wait(15);
      target->unk5b = i;
      target->speed = 0x3333;
      target->lift = 0x3333;
      Actor_SetMoveTargetFar(target, p->x, p->y, p->z);
      anchors[0]->speed = 0x3333;
      anchors[0]->lift = 0x3333;
      second = anchors[1];
      second->speed = 0x3333;
      second->lift = 0x3333;
      Actor_SetMoveTargetFar(anchors[0], p->x + 0x100000, p->y, p->z);
      Actor_SetMoveTargetFar(second, p->x - 0x100000, p->y, p->z);
      Actor_WaitForMoveFar(target);
      target->x = x;
      target->z = z;
      target->velocityX = i;
      target->velocityZ = i;
      wait(10);
      break;
    }
  }
  Actor_SelectAnimationFar(anchors[0], 4);
  Actor_SelectAnimationFar(anchors[1], 4);
  Task_Remove((u32)FieldEffect_SpawnFallingParticles);
  Sound_PlayFar(135);
  wait(15);
  Sound_PlayFar(135);
  wait(15);
  p->x = target->x;
  p->y = target->y + 0x100000;
  p->z = target->z;
  {
    // The shards are stored through a walking pointer, so the counter runs
    // down from 19 as the ROM's does; as shards[i], 22 of 551 instructions
    // differ in tla-en.
    struct FxObject **walk = shards;

    for (i = 0; i < 20; i++) {
      object = Actor_Spawn(0x2a1, p->x, p->y, p->z);
      *walk++ = object;
      if (object != NULL) {
        Actor_StartScriptFar((struct Actor *)object,
                             (u32)gFieldEffect_FragmentScript);
        object->speed = Random16() + 0x20000;
        object->lift = 0x20000;
        object->flag = 0;
        magnitude = Random16() * 24 + 0x80000;
        Actor_SetTargetPositionFromMagnitudeAngle(object, magnitude,
                                                  Random16());
      }
    }
  }
  Sound_PlayFar(131);
  Actor_ReleaseFar(anchors[0]);
  Actor_ReleaseFar(anchors[1]);
  Actor_SetSpriteColorModeFar((struct Actor *)target, work->palette);
  Actor_StartScriptFar((struct Actor *)target, (u32)(work->script));
  target->callback = work->callback;
  target->flag = 3;
  target->velocityY = 0xa0000;
  target->gravity = 0x3333;
  target->mode = 0;
  source->callback = NULL;
  Actor_SetSpriteColorModeFar((struct Actor *)source, 0);
  if ((s8)work->falls != 0) {
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
