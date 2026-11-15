/// @file
/// The field Psynergy that spread effects around their target: Whirlwind,
/// Halt, Catch, Lash, Retreat and Avoid, with the particle rings and arcs
/// they share.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "gamepak/far_call/field.h"
#include "macros.h"

#include "field/event_commands.h"
#include "field/field_bank.h"
#include "field/object.h"
#include "field/field_psynergy.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/scene_call.h"
#include "field/flag_call.h"
#include "gamepak/far_call/window.h"
#include "text/msg_ids.h"

extern const s32 gBattleEffect_SharedParticleScript[];

struct OrbitEffect {
  u8 unused[6]; ///< no code names it
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unusedAfterScale[28]; ///< no code names it
  s32 speed;
  u8 unusedAfterSpeed[24]; ///< no code names it
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

/// Spawns 17 particles that spread from origin in a flat ring, their x velocity
/// 1.5 times their z velocity, so the ring is wider than deep as the field is
/// drawn.
void FieldEffect_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions options;
  s32 velocity[3];
  s32 sine;
  s32 angle;
  u32 i;

  options.palette = 0;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xCCCC;
  options.startScaleY = 0xCCCC;
  i = 0;
  do {
    angle = i << 12;
    velocity[0] = Trig_Cos(angle) * 3 / 2;
    velocity[1] = 0;
    sine = Trig_Sin(angle);
    velocity[2] = sine;
    FieldEffect_SpawnResident(origin->x, origin->y, origin->z, velocity[0],
                              velocity[1], sine, 0x01090001, &options);
    i++;
  } while (i <= 16);
}

/// Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame, now
/// and then turning sharply; after 101 frames it hands over to the shared
/// particle script.
void FieldEffect_WanderAroundAnchor(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16() + px(2);
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += TRIG_TURN / 32;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Actor_StartScriptFar((struct Actor *)effect,
                         (u32)gBattleEffect_SharedParticleScript);
}

/// Per-frame update of a Whirlwind particle: circles the anchor at 8 pixels,
/// a sixteenth of a turn a frame; after 121 frames it starts to wander
/// (FieldEffect_WanderAroundAnchor) with upward speed and gravity. As The
/// Broken Seal's FieldEffect_CircleAnchor.
void FieldEffect_CircleAnchor(struct OrbitEffect *effect) {
  struct OrbitEffect *anchor;
  s32 angle;
  s32 radius;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  radius = px(8);
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += TRIG_TURN / 32;
  if (++effect->timer == 121) {
    effect->update = FieldEffect_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = Q16_ONE * 3;
    effect->angle = Random16();
  }
}

#define Actor_SetPositionAndResetMotionFar                                     \
  ((s32 (*)(void *, s32, s32, s32))Actor_SetPositionAndResetMotionFar)

/// Shrinks an event-selected Whirlwind target each frame; below half size,
/// moves it to the origin, stops it and removes the update.
void FieldEffect_ShrinkActorScaleUntilHalf(struct Actor *object) {
  s32 scale;

  scale = object->scaleX - 0x80;
  object->scaleY = scale;
  object->scaleX = scale;
  if (scale < Q16_ONE / 2) {
    Actor_SetPositionAndResetMotionFar(object, 0, 0, 0);
    object->update = 0;
  }
}

/// Runs Whirlwind cast on a target, using the target already stored in the
/// field effect.
void FieldPsynergy_WhirlwindOnTarget(s32 unused) { FieldPsynergy_Whirlwind(); }
#undef Actor_SetPositionAndResetMotionFar

// An orbiting mote: field_psynergy.c's OrbitEffect.

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object);

#define ORB_OBJECT 280
#define MOTE_OBJECT 308

// The two targets Whirlwind treats apart, by their first animation.
#define TARGET_ANIMATION_A 372
#define TARGET_ANIMATION_B 373

/// Whirlwind: carries an orb from the caster to the Psynergy's position over
/// ten frames, growing it from a quarter size, then plays out what it does
/// there.
void FieldPsynergy_Whirlwind(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *caster = scene->mainActor;
  struct Actor *target = scene->child;
  struct OrbTargetSprite *sprite = target->sprite;
  struct EffectVector position;
  struct EffectVector from;
  struct EffectVector to;
  struct Actor *orb;
  struct OrbitEffect *mote;
  s32 kind = ORB_PLAIN;
  s32 step;
  s32 i;
  s32 frames = 11;

  if (scene->childOption != 0)
    return;
  if (scene->enabled == 0) {
    s32 animation;

    kind = ORB_ORBITING;
    animation = sprite->entries[0]->animationId;
    if (animation == TARGET_ANIMATION_A)
      kind = ORB_WANDER;
    if (animation == TARGET_ANIMATION_B)
      kind = ORB_HANDOFF;
  }

  SET_POSITION(&from, caster->x.fixed, caster->y.fixed, caster->z.fixed);
  SET_POSITION(&to, scene->x, scene->y - px(4), scene->z);
  orb = Actor_Spawn(ORB_OBJECT, 0, 0, 0);
  if (orb == 0)
    return;

  FieldPsynergy_FadeToCastColor();
  Actor_SelectAnimationFar(orb, 2);
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(orb, from.x + (to.x - from.x) * step / 10,
                       from.y + (to.y - from.y) * step / 10,
                       from.z + (to.z - from.z) * step / 10);
    scale = step * 0x10ccc / 10 + Q16_ONE / 4; // from 0.25 to 1.3
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    step++;
  } while (step < frames);
  orb->scaleX = 0x1b333; // 1.7 wide
  orb->scaleY = 0x14ccc; // 1.3 high
  Sound_PlayFar(SE_RUSH);

  if (kind == ORB_PLAIN) {
    Actor_SelectAnimationFar(orb, 1);
    wait(30);
    Sound_PlayFar(SE_WHOOSH);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  } else if (kind == ORB_ORBITING) {
    // Sixteen motes, one every six frames, 4 pixels above the orb and 0.8
    // higher each, 3 to 8 pixels out, circle it while the target shrinks.
    wait(20);
    if (target != 0)
      target->update = (void (*)(void))FieldEffect_ShrinkActorScaleUntilHalf;
    for (i = 0; i <= 15; i++) {
      s32 distance;

      SET_POSITION(&position, orb->x.fixed, orb->y.fixed + i * 0xcccc + px(4),
                   orb->z.fixed);
      distance = Random16() * 5 + px(3);
      Vector_AddPolarOffset(distance, Random16(), &position);
      mote = (struct OrbitEffect *)Actor_Spawn(MOTE_OBJECT, position.x,
                                               position.y, position.z);
      if (mote != 0) {
        mote->update = FieldEffect_CircleAnchor;
        mote->anchor = orb;
        mote->timer = 0;
        mote->pause = 0;
        mote->angle = Random16();
      }
      wait(6);
    }
    wait(20);
    wait(120);
    Actor_SelectAnimationFar(orb, 1);
    wait(30);
    Sound_PlayFar(SE_WHOOSH);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  } else if (kind == ORB_WANDER || kind == ORB_HANDOFF) {
    wait(5);
    Actor_SelectAnimationFar(orb, 1);
    if (kind == ORB_WANDER) {
      // Sixteen motes that wander about the orb, the first pausing longest.
      for (i = 0; i <= 15; i++) {
        s32 distance;

        SET_POSITION(&position, orb->x.fixed, orb->y.fixed + i * 0xcccc + px(4),
                     orb->z.fixed);
        distance = Random16() * 5;
        distance += px(3);
        Vector_AddPolarOffset(distance, Random16(), &position);
        mote = (struct OrbitEffect *)Actor_Spawn(MOTE_OBJECT, position.x,
                                                 position.y, position.z);
        if (mote != 0) {
          mote->update = FieldEffect_WanderAroundAnchor;
          mote->timer = 20 + i * -2;
          mote->pause = 0;
          mote->speed = 0x1999;
          mote->scale = Q16_ONE * 3;
          mote->angle = Random16();
        }
      }
    }
    BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
    FieldPsynergy_FadeFromCastColor();
    ((struct OrbitEffect *)orb)->timer = 0;
    orb->update = (void (*)(void))BattleEffect_AgeAndCueAtEighty;
    HangingVine_Swing(scene->secondActorId);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  }
  FieldPsynergy_FadeFromCastColor();
}
#undef ORB_OBJECT
#undef MOTE_OBJECT
#undef TARGET_ANIMATION_A
#undef TARGET_ANIMATION_B

