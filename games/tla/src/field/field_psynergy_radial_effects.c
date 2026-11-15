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

#include "sprite_ids.h"

#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "field/field_psynergy.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/scene_call.h"
#include "field/flag_call.h"
#include "gamepak/far_call/window.h"
#include "text/msg_ids.h"
#include "window/window.h"
#include "item_ids.h"
#include "ability_ids.h"

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
void BattleEffect_AgeAndCueAtEighty(struct Actor *object);

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
  struct Sprite *sprite = target->sprite;
  struct Vector3 position;
  struct Vector3 from;
  struct Vector3 to;
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
void BattleEffect_AgeAndCueAtEighty(struct Actor *object) {
  if (object->timer == 80) {
    object->update = 0;
    Sound_PlayFar(SE_WHOOSH);
  }
  object->timer++;
}

/// Field effect sprite callback (enum OutAndBackState): the particle heads for
/// a point 40 to 70 pixels from where it appeared, then for a point 4 pixels
/// round 8 pixels above the Psynergy's position on screen, and is freed on
/// arrival.
void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
  struct FieldEffectWork *effectWork;
  struct Vector3 pos;
  s16 angle;
  s32 state;

  effectWork = Ram_HeapSlots->fieldEffectWork;
top:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + px(40), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = px(4);
    effect->maxSpeed = px(4);
    effect->limitTurnStep = state;
    effect->state++;
    return;
  }
  if (state == OUT_AND_BACK_FLYING_OUT) {
    if (FieldParticle_HasPendingTarget(effect) != 0)
      return;
    effect->state++;
    goto top;
  }
  if (state == OUT_AND_BACK_AIM_BACK) {
    pos.x = effectWork->x;
    pos.y = effectWork->y + px(8);
    pos.z = effectWork->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(px(4), Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = TRIG_TURN / 16;
    effect->limitTurnStep = 1;
    effect->state++;
    return;
  }
  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// Pauses the target (effectWork->child) and runs FieldPsynergy_Halt.
void FieldPsynergy_HaltOnTarget(s32 unused) {
  struct Actor *target =
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->child;

  target->paused = 1;
  FieldPsynergy_Halt();
}

/// Throws an actor from the caster to the Psynergy's position over 10 frames
/// and scatters it into 16 particles.
void FieldPsynergy_Halt(void) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Actor *caster = effectWork->mainActor;
  struct Actor *orb;
  struct FieldParticle *effect;
  struct Vector3 pos;
  struct Vector3 from;
  struct Vector3 to;
  s32 step;
  s32 i;

  effectWork->y = caster->y.fixed;
  orb = Actor_Spawn(SPRITE_STOPWATCH, 0, 0, 0);
  step = 0;
  Actor_SelectAnimationFar(orb, 0);
  if (orb == 0)
    return;

  // From 16 pixels above the caster to 8 pixels above the Psynergy's position.
  FieldPsynergy_FadeToCastColor();
  from.x = caster->x.fixed;
  from.y = caster->y.fixed + px(16);
  from.z = caster->z.fixed;
  to.x = effectWork->x;
  to.y = effectWork->y + px(8);
  to.z = effectWork->z;
  // Steps 0 to 10: the orb arrives on the 11th frame, grown to full size.
  for (;;) {
    s32 steps = 11;
    s32 scale;

    SET_ACTOR_POSITION(orb, from.x + (to.x - from.x) * step / 10,
                       from.y + (to.y - from.y) * step / 10,
                       from.z + (to.z - from.z) * step / 10);
    scale = FieldEffect_Interpolate(Q16_ONE, Q16_ONE / 4,
                                    step); // from 0.25 to full
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  // Three low tones and a high one, 10 frames apart, as the orb changes to
  // its second animation.
  wait(5);
  Actor_SelectAnimationFar(orb, 1);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_HIGH);

  // Particles 4 pixels round it fly out, then come back to the Psynergy's
  // position.
  effect = &effectWork->particles[0];
  for (i = 0; i < 16; i++) {
    pos.x = orb->x.fixed;
    pos.y = orb->y.fixed + px(8);
    pos.z = orb->z.fixed;
    Camera_WorldToScreen((struct Vector3 *)&pos.x);
    Vector_AddPolarOffset(px(4), Random16(), &pos.x);
    FieldParticle_Initialize(effect, SPRITE_SPARKLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateHaltParticle);
    Animation_ApplyChildValuesToRecordFar(effect->object, SPRITE_COLOR_WHITE);
    effect++;
  }

  pos.x = orb->x.fixed;
  pos.y = orb->y.fixed + px(8);
  pos.z = orb->z.fixed;
  wait(8);
  Actor_ReleaseFar(orb);
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
  struct Vector3 spawn;
  struct Vector3 origin;
  struct Vector3 target;
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
  Actor_SetSpriteColorModeFar(object, SPRITE_COLOR_GRB);
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
  object = Actor_Spawn(SPRITE_LASH_HAND, 0, 0, 0);
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
      sprite->oam.fullColor = 1;
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
void FieldEffect_UpdateSpinAngle(struct Actor *actor) {
  u32 step;

  step = actor->timer * 80;
  actor->facing = actor->facing + step + FACING_STEP;
  if (step < FACING_STEP)
    actor->timer = actor->timer + 1;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
void FieldEffect_UpdateRetreatCaster(struct Actor *actor) {
  u32 scale;

  actor->scaleY -= 640;
  scale = actor->scaleX - 640;
  actor->facing += TRIG_TURN / 8;
  actor->y.fixed += px(1);
  actor->scaleX = scale;
  if ((s32)scale < Q16_ONE * 3 / 16)
    actor->spriteStorageKind = ACTOR_SPRITE_NONE;
}

/// Field effect sprite callback: a particle flies 120 pixels from the main
/// actor's screen position (8 pixels lower) in a direction within 1/8 turn of
/// up the screen, at up to 5 pixels a frame, and starts again from the main
/// actor on arrival until it takes its last flight.
void FieldEffect_UpdateRadialLaunch(struct FieldParticle *effect) {
  struct Vector3 pos;
  struct Actor *main;
  s32 state;
  u32 spread;

  main = ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  state = effect->state;

  if (state == RETREAT_PARTICLE_AIM) {
    pos.x = main->x.fixed;
    pos.y = main->y.fixed;
    pos.z = main->z.fixed;
    Camera_WorldToScreen(&pos);

    effect->x = effect->originX = pos.x;
    effect->z = effect->originZ = pos.z + px(8);

    spread = Random16();
    Vector_AddPolarOffset(px(120),
                          ((spread << 13) >> 16) - ((Random16() << 13) >> 16) +
                              FACING_NORTH,
                          &pos);

    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = px(5);
    effect->maxSpeed = px(5);
    effect->limitTurnStep = state;
    effect->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlayFar(SE_GLITTER);
  } else if (state == RETREAT_PARTICLE_FLYING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == RETREAT_PARTICLE_LAST_FLIGHT) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}

/// Spins the main actor and shrinks it while 24 particles fly out, one a frame.
void FieldPsynergy_Retreat(void) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Actor *actor = effectWork->mainActor;
  struct Vector3 pos;
  struct FieldParticle *effect;
  struct FieldParticle *effects;
  u32 i;

  Event_Start();
  FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldPsynergy_FadeToCastColor();
  wait(10);
  Actor_FaceDirectionById(effectWork->firstActorId, FACING_SOUTH, 0);
  wait(30);
  actor->update = (void (*)(void))Actor_ApplyRandomChildValues;
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_SelectAnimationFar(actor, ANIM_LEADER_ARMS_RAISED);
  wait(40);

  // Start spinning.
  Sound_PlayFar(SE_SHIMMER);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  Actor_SelectAnimationFar(actor, 3);
  actor->update = (void (*)(void))FieldEffect_UpdateSpinAngle;
  actor->timer = 0;
  wait(70);

  // Start shrinking, and send particles from where it is on screen.
  Actor_SetSpriteFlagsFar(actor, 0);
  actor->motionFlags = 0;
  actor->update = (void (*)(void))FieldEffect_UpdateRetreatCaster;
  actor->targetX = TARGET_UNSET;
  SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Camera_WorldToScreen(&pos);
  i = 0;
  effect = effectWork->particles;
  effects = effect;
  do {
    s32 scale;

    FieldParticle_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRadialLaunch);
    FieldParticle_SetActorMode(effect, 7);
    Animation_ApplyChildValuesToRecordFar(effect->object, Random_Below(7));
    scale = (Random16() >> 1) + Q16_ONE * 6 / 5; // 1.2 to 1.7
    effect->scaleY = scale;
    effect->scaleX = scale;
    wait(1);
    i++;
    effect++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  wait(70);

  // The flying particles vanish once they arrive.
  i = 0;
  effect = effects;
  do {
    if (effect->active != 0)
      effect->state = RETREAT_PARTICLE_LAST_FLIGHT;
    i++;
    effect++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  wait(40);
  FieldPsynergy_FadeFromCastColor();
  actor->scaleX = 0;
  actor->y.fixed = actor->groundHeight;
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

  limit = particle->groundHeight + px(10);
  source = particle->watchedActor;
  if (gMainRoutineWork.mapId == SCENE_CLEAR)
    limit = particle->groundHeight + px(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + Q16_ONE * 3 / 64;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - px(2);
  particle->z.fixed = source->z.fixed + ((Q16_ONE - scale) * 5) + px(9);
}

/// As FieldEffect_UpdateDescendingParticlePositiveArc for the arc drawn upside
/// down, 16 pixels in front of the actor and up to 5 pixels nearer.
void FieldEffect_UpdateDescendingParticleNegativeArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->groundHeight + px(10);
  source = particle->watchedActor;
  if (gMainRoutineWork.mapId == SCENE_CLEAR)
    limit = particle->groundHeight + px(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + Q16_ONE * 3 / 64;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - px(2);
  particle->z.fixed = source->z.fixed - ((Q16_ONE - scale) * 5) + px(16);
}

/// Spawns a pair of falling arcs (one upside down) on arc, 64 pixels above it,
/// at 0.4 of full size, drawn with the tiles in effectWork->arcTileSlot.
void FieldEffect_SpawnDescendingArcParticles(struct Actor *source) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Actor *leader = effectWork->mainActor;
  struct Actor *actors[2];
  struct Actor *actor;
  struct DescendingArcSprite *sprite;
  s32 arcIndex;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    actor = Actor_CreateFar(SPRITE_GOLDEN_RINGS, source->x.fixed,
                            source->y.fixed + px(64), source->z.fixed);
    actors[arcIndex] = actor;
    if (actor == NULL)
      continue;
    actor->groundHeight = source->groundHeight;
    sprite = (struct DescendingArcSprite *)actor->sprite;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->watchedActor = source;
    actor->scaleY = Q16_ONE * 2 / 5;
    actor->scaleX = Q16_ONE * 2 / 5;
    if (sprite == NULL)
      continue;
    Sprite_StartAnimationFar((struct Sprite *)sprite, 0);
    sprite->flags = 0;
    VramCache_FreeEntry(sprite->slot);
    sprite->slot = effectWork->arcTileSlot;
    sprite->retainCacheEntry = 1;
    sprite->attr.half.tile = gVramCacheEntries[sprite->slot].offset >> 5;
    sprite->fullColor = 0;
    sprite->shape = 1;
    sprite->size = 2;
    sprite->firstEntry->picture = 0;
  }
  // BUG: failed allocations above leave a missing arc or sprite here.
  actors[0]->update =
      (void (*)(void))FieldEffect_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)actors[0]->sprite)->attr.byte.priority = 0;
  actors[1]->update =
      (void (*)(void))FieldEffect_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)actors[1]->sprite)->attr.byte.priority =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.priority;
}