/// Per-frame update: counts the actor's frames and, at frame 80, plays
/// SE_WHOOSH and stops itself.
void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object) {
  if (object->age == 80) {
    object->update = 0;
    Sound_PlayFar(SE_WHOOSH);
  }
  object->age++;
}

/// Field effect sprite callback: the particle heads for a point 40 to 70 pixels
/// from where it appeared, then for a point 4 pixels round 8 pixels above the
/// Psynergy's position on screen, and is freed on arrival.
void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
  struct FieldEffectWork *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = Ram_HeapSlots->fieldEffectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + px(40), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = px(4);
    effect->maxSpeed = px(4);
    effect->limitTurnStep = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    SET_POSITION(&pos, scene->x, scene->y + px(8), scene->z);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(px(4), Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// Pauses the scene's child (byte 91) and runs the fallback transition.
void FieldPsynergy_HaltOnTarget(void) {
  (*(u8 **)((u8 *)Ram_HeapSlots->fieldEffectWork + 20))[91] = 1;
  FieldPsynergy_Halt();
}

/// Throws an actor from the caster to the Psynergy's position over 10 frames
/// and scatters it into 16 particles.
void FieldPsynergy_Halt(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *user = scene->mainActor;
  struct Actor *obj;
  struct FieldParticle *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y.fixed;
  obj = Actor_Spawn(SPRITE_STOPWATCH, 0, 0, 0);
  step = 0;
  Actor_SelectAnimationFar(obj, 0);
  if (obj == 0)
    return;

  FieldPsynergy_FadeToCastColor();
  SET_POSITION(&from, user->x.fixed, user->y.fixed + px(16), user->z.fixed);
  SET_POSITION(&to, scene->x, scene->y + px(8), scene->z);
  for (;;) {
    s32 steps = 11;
    s32 scale;

    SET_ACTOR_POSITION(obj, from.x + (to.x - from.x) * step / 10,
                       from.y + (to.y - from.y) * step / 10,
                       from.z + (to.z - from.z) * step / 10);
    scale = FieldEffect_Interpolate(Q16_ONE, Q16_ONE / 4, step);
    obj->scaleX = scale;
    obj->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  wait(5);
  Actor_SelectAnimationFar(obj, 1);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_HIGH);

  slot = &((struct FallbackScene *)scene)->slots[0];
  for (i = 0; i < 16; i++) {
    SET_POSITION(&pos, obj->x.fixed, obj->y.fixed + px(8), obj->z.fixed);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(px(4), Random16(), &pos);
    FieldParticle_Initialize(slot, 673, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateHaltParticle);
    Animation_ApplyChildValuesToRecordFar(slot->object, 7);
    slot++;
  }

  SET_POSITION(&pos, obj->x.fixed, obj->y.fixed + px(8), obj->z.fixed);
  wait(8);
  Actor_ReleaseFar(obj);
  wait(4);
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(void *, s32))Actor_SetAnimationSpeedFar)

/// Per-frame update of a paused actor: gives its sprite one of the table's
/// eight params at random.
void FieldPsynergy_UpdateHaltedActor(struct Actor *actor) {
  s8 *table = gBattleEffect_RandomChildValues;
  s32 random = Random16();
  Actor_SetSpriteColorModeFar(actor, table[(u32)(random * 8) >> 16]);
}

/// Pauses actor id, keeping its per-frame update and, if it is animated, its
/// first animation's param in gMainRoutineWork.
void FieldPsynergy_HaltActor(s32 actorId) {
  struct Actor *actor;
  struct SpriteAnimation *entry;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    gMainRoutineWork.pausedUpdate = (u32)actor->update;
    gMainRoutineWork.pausedPose = 0;
    if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
      entry = ((struct Sprite *)actor->sprite)->entries[0];
      if (entry != NULL) {
        gMainRoutineWork.pausedPose = entry->colorMode;
      }
    }
    actor->update = (void (*)(void))FieldPsynergy_UpdateHaltedActor;
    actor->paused = 1;
    Actor_SetAnimationSpeedFar(actor, 0);
  }
}

/// Resumes actor id with the update and param kept by FieldPsynergy_HaltActor.
void FieldPsynergy_ResumeHaltedActor(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    if (actor->update == (void (*)(void))FieldPsynergy_UpdateHaltedActor) {
      actor->update = (void (*)(void))gMainRoutineWork.pausedUpdate;
      gMainRoutineWork.pausedUpdate = 0;
      Actor_SetSpriteColorModeFar(actor, (s8)gMainRoutineWork.pausedPose);
    }
    actor->paused = 0;
    Actor_SetAnimationSpeedFar(actor, 16);
  }
}

s32 Speaker_GetHaltedLines(s32 id);

/// Returns no target (-1) when Halt passes actor id over (its sprite has no
/// halted lines, 0xff), else id.
s32 FieldPsynergy_FilterHaltTarget(s32 id) {
  if (Speaker_GetHaltedLines(id) == 0xff)
    return -1;
  return id;
}

/// Catch cast on a target: runs FieldPsynergy_Catch.
void FieldPsynergy_CatchOnTarget(s32 unused) { FieldPsynergy_Catch(); }

/// The value step tenths of the way from from to to.
static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
  return from + (to - from) * step / 10;
}

/// Catch: carries the hand (sprite 278) from 16 pixels above the caster to the
/// goal in 10 steps, growing from a quarter to full size, presses down 20
/// pixels moving the target to (-9, -9), rises 19.5 pixels and shrinks back.
void FieldPsynergy_Catch(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *mainActor = scene->mainActor;
  struct Actor *secondary = scene->child;
  struct Actor *object;
  struct FxPosition13 spawn;
  struct FxPosition13 origin;
  struct FxPosition13 target;
  s32 step;
  s32 index;
  s32 steps = 11;

  SET_POSITION(&origin, mainActor->x.fixed, mainActor->y.fixed + px(16),
               mainActor->z.fixed);
  if (scene->enabled) {
    SET_POSITION(&target, mainActor->x.fixed, mainActor->y.fixed + px(32),
                 mainActor->z.fixed);
    Vector_AddPolarOffset(px(32), scene->angle, &target.x);
  } else {
    SET_POSITION(&target, scene->x, scene->y + px(32), scene->z);
  }
  SET_POSITION(&spawn, scene->x, scene->y + px(32), scene->z);
  object = Actor_Spawn(SPRITE_MOVE_HAND, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  object->facing = mainActor->facing;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  Actor_SelectAnimationFar(object, 5);
  Actor_SetSpriteColorModeFar(object, 1);
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(object, origin.x + (target.x - origin.x) * step / 10,
                       origin.y + (target.y - origin.y) * step / 10,
                       origin.z + (target.z - origin.z) * step / 10);
    scale = Interpolate(Q16_ONE / 4, Q16_ONE, step);
    object->scaleX = scale;
    object->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  wait(10);
  Actor_SelectAnimationFar(object, 6);
  wait(15);
  for (index = 0; index < 10; index++) {
    object->y.fixed -= px(2);
    wait(1);
  }
  Actor_SelectAnimationFar(object, 5);
  Sound_PlayFar(SE_WATER);
  if (secondary != 0)
    Actor_SetPositionAndResetMotionFar(secondary, -px(9), secondary->y.fixed,
                                       -px(9));
  wait(20);
  for (index = 0; index < 13; index++) {
    object->y.fixed += px(3) / 2;
    wait(1);
  }
  wait(10);
  Sound_PlayFar(SE_MENU_ERROR);
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(object, target.x + (origin.x - target.x) * step / 10,
                       target.y + (origin.y - target.y) * step / 10,
                       target.z + (origin.z - target.z) * step / 10);
    scale = Interpolate(Q16_ONE, Q16_ONE / 4, step);
    object->scaleX = scale;
    object->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  Actor_ReleaseFar(object);
  FieldPsynergy_FadeFromCastColor();
}

#define EffectSprite_DrawAtWorldPosition EffectSprite_DrawAtWorldPosition
void Rope_GetEndNodePosition(s32 target, struct Vector3 *position);

/// Task of the Lash: its scattering particles along the rope.
void FieldPsynergy_UpdateLash(void) {
  struct ScatterWork *work = (struct ScatterWork *)Ram_HeapSlots->battleEffect;
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *object = work->object;
  struct Vector3 position;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  s32 i;

  work->flash = 0;
  for (i = 0; i < work->spawnCount; i++) {
    s32 distance;
    s32 speed;

    particle = &work->particles[work->next];
    SET_POSITION(particle, object->x.fixed, object->y.fixed, object->z.fixed);
    distance = Random16() * 12 + px(4);
    Vector_AddPolarOffset(distance, Random16(), particle);
    particle->velocityX = 0;
    particle->velocityY = px(2) - Random16() * 2;
    particle->velocityZ = 0;
    speed = Random16() + 0x18000;
    Vector_AddPolarOffset(speed, Random16(), &particle->velocityX);
    particle->timer = 0;
    work->next++;
    work->next &= 63;
  }

  for (i = 0, particle = work->particles; i <= 63; i++, particle++) {
    s32 timer = particle->timer;

    if (timer >= 0) {
      sprite = &work->sprites[i];
      if (i & 1)
        sprite->oam.index = work->tileA + (timer & 3) * 2;
      else
        sprite->oam.index = work->tileB + (timer & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_IntegrateVector3(particle, 63, -0x1400);
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  switch (work->phase) {
  case 0: {
    s32 a = Random16();
    s32 b = Random16();
    s32 c, d;

    position.x = scene->x + a * 3 - b * 3;
    c = Random16();
    d = Random16();
    position.y = scene->y + c * 3 - d * 3;
    position.z = scene->z;
    Actor_SetPositionAndResetMotionFar(object, position.x, position.y,
                                       position.z);
    if (work->timer == 20) {
      work->phase++;
      work->timer = -1;
    }
    break;
  }
  case 1:
    Actor_SetPositionAndResetMotionFar(object, scene->x, scene->y, scene->z);
    if (work->timer == 6) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 0)
      work->flash = 1;
    Rope_GetEndNodePosition(event->targetX, &position);
    SET_ACTOR_POSITION(object, position.x, position.y - px(4), position.z);
    if (work->hit != 0) {
      work->phase++;
      work->timer = -1;
    }
    if (work->timer == 90) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 3:
    Actor_SelectAnimationFar(object, 3);
    work->spawnCount = 3;
    object->y.fixed += work->timer << 17;
    if (work->timer == 10) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 4:
    object->scaleX = 0;
    work->spawnCount = 0;
    if (work->timer == 30)
      work->phase = FIELD_PSYNERGY_FINISHED;
    break;
  }
  work->timer++;
}

/// Lash cast on a target: finds the rope's two ends (Rope_FindById); with none,
/// Lash runs as if cast on nothing (enabled), else the field work keeps the
/// rope and its ends. Then runs FieldPsynergy_Lash.
void FieldPsynergy_LashOnTarget(s32 value) {
  struct FieldWork *state = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  s32 second;
  s32 first;

  if (Rope_FindById(value, &first, &second)) {
    work->enabled = 1;
  } else {
    state->targetY = value;
    state->targetZ = first;
    state->targetX = second;
  }
  FieldPsynergy_Lash();
}

/// Lash: a hand forms over the spot and comes down on it. With no target it
/// bursts into sparkles; cast on a rope's end it whips out and ties the rope
/// (Rope_Lash) at the event's target.
void FieldPsynergy_Lash(void) {
  struct ScatterWork *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x2114);
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *caster = scene->mainActor;
  struct Actor *object;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  s32 slotA;
  s32 slotB;
  s32 tileA;
  s32 i;

  if (scene->child != 0) {
    scene->y += px(4);
    scene->z += px(4);
  }
  FieldPsynergy_FadeToCastColor();
  object = Actor_Spawn(363, 0, 0, 0);
  if (object != 0) {
    object->facing = caster->facing;
    object->motionFlags = 0;
    if (caster->priorityFlags & ACTOR_PRIORITY_IGNORES_RISES)
      object->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    Actor_SelectAnimationFar(object, 0);
    Sound_PlayFar(SE_ITEM_BREAK);
    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                        work);
    slotA = VramCache_FindFreeEntry();
    tileA = VramCache_Load(slotA, 0x100, work);
    work->tileA = tileA;
    Decompress_TaggedLz(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    slotB = VramCache_FindFreeEntry();
    work->tileB = VramCache_Load(slotB, 0x100, work);

    particle = work->particles;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--) {
      EffectSprite_Init(sprite, 4, 4, 0, tileA);
      sprite->depth = 250;
      sprite->oam.priority = 0;
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      SET_POSITION(particle, object->x.fixed, object->y.fixed, object->z.fixed);
      Vector_AddPolarOffset(px(12), Random16(), particle);
      particle->timer = -1;
      sprite++;
      particle++;
    }

    FieldPsynergy_GrowFromCaster(object, scene->x, scene->y + px(32), scene->z);
    Actor_SelectAnimationFar(object, 1);
    wait(10);
    for (i = 15; i >= 0; i--) {
      object->y.fixed += -px(2);
      wait(1);
    }
    Sound_PlayFar(SE_WATER);
    if (scene->enabled != 0) {
      for (i = 15; i >= 0; i--)
        BattleEffect_SpawnScatterParticle(object->x.fixed, object->y.fixed,
                                          object->z.fixed);
      wait(1);
    } else {
      wait(6);
      work->object = object;
      work->phase = 0;
      work->timer = 0;
      work->next = 0;
      work->spawnCount = 1;
      work->hit = 0;
      Task_Add(FieldPsynergy_UpdateLash, TASK_PASS_VBLANK);
      while (work->phase != FIELD_PSYNERGY_FINISHED) {
        if (work->flash == 1) {
          Sound_PlayFar(SE_LASH);
          Rope_Lash(event->targetX, event->targetY, event->targetZ);
          work->hit = 1;
          Sound_PlayFar(SOUND_FADE_OUT_SE5);
        }
        wait(1);
      }
      Task_Remove(FieldPsynergy_UpdateLash);
    }
    VramCache_FreeEntry(slotA);
    VramCache_FreeEntry(slotB);
    Actor_ReleaseFar(object);
  }
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Per-frame update: spins by FACING_STEP + timer * 80 a frame, speeding up
/// (timer counts up) until timer * 80 reaches FACING_STEP: from 1/16 turn a
/// frame to just over 1/8 in 52 frames, as FieldEffect_AdvanceAngleWithRamp.
void FieldEffect_UpdateSpinAngle(struct Actor *object) {
  u32 step;

  step = object->timer * 0x50;
  object->facing += step + FACING_STEP;
  if (step < 0x1000)
    object->timer++;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
void FieldEffect_UpdateRetreatCaster(struct Actor *obj) {
  u32 scale;
  u32 scaleX;
  s32 step = -0x280;

  obj->scaleY += step;
  scaleX = obj->scaleX;
  scale = scaleX + step;
  obj->facing += TRIG_TURN / 8;
  obj->y.fixed += px(1);
  obj->scaleX = scale;
  if ((s32)scale < 0x3000) {
    obj->spriteStorageKind = 0;
  }
}

/// Field effect sprite callback: a particle flies 120 pixels from the main
/// actor's screen position (8 pixels lower) in a direction within 1/8 turn of
/// up the screen, at up to 5 pixels a frame, and starts again from the main
/// actor on arrival until it takes its last flight.
void FieldEffect_UpdateRadialLaunch(struct FieldParticle *slot) {
  struct LaunchVector pos;
  struct LaunchObject *src;
  s32 state;
  u32 r;
  struct LaunchVector *arg;
  s32 lift;

  src = ((struct LaunchScene *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x;
    arg = &pos;
    pos.y = src->y;
    pos.z = src->z;
    Camera_WorldToScreen(arg);

    slot->x = slot->originX = pos.x;
    slot->z = slot->originZ = pos.z + px(8);

    r = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = px(5);
    slot->maxSpeed = px(5);
    slot->limitTurnStep = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlayFar(SE_GLITTER);
  } else if (state == 1) {
    if (FieldParticle_HasPendingTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(slot) == 0)
      FieldEffect_ClearOwnedSlot(slot);
  }
}

#define SLOT_MAX 24

struct SpinScene {
  u8 unused[0x10]; ///< no code names it
  struct SpinObject *mainActor;
  u8 unk14[4];
  s16 firstActorId;
  u8 unusedAfterFirstActorId[0x36]; ///< no code names it
  struct FieldParticle slots[SLOT_MAX];
};

void Actor_ApplyRandomChildValues(struct SpinObject *owner);

/// Spins the main actor and shrinks it while 24 particles fly out, one a frame.
void FieldPsynergy_Retreat(void) {
  struct SpinScene *scene = Ram_HeapSlots->fieldEffectWork;
  struct SpinObject *obj = scene->mainActor;
  struct SpinVector pos;
  struct FieldParticle *slot;
  struct FieldParticle *slots;
  u8 last;
  u32 i;

  Event_Start();
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  FieldPsynergy_FadeToCastColor();
  wait(10);
  Actor_FaceDirectionById(scene->firstActorId, 0x4000, 0);
  wait(30);
  obj->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_SelectAnimationFar(obj, 28);
  wait(40);

  Sound_PlayFar(SE_SHIMMER);
  Actor_SetSpriteColorModeFar(obj, 0);
  Actor_SelectAnimationFar(obj, 3);
  obj->update = FieldEffect_UpdateSpinAngle;
  obj->timer = 0;
  wait(70);

  Actor_SetSpriteFlagsFar(obj, 0);
  obj->motionFlags = 0;
  obj->update = FieldEffect_UpdateRetreatCaster;
  obj->targetX = EFFECT_NO_TARGET;
  SET_POSITION(&pos, obj->x, obj->y, obj->z);
  Camera_WorldToScreen(&pos);
  i = 0;
  slot = scene->slots;
  slots = slot;
  do {
    s32 scale;

    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateRadialLaunch);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333;
    slot->scaleY = scale;
    slot->scaleX = scale;
    wait(1);
    i++;
    slot++;
  } while (i < SLOT_MAX);
  wait(70);

  i = 0;
  last = 2;
  slot = slots;
  do {
    if (slot->active != 0)
      slot->state = last;
    i++;
    slot++;
  } while (i < SLOT_MAX);
  wait(40);
  FieldPsynergy_FadeFromCastColor();
  obj->fall = 0;
  obj->y = obj->floor;
  wait(10);
}

/// Per-frame update of a falling arc: grows toward full size (3/64 a frame)
/// as it falls 2 pixels a frame on the actor it follows, 9 pixels in front
/// of it and up to 5 pixels further while small; released 10 pixels above
/// the ground (4 on the World Map).
void FieldEffect_UpdateDescendingParticlePositiveArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct MainRoutineWork *party = &gMainRoutineWork;

  limit = particle->groundHeight + 0xA0000;
  source = particle->watchedActor;
  if (party->mapId == SCENE_CLEAR)
    limit = particle->groundHeight + px(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  SET_ACTOR_POSITION(particle, source->x.fixed, y - px(2),
                     source->z.fixed + ((Q16_ONE - scale) * 5) + px(9));
}

/// As FieldEffect_UpdateDescendingParticlePositiveArc for the arc drawn upside
/// down, 16 pixels in front of the actor and up to 5 pixels nearer.
void FieldEffect_UpdateDescendingParticleNegativeArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct MainRoutineWork *party = &gMainRoutineWork;

  limit = particle->groundHeight + 0xA0000;
  source = particle->watchedActor;
  if (party->mapId == SCENE_CLEAR)
    limit = particle->groundHeight + px(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  SET_ACTOR_POSITION(particle, source->x.fixed, y - px(2),
                     source->z.fixed - ((Q16_ONE - scale) * 5) + px(16));
}

/// Spawns a pair of falling arcs (one upside down) on arc, 64 pixels above it,
/// at 0.4 of full size, drawn with the tiles in effectWork->arcTileSlot.
void FieldEffect_SpawnDescendingArcParticles(struct ArcObject *source) {
  u8 *work = Ram_HeapSlots->fieldEffectWork;
  struct ArcObject *leader = *(struct ArcObject **)(work + 16);
  struct ArcObject *objects[2];
  struct ArcObject *object;
  struct ArcSprite *sprite;
  s32 i;
  u8 zero;

  for (i = 0; i <= 1; i++) {
    object = Actor_CreateFar(SPRITE_GOLDEN_RINGS, source->x, source->y + px(64),
                             source->z);
    objects[i] = object;
    if (object == NULL)
      continue;
    object->unk14 = source->unk14;
    sprite = object->sprite;
    object->unk55 = 0;
    object->unk64 = 0;
    zero = 0;
    object->source = source;
    object->scaleY = 0x6666;
    object->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    Sprite_StartAnimationFar(sprite, 0);
    sprite->animation = zero;
    VramCache_FreeEntry(sprite->resource);
    sprite->resource = *(u16 *)(work + 0x710);
    sprite->active = 1;
    sprite->attr.half.tile = gVramCacheEntries[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->picture->visible = zero;
  }
  objects[0]->callback =
      (void *)FieldEffect_UpdateDescendingParticleNegativeArc;
  objects[0]->sprite->attr.byte.variant = 0;
  objects[1]->callback =
      (void *)FieldEffect_UpdateDescendingParticlePositiveArc;
  objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}

/// The caster during Avoid (FieldPsynergy_Avoid): sheds a pair of falling
/// arcs every 5 frames (7 in scene Clear), and spins in phase 1.
void FieldEffect_UpdateAvoidCaster(struct ArcEffectObject *caster) {
  s16 phase;
  s32 elapsedFrames;
  struct MainRoutineWork *party;

  phase = caster->phase;
  elapsedFrames = (s16)caster->step++;
  party = &gMainRoutineWork;

  if (party->mapId == SCENE_CLEAR) {
    if (elapsedFrames % 7 == 0)
      FieldEffect_SpawnDescendingArcParticles(caster);
  } else if (elapsedFrames % 5 == 0) {
    FieldEffect_SpawnDescendingArcParticles(caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

/// Avoid (or the Sacred Feather) has worn off, and the game says so.
/// "Avoid's effects wore off." (or "Sacred Feather's ...").
///
/// Clears gMainRoutineWork.avoidDistance. Reached through
/// FieldPsynergy_RunExtraHandler when the Psynergy request the leader's walking
/// raises as Avoid runs out (field_sequence_walking.c) is handled.
void FieldPsynergy_ShowAvoidWoreOff(void) {
  gMainRoutineWork.avoidDistance = 0;
  WorldMap_Freeze();
  if (gMainRoutineWork.avoidByItem == 0) {
    MessageArgument_PushFar(150, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgAbilityWoreOff, 1);
  } else {
    MessageArgument_PushFar(236, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)&MsgItemWoreOff, 1);
  }
  WorldMap_ReloadPendingAreaGraphics();
}

/// Avoid: the caster spins, arcs rain down round it, then it flashes.
void FieldPsynergy_Avoid(void) {
  struct ArcScene *scene;
  struct ArcEffectObject *object;
  struct ArcGroup *group;
  struct ArcEntry *entry;
  const u8 *tiles;
  u32 angle;
  s32 flags;
  s32 slot;
  s32 count;
  s32 phase;

  scene = Ram_HeapSlots->fieldEffectWork;
  object = scene->mainActor;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  flags = group->flags;
  slot = VramCache_FindFreeEntry();
  scene->arcTileSlot = slot;
  tiles = gFieldEffect_ArcSparkTiles;
  VramCache_Load((s16)slot, 0x100, tiles);
  gMainRoutineWork.avoidDistance = 0x09600000;
  gMainRoutineWork.avoidByItem = GameFlag_Get(FLAG_ITEM_USE);
  Actor_SetSpriteColorModeFar(object, 0);
  object->callback = FieldEffect_UpdateAvoidCaster;
  object->phase = 0;
  object->step = 0;
  Sound_PlayFar(SE_SPIN);
  wait(15);
  object->phase = 1;
  wait(10);
  for (count = 0; count < 20; count++) {
    entry->param = 7;
    group->dirty = 1;
    wait(2);
    entry->param = 0;
    group->dirty = 1;
    group->flags = 1;
    wait(3);
  }
  object->callback = NULL;
  object->angle = angle;

  // For 70 frames, set and clear flags bit 2 on a five-frame cycle.
  for (count = 0; count < 70; count++) {
    phase = count % 5;
    if (phase == 0) {
      group->dirty = 1;
      group->flags = flags | 2;
    } else if (phase == 2) {
      group->dirty = 1;
      group->flags = flags;
      entry->param = 0;
    }
    if (count == 15)
      Sound_PlayFar(SE_NOTE_MID);
    wait(1);
  }
  group->flags = flags;
  group->dirty = 1;
  {
    u32 cleared = 0;

    entry->param = cleared;
  }
  wait(1);

  VramCache_FreeEntry(scene->arcTileSlot);
  FieldMessage_ShowFar((s32)&MsgMonstersAttackLess, 1);
}

/// Waits, at most 90 frames, until both pairs of swelling arcs have run their
/// 32 frames.
void BattleEffect_WaitForArcGroups(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->arcEffect;
  s32 i = 0;

  while (work->age[0] <= 31 || work->age[1] <= 31) {
    wait(1);
    i++;
    if (i > 89)
      return;
  }
}

/// Per-frame update: flashes an actor between colour mode 7 and 0 every 2
/// frames and spawns the swelling arcs on it every 16.
void FieldEffect_FlickerActorAndSpawnArcs(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorModeFar(object, 7);
  } else {
    Actor_SetSpriteColorModeFar(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    BattleEffect_SpawnScaledArcObjects(object);
  }
}

/// Per-frame update: cycles an actor through colour modes 0 to 5, one step
/// every 2 frames, and spawns the swelling arcs on it every 16.
void FieldEffect_CycleActorAndSpawnArcs(struct Actor *object) {
  if (gFrameCount & 1)
    Actor_SetSpriteColorModeFar(object, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    BattleEffect_SpawnScaledArcObjects(object);
}

/// Gives the caster its look while it casts: 0 none (normal colours), 1
/// flashing white with swelling arcs (FieldEffect_FlickerActorAndSpawnArcs), 2
/// cycling colours with them (FieldEffect_CycleActorAndSpawnArcs).
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Actor_SetSpriteColorModeFar(object, 0);
  } else if (mode == 1) {
    object->update = (void (*)(void))FieldEffect_FlickerActorAndSpawnArcs;
  } else {
    object->update = (void (*)(void))FieldEffect_CycleActorAndSpawnArcs;
  }
}

// The arc effect's next pair of scaled sprites.

/// Sets up the current group's two arc sprites from the effect's tiles, at the
/// target's priority, then restarts that group's age, records the target and
/// switches to the other group.
void BattleEffect_SpawnScaledArcObjects(struct Actor *target) {
  struct ArcWork *work = Ram_HeapSlots->arcEffect;
  s32 tiles = gVramCacheEntries[work->resource].offset >> 5;
  s32 group = work->group;
  struct EffectSprite *sprite;
  s32 i;

  for (i = 0; i < 2; i++) {
    sprite = &work->sprites[group][i];
    EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tiles);
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(target);
  }
  work->age[group] = 0;
  work->target = target;
  work->group ^= 1;
}

/// Task: draws the two pairs of swelling arcs on their actor: each pair swells
/// and shrinks over 32 frames (scale sin(age / 64 turn)) as it rises a pixel a
/// frame, the second arc of a pair upside down, one in front of the actor and
/// one behind.
void BattleEffect_UpdateArcEffect(void) {
  struct ArcWork *work = Ram_HeapSlots->arcEffect;
  struct Actor *target = work->target;
  s32 group;
  s32 position[3];
  s32 i;

  for (group = 0; group < 2; group++) {
    s32 scale = Trig_Sin(work->age[group] << 10);

    if (work->age[group] < 0)
      continue;
    if (work->age[group] < 32) {
      struct EffectSprite *sprite;
      s32 x, y;

      position[0] = target->x.fixed;
      position[1] = target->y.fixed + (work->age[group] << 16);
      position[2] = target->z.fixed;
      Camera_WorldToScreen(position);
      x = position[0];
      y = position[2];
      for (i = 0; i < 2; i++) {
        sprite = &work->sprites[group][i];
        sprite->x = x;
        sprite->y = y;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        if (i == 0) {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) - 1;
        } else {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) + 1;
          sprite->y -= px(1);
          sprite->scaleY = -sprite->scaleY;
        }
        EffectSprite_Draw(sprite);
      }
      work->age[group]++;
    }
  }
}

/// Starts the swelling arcs over a caster: loads their tiles and starts
/// BattleEffect_UpdateArcEffect with no pair showing.
void BattleEffect_StartArcEffect(void) {
  struct ArcEffectWork *work =
      HeapSlot_Alloc(HEAP_SLOT_ARC_EFFECT, sizeof(*work));

  work->resource = VramCache_FindFreeEntry();
  VramCache_Load(work->resource, 0x100, gBattleEffect_ArcTiles);
  work->group = 0;
  work->age[0] = 999;
  work->age[1] = 999;
  work->target = 0;
  Task_Add((s32)BattleEffect_UpdateArcEffect, TASK_PASS_VBLANK);
}

/// Ends the swelling arcs: frees their tiles and their work, and stops their
/// task.
void BattleEffect_EndArcEffect(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->arcEffect;

  VramCache_FreeEntry(work->resource);
  Task_Remove((u32)BattleEffect_UpdateArcEffect);
  HeapSlot_Release(HEAP_SLOT_ARC_EFFECT);
}

#define BOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Task of the Arrow, the field Psynergy of the unused ability "A??" (named
/// here for the bow it draws and the arrow it looses): the bow and the flaming
/// arrow.
void FieldPsynergy_UpdateArrow(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct BowWork *work = (struct BowWork *)Ram_HeapSlots->battleEffect;
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->canvasSprite;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  struct BowMotion *motion;
  struct EffectSprite *sprite;
  s32 pos[3];
  s32 objectIndex;
  s32 i;
  s32 dx;
  s32 dz;

again:
  switch (work->phase) {
  hit:
    work->done = 1;
    work->result = objectIndex;
    BOW_NEXT_PHASE(work);
    goto again;
  case 0:
    if (work->timer == 0) {
      work->sparking = 0;
      work->sparksRepeat = 1;
      work->arrowMoving = 0;
      work->bowGrowing = 1;
      work->frame = 999;
    } else if (work->bowGrowing == 0) {
      work->frame = 0;
      BOW_NEXT_PHASE(work);
    }
    break;
  case 1:
    if (work->timer == 0) {
      sprite = &work->arrow;
      sprite->scaleX = sprite->scaleY = Q16_ONE;
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset(0x2666, scene->angle8 + TRIG_TURN / 2, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      work->arrowMoving = 1;
    }
    if (work->timer == 20)
      work->sparking = 1;
    if (work->timer == 56) {
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset(px(5), scene->angle8, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      BOW_NEXT_PHASE(work);
    }
    break;
  case 2:
    motion = &work->arrowMotion;
    if (work->timer == 90) {
      BOW_NEXT_PHASE(work);
      break;
    }
    dx = work->target[0] - work->start[0];
    if (dx < 0)
      dx = work->start[0] - work->target[0];
    dz = motion->x - work->start[0];
    if (dz < 0)
      dz = work->start[0] - motion->x;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    dx = work->target[2] - work->start[2];
    if (dx < 0)
      dx = work->start[2] - work->target[2];
    dz = motion->z - work->start[2];
    if (dz < 0)
      dz = work->start[2] - motion->z;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    pos[0] = motion->x;
    pos[1] = motion->y;
    pos[2] = motion->z;
    if (MapGrid_CheckCollisionFar(scene->mainActor, pos) > 0) {
      work->done = 1;
      work->result = -1;
      work->phase++;
      work->timer = -1;
      goto again;
    }
    for (objectIndex = 0; objectIndex < 80; objectIndex++) {
      object = event->actors[objectIndex];
      if (object != NULL && object->script != NULL &&
          (object->collisionFlags & 1) && object != scene->mainActor &&
          FieldPsynergy_CheckOverlap(&motion->x, 2, &object->x.fixed,
                                     object->radius - 2) >= 0)
        goto hit;
    }
    break;
  case 3:
    if (work->done != 0) {
      work->arrowMoving = 0;
    } else {
      work->sparksRepeat = 0;
      work->bowShrinking = 1;
    }
    motion = &work->arrowMotion;
    pos[0] = pos[1] = pos[2] = 0;
    Vector_AddPolarOffset(px(2), scene->angle8, pos);
    motion->velocityX = pos[0];
    motion->velocityY = pos[1];
    motion->velocityZ = pos[2];
    BOW_NEXT_PHASE(work);
    break;
  case 4:
    sprite = &work->arrow;
    if (work->done != 0) {
      if (work->timer < 32) {
        s32 wobble[8];
        s32 fade[4];
        s32 w;

        memcpy(wobble, Data_080f0f34, sizeof(wobble));
        memcpy(fade, Data_080f0f54, sizeof(fade));
        w = wobble[work->timer & 7];
        sprite->angle = scene->angle8 + w * fade[work->timer / 8] / (TRIG_TURN);
      }
      if (work->timer == 32)
        work->bowShrinking = 1;
      if (work->timer == 50) {
        sprite->scaleX = sprite->scaleY = 0;
        work->arrowMoving = 1;
        work->sparksRepeat = 0;
        BOW_NEXT_PHASE(work);
      }
    } else {
      sprite->scaleX += -0x3000;
      sprite->scaleY = sprite->scaleX;
      if (sprite->scaleX < 0) {
        sprite->scaleX = sprite->scaleY = 0;
        BOW_NEXT_PHASE(work);
      }
    }
    break;
  case 5:
    if (work->bowShrinking == 0)
      work->phase = FIELD_PSYNERGY_FINISHED;
    break;
  }

  if (work->bowGrowing != 0) {
    sprite = &work->bow;
    sprite->scaleX += 0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += TRIG_TURN / 8;
    if (sprite->scaleX >= Q16_ONE) {
      sprite->scaleX = sprite->scaleY = Q16_ONE;
      sprite->angle = scene->angle8;
      work->bowGrowing = 0;
    }
  }
  if (work->bowShrinking != 0) {
    sprite = &work->bow;
    sprite->scaleX += -0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += TRIG_TURN / 8;
    if (sprite->scaleX <= 0x6665) {
      sprite->scaleX = sprite->scaleY = 0;
      work->bowShrinking = 0;
    }
  }

  // The bow's drawing pose, by frame: drawn back, then loosed and recovered.
  if (work->frame != 999) {
    work->frame++;
    sprite = &work->bow;
    switch (work->frame) {
    case 10:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 20:
      sprite->oam.index = work->bowTile + 64;
      break;
    case 54:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 62:
      sprite->oam.index = work->bowTile + 96;
      break;
    case 65:
      sprite->oam.index = work->bowTile + 128;
      break;
    case 68:
      sprite->oam.index = work->bowTile + 160;
      break;
    case 58:
    case 71:
      sprite->oam.index = work->bowTile;
      break;
    case 72:
      work->frame = 999;
      break;
    }
  }

  motion = &work->bowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  sprite = &work->bow;
  Camera_WorldToScreen(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];
  EffectSprite_Draw(sprite);
  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Camera_WorldToScreen(pos);
  sprite = &work->arrow;
  sprite->x = pos[0];
  sprite->y = pos[2];
  EffectSprite_Draw(sprite);
  if (work->arrowMoving != 0)
    BattleEffect_IntegrateVector3(&motion->x, 64, 0);

  if (work->sparking != 0) {
    motion = work->sparks;
    for (i = 9; i >= 0; i--) {
      if (motion->delay == 0) {
        s32 speed;
        s32 angle;

        speed = (Random16() & 255) + 256;
        angle = (Random16() & 0x1fff) + 0x7000;
        motion->x = px(60);
        motion->y = px(28);
        motion->velocityX = Trig_Cos(angle) * speed >> 6;
        motion->velocityY = Trig_Sin(angle) * speed >> 6;
      }
      if (motion->delay > 0) {
        s32 x = (s16)FIELD_PIXEL(motion->x);

        BattleEffect_DrawCanvasBlock(Data_080eda0c + (motion->delay & 1) * 64,
                                     x, (s16)FIELD_PIXEL(motion->y), 8, 8);
        BattleEffect_IntegrateVector2(&motion->x, 60, 0);
        /* FAKEMATCH(barrier): an empty asm after the step. */
        asm("");
      }
      motion->delay++;
      if (work->sparksRepeat == 1 && motion->delay == 10)
        motion->delay = 0;
      motion++;
    }
  }

  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Vector_AddPolarOffset(-px(36), scene->angle8, pos);
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIELD_PIXEL(pos[0]);
  canvas->y = (s16)FIELD_PIXEL(pos[2]);
  canvas->command = CANVAS_UPLOAD_AND_FADE3;
  work->timer++;
}

/// Effect 29 cast on a target: runs the effect. Nothing calls it; the
/// targeted dispatch has no case for animation 29.
void FieldPsynergy_ArrowOnTarget(void) { FieldPsynergy_Arrow(); }

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.

void FieldPsynergy_Arrow(void) {
  struct BowWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BowWork));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct BowMotion *motion;
  s32 pos[3];
  s32 arrowSlot;
  s32 bowSlot;
  s32 tile;
  u16 *palette;
  s32 blue;
  s32 shade;
  s32 i;

  FieldPsynergy_FadeToCastColor();
  // The spark pointer and the sprite pointer are one variable.
  sprite = (struct EffectSprite *)work->sparks;
  for (i = 0; i < 32; i++) {
    ((struct BowMotion *)sprite)->delay = -i;
    sprite = (struct EffectSprite *)((struct BowMotion *)sprite + 1);
  }

  work->tiles = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, 0x1800);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_BowDraw),
                      work->tiles);
  bowSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(bowSlot, 0x1800, work->tiles);
  work->bowTile = tile;
  sprite = &work->bow;
  EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X32, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainActor);
  // volatile keeps the depth store before the scale stores.
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainActor) + 1;
  *(volatile s32 *)&sprite->scaleX = 0x6666;
  *(volatile s32 *)&sprite->scaleY = 0x6666;
  sprite->angle = scene->angle8;
  pos[0] = scene->mainActor->x.fixed;
  pos[2] = scene->mainActor->z.fixed;
  motion = &work->bowMotion;
  Vector_AddPolarOffset(px(8), scene->angle8, pos);
  SET_POSITION(motion, pos[0], px(16), pos[2]);

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FlamingArrow),
                      work->tiles);
  arrowSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(arrowSlot, 0x100, work->tiles);
  sprite = &work->arrow;
  EffectSprite_Init(sprite, 12, 8, OBJ_SIZE_16X16, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainActor);
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainActor) + 2;
  *(volatile s32 *)&sprite->scaleX = 0;
  *(volatile s32 *)&sprite->scaleY = 0;
  sprite->angle = scene->angle8;
  pos[0] = scene->mainActor->x.fixed;
  pos[2] = scene->mainActor->z.fixed;
  motion = &work->arrowMotion;
  Vector_AddPolarOffset(px(18), scene->angle8, pos);
  SET_POSITION(motion, pos[0], px(16), pos[2]);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);

  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainActor);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainActor) + 3;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->command = 0;
  canvas->angle = scene->angle8;
  work->start[0] = scene->mainActor->x.fixed;
  work->start[1] = scene->mainActor->y.fixed;
  work->start[2] = scene->mainActor->z.fixed;
  blue = RGB_LEVEL_MAX << 10;
  work->target[0] = scene->mainActor->x.fixed;
  work->target[1] = scene->mainActor->y.fixed;
  work->target[2] = scene->mainActor->z.fixed;
  Vector_AddPolarOffset(px(80), scene->angle8, work->target);

  palette = (u16 *)&OBJ_PLTT_COLOR(14, 0);
  for (i = 0; i < 16; i++) {
    shade = i + 16;
    // A once-only block: the store stays ahead of the loop's increments.
    do {
      *palette = (shade << 5 | blue) | shade;
    } while (0);
    palette++;
  }

  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  work->bowGrowing = 0;
  work->bowShrinking = 0;
  work->done = 0;
  work->result = -1;
  work->phase = 0;
  work->timer = 0;
  Task_Add(FieldPsynergy_UpdateArrow, TASK_PASS_VBLANK);
  while (work->phase != FIELD_PSYNERGY_FINISHED)
    wait(1);
  Task_Remove(FieldPsynergy_UpdateArrow);
  if (work->done != 0 && work->result != -1)
    event->unkd30 = work->result;
  VramCache_FreeEntry(arrowSlot);
  VramCache_FreeEntry(bowSlot);
  FieldPsynergy_FadeFromCastColor();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Looks at 21 map cells around the effect owner, at the tile offsets of