/// The caster during Avoid (FieldPsynergy_Avoid): sheds a pair of falling
/// arcs every 5 frames (7 in scene Clear), and spins in phase 1, 3/64 turn a
/// frame.
void FieldEffect_UpdateAvoidCaster(struct Actor *caster) {
  s16 phase;
  s32 elapsedFrames;

  phase = caster->arcPhase;
  elapsedFrames = caster->arcFrames++;

  if (gMainRoutineWork.mapId == SCENE_CLEAR) {
    if (elapsedFrames % 7 == 0)
      FieldEffect_SpawnDescendingArcParticles(caster);
  } else if (elapsedFrames % 5 == 0) {
    FieldEffect_SpawnDescendingArcParticles(caster);
  }

  if (phase == 1)
    caster->facing += TRIG_TURN * 3 / 64;
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
    MessageArgument_PushFar(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
  } else {
    MessageArgument_PushFar(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)&MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
  }
  WorldMap_ReloadPendingAreaGraphics();
}

/// Avoid: the caster spins and flashes, then blinks, and encounters are kept
/// away until Avoid wears off.
void FieldPsynergy_Avoid(void) {
  struct FieldEffectWork *effectWork;
  struct Actor *actor;
  struct Sprite *group;
  struct SpriteAnimation *entry;
  const u8 *tiles;
  u32 angle;
  s32 flags;
  s32 slot;
  s32 count;
  s32 phase;

  effectWork = Ram_HeapSlots->fieldEffectWork;
  actor = effectWork->mainActor;
  group = actor->sprite;
  entry = group->entries[0];
  angle = actor->facing;
  flags = group->flags;
  slot = VramCache_FindFreeEntry();
  // The arcs' tiles (256 bytes) stay loaded until the Psynergy's end; the
  // distance is 2,400 pixels in 16.16.
  effectWork->arcTileSlot = slot;
  tiles = gFieldEffect_ArcSparkTiles;
  VramCache_Load((s16)slot, 256, tiles);
  gMainRoutineWork.avoidDistance = px(2400);
  gMainRoutineWork.avoidByItem = GameFlag_Get(FLAG_ITEM_USE);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  actor->update = (void (*)(void))FieldEffect_UpdateAvoidCaster;
  actor->arcPhase = 0;
  actor->arcFrames = 0;
  Sound_PlayFar(SE_SPIN);
  wait(15);
  actor->arcPhase = 1;
  wait(10);
  // Flash white for 2 frames in every 5.
  for (count = 0; count < 20; count++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    group->dirty = 1;
    wait(2);
    entry->colorMode = SPRITE_COLOR_NORMAL;
    group->dirty = 1;
    group->flags = SPRITE_FLAG_SHADOW;
    wait(3);
  }
  actor->update = NULL;
  actor->facing = angle;

  // For 70 frames, blink its outline beside its own flags on a five-frame
  // cycle.
  for (count = 0; count < 70; count++) {
    phase = count % 5;
    if (phase == 0) {
      group->dirty = 1;
      group->flags = flags | SPRITE_FLAG_OUTLINED;
    } else if (phase == 2) {
      group->dirty = 1;
      group->flags = flags;
      entry->colorMode = SPRITE_COLOR_NORMAL;
    }
    if (count == 15)
      Sound_PlayFar(SE_NOTE_MID);
    wait(1);
  }
  group->flags = flags;
  group->dirty = 1;
  {
    u32 cleared = SPRITE_COLOR_NORMAL;

    entry->colorMode = cleared;
  }
  wait(1);

  VramCache_FreeEntry(effectWork->arcTileSlot);
  FieldMessage_ShowFar((s32)&MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
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
    Actor_SetSpriteColorModeFar(object, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorModeFar(object, SPRITE_COLOR_NORMAL);
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
    Actor_SetSpriteColorModeFar(object, SPRITE_COLOR_NORMAL);
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
    sprite->oam.fullColor = 0;
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

        memcpy(wobble, gArrow_WobbleAngles, sizeof(wobble));
        memcpy(fade, gArrow_WobbleFades, sizeof(fade));
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
        s32 y = (s16)FIELD_PIXEL(motion->y);

        BattleEffect_DrawCanvasBlock(
            gArrow_GlintPixels + (motion->delay & 1) * 64, x, y, 8, 8);
        BattleEffect_IntegrateVector2(&motion->x, 60, 0);
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
  canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
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
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainActor);
  // volatile keeps the depth store before the scale stores.
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainActor) + 1;
  *(volatile s32 *)&sprite->scaleX = Q16_ONE * 2 / 5;
  *(volatile s32 *)&sprite->scaleY = Q16_ONE * 2 / 5;
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
  sprite->oam.fullColor = 1;
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
    x = owner->x.fixed +
        FIELD_CELL_ORIGIN(gFieldPsynergy_NearbyCellOffsets[i * 2]);
    z = owner->z.fixed +
        FIELD_CELL_ORIGIN(gFieldPsynergy_NearbyCellOffsets[i * 2 + 1]);
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

/// Resolves each cell in the event work's touched list to an event id at the
/// owner's layer, sorts the distinct ids and runs each one's event for the
/// effect's kind.
void FieldPsynergy_RunTouchedEvents(void) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct TouchCell *cells = (struct TouchCell *)gFieldWork->psynergyCells;
  struct Actor *caster = effectWork->mainActor;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (i = 0; cells[i].x != -1; i++) {
    s16 id =
        MapGrid_GetCellBehavior(FIELD_CELL_ORIGIN(cells[i].x),
                                FIELD_CELL_ORIGIN(cells[i].z), caster->layer);

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
    s32 id = ids[i];
    s32 event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                          effectWork->fieldPsynergy, id);

    if (event != 0)
      Field_RunPsynergyEvent(event, effectWork->firstActorId,
                             effectWork->secondActorId);
  }
}

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
      MapGrid_GetCellAt(FIELD_CELL_ORIGIN(cell[i].x),
                        FIELD_CELL_ORIGIN(cell[i].z), 2)
          ->collisionCode = 0;
      n = gTouchCopies.count;
      if (n < 64) {
        gTouchCopies.copies[n].toX = sourceX + x;
        gTouchCopies.copies[n].toZ = sourceZ + z;
        gTouchCopies.copies[n].fromX = windowX + x;
        gTouchCopies.copies[n].fromZ = windowZ + z;
        gTouchCopies.copies[n].cellX = cell[i].x;
        gTouchCopies.copies[n].cellZ = cell[i].z;
        gTouchCopies.count++;
      }
    }
  }
}
#undef BOW_NEXT_PHASE

/* VIEW(f0e2718): variable gSceneLocalState as u8 gTileCopies_RestoreState[];
 * system/ram_buffer.h has u8 gSceneLocalState[]; tried as the one declaration,
 * the restore's address is shared with the copies above and its loop moves (29
 * lines in tla-en). */
extern u8 gTileCopies_RestoreState[] __asm__("gSceneLocalState");

/// For each entry of the event work's list at 0x174c, calls
/// MapGrid_CopyCellPictureCellsRectFar with its bytes and clears the collision
/// code of the cell at its position.
void TileCopies_Restore(void) {
  u8 *base = gTileCopies_RestoreState;
  s32 i;

  for (i = 0; i < (s8)gTileCopies_RestoreState[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    MapGrid_CopyCellPictureCellsRectFar(entry[0], entry[1], 1, 1, entry[2],
                                        entry[3]);
    MapGrid_GetCellAt(FIELD_CELL_ORIGIN(entry[4]), FIELD_CELL_ORIGIN(entry[5]),
                      2)
        ->collisionCode = 0;
  }
}