/// gFieldPsynergy_NearbyCellOffsets (signed x, z byte pairs), on the owner's
/// layer. Each cell of kind 1, 2 or 3 that Field_MeasurePsynergyHeight accepts
/// for the effect's id is listed as a tile in the event work's point list at
/// 0xcc0, which ends with -1, -1; a kind-1 cell also stores its kind in the
/// heap slot 0x5c work's byte 0x1b34, and the last kind-2 and kind-3 cells are
/// kept in pixels, centred on their tile, at 0xd3c and 0xd40.
void FieldPsynergy_GatherNearbyCells(void) {
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct PointWork *event = Ram_HeapSlots->fieldWork;
  u8 *work = Ram_HeapSlots->battleEffect;
  s16 *points = event->points;
  struct Actor *owner = effect->mainActor;
  s32 position[3];
  s32 x, z;
  s32 kind;
  u32 i;
  s32 count = 0;

  work[0x1b34] = count;
  event->cycloneCarryX = -1;
  event->cycloneCarryZ = -1;
  event->cycloneLiftX = -1;
  event->cycloneLiftZ = -1;
  for (i = 0; i <= 20; i++) {
    x = owner->x.fixed + (gFieldPsynergy_NearbyCellOffsets[i * 2] << 20);
    z = owner->z.fixed + (gFieldPsynergy_NearbyCellOffsets[i * 2 + 1] << 20);
    kind = MapGrid_GetCycloneCellKind(x, z, owner->layer);
    // position[1] is never set: only x and z reach Field_MeasurePsynergyHeight.
    position[0] = x;
    position[2] = z;
    if (kind == 1 && Field_MeasurePsynergyHeight(effect->fieldPsynergy, owner,
                                                 position) != -1) {
      points[count * 2] = x / px(16);
      points[count * 2 + 1] = z / px(16);
      count++;
      work[0x1b34] = kind;
    }
    if (kind == 2 && Field_MeasurePsynergyHeight(effect->fieldPsynergy, owner,
                                                 position) != -1) {
      event->cycloneCarryX = FIELD_PIXEL(x);
      event->cycloneCarryZ = FIELD_PIXEL(z);
      event->cycloneCarryX = (event->cycloneCarryX & 0xfff0) + 8;
      event->cycloneCarryZ = (event->cycloneCarryZ & 0xfff0) + 8;
      points[count * 2] = x / px(16);
      points[count * 2 + 1] = z / px(16);
      count++;
    }
    if (kind == 3 && Field_MeasurePsynergyHeight(effect->fieldPsynergy, owner,
                                                 position) != -1) {
      event->cycloneLiftX = FIELD_PIXEL(x);
      event->cycloneLiftZ = FIELD_PIXEL(z);
      event->cycloneLiftX = (event->cycloneLiftX & 0xfff0) + 8;
      event->cycloneLiftZ = (event->cycloneLiftZ & 0xfff0) + 8;
      points[count * 2] = x / px(16);
      points[count * 2 + 1] = z / px(16);
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}

// Runs the field events an effect touched.

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

// Only for the compiler: see the FAKEMATCH tag in
// FieldPsynergy_RunTouchedEvents.
/// An index, as passed.
static inline s32 Index(s32 i) { return i; }

/// Resolves each cell in the event work's touched list to an event id at the
/// owner's layer, sorts the distinct ids and runs each one's event for the
/// effect's kind.
void FieldPsynergy_RunTouchedEvents(void) {
  struct TouchWork *work = Ram_HeapSlots->fieldEffectWork;
  struct TouchCell *cells = (struct TouchCell *)gFieldWork->psynergyCells;
  struct TouchOwner *owner = work->owner;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (i = 0; cells[i].x != -1; i++) {
    s16 id = MapGrid_GetCellBehavior(cells[i].x << 20, cells[i].z << 20,
                                     owner->layer);

    for (j = 0; j < count; j++) {
      if (ids[j] == id)
        break;
    }
    if (j == count)
      ids[count++] = id;
  }
  ids[count] = -1;

  if (count > 1) {
    for (i = 0; i < count - 1; i++) {
      for (j = i + 1; j < count; j++) {
        if (ids[i] > ids[j]) {
          s16 swap = ids[i];

          ids[i] = ids[j];
          ids[j] = swap;
        }
      }
    }
  }

  for (i = 0; ids[i] != -1; i++) {
    /* FAKEMATCH(wrapper): ids[Index(i)], through an inline that returns i. */
    s32 event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                          work->kind, ids[Index(i)]);

    if (event != 0)
      Field_RunPsynergyEvent(event, work->argument0, work->argument1);
  }
}

extern struct TouchCopies gSceneLocalState;

struct MapCell *MapGrid_GetCellAt(s32 x, s32 z, s32 layer);

/// Clears what Cyclone blew away: each cell of FieldWork.psynergyCells that
/// falls in the width by height window at (windowX, windowZ) of layerIndex
/// (counted from the layer's scroll and the leader's height) takes the cell
/// picture from the same place of the window at (sourceX, sourceZ), is opened
/// on layer 2, and is recorded (up to 64) in gSceneLocalState to be made again.
void TileCopies_CopyTouchedCells(s32 sourceX, s32 sourceZ, s32 width,
                                 s32 height, s32 windowX, s16 windowZ,
                                 s32 layerIndex) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 ownerHeight = FIELD_CELL(owner->groundHeight);
  struct TouchMap *map = (struct TouchMap *)Ram_HeapSlots->mapGrid;
  struct TouchLayer *layer = &map->layers[layerIndex];
  s32 i;

  for (i = 0; cell[i].x != -1; i++) {
    s32 x = cell[i].x - windowX;
    s32 z = cell[i].z - windowZ - ownerHeight;

    x += FIELD_CELL(layer->x);
    z += FIELD_CELL(layer->z);

    if (x >= 0 && x < width && z >= 0 && z < height) {
      s32 n;

      MapGrid_CopyCellPictureCellsRectFar(sourceX + x, sourceZ + z, 1, 1,
                                          windowX + x, windowZ + z);
      MapGrid_GetCellAt(cell[i].x << 20, cell[i].z << 20, 2)->collisionCode = 0;
      n = gSceneLocalState.count;
      if (n < 64) {
        gSceneLocalState.copies[n].toX = sourceX + x;
        gSceneLocalState.copies[n].toZ = sourceZ + z;
        gSceneLocalState.copies[n].fromX = windowX + x;
        gSceneLocalState.copies[n].fromZ = windowZ + z;
        gSceneLocalState.copies[n].cellX = cell[i].x;
        gSceneLocalState.copies[n].cellZ = cell[i].z;
        gSceneLocalState.count++;
      }
    }
  }
}
#undef BOW_NEXT_PHASE

/* VIEW(f0e2718): variable gSceneLocalState as extern u8 gSceneLocalState[];
 * f0e150c has extern struct TouchCopies gSceneLocalState; tried as the rest of
 * the object declares it, it does not compile in tla-ja:
 * games/tla/src/field/field_psynergy_radial_effects.c:1938: incompatible types
 * in initialization. */
extern u8 gData_02001000__f0e2718[] __asm__("gSceneLocalState");

/// For each entry of the event work's list at 0x174c, calls
/// MapGrid_CopyCellPictureCellsRectFar with its bytes and clears the collision
/// code of the cell at its position.
void TileCopies_Restore(void) {
  u8 *base = gData_02001000__f0e2718;
  s32 i;

  for (i = 0; i < (s8)gData_02001000__f0e2718[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    MapGrid_CopyCellPictureCellsRectFar(entry[0], entry[1], 1, 1, entry[2],
                                        entry[3]);
    MapGrid_GetCellAt(entry[4] << 20, entry[5] << 20, 2)->collisionCode = 0;
  }
}
