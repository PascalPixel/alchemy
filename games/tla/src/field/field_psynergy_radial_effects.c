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
#include "field/screen_transition.h"
#include "gamepak/far_call/window.h"
#include "text/msg_ids.h"
#include "window/window.h"
#include "item_ids.h"
#include "ability_ids.h"

extern const s32 gFieldEffect_ReleaseScript[];

/// Whirlwind: its leaves, their circling and wandering, the target's shrinking
/// and the orb's flight, in frames.
enum {
  WHIRLWIND_LEAF_COUNT = 16,
  WHIRLWIND_ORBIT_RADIUS = toFIXED(8),
  WHIRLWIND_ORBIT_ANGLE_STEP = TRIG_TURN / 32,
  WHIRLWIND_ORBIT_FRAMES = 121,
  WHIRLWIND_WANDER_FRAMES = 101,
  WHIRLWIND_TARGET_SCALE_STEP = FIXED_ONE / 512,
  WHIRLWIND_TARGET_MIN_SCALE = FIXED_ONE / 2,
  WHIRLWIND_TRAVEL_FRAMES = 10
};

/// The radial ring's particles, a 16th of a turn apart; the last goes where the
/// first went.
#define RADIAL_RING_STEPS 16

/// Spawns 17 particles that spread from origin in a flat ring, their x velocity
/// 1.5 times their z velocity, so the ring is wider than deep as the field is
/// drawn.
void FieldEffect_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions options;
  FIXED velocity[3];
  FIXED sine;
  s32 angle;
  u32 i;

  options.palette = 0;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i <= RADIAL_RING_STEPS; i++) {
    angle = i * (TRIG_TURN / RADIAL_RING_STEPS);
    velocity[0] = Trig_Cos(angle) * 3 / 2;
    velocity[1] = 0;
    sine = Trig_Sin(angle);
    velocity[2] = sine;
    FieldEffect_SpawnResident(
        origin->x, origin->y, origin->z, velocity[0], velocity[1], sine,
        EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
        &options);
  }
}

/// Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame, now
/// and then turning sharply; after 101 frames it hands over to the shared
/// particle script.
void FieldEffect_WanderAroundAnchor(struct Actor *effect) {
  FIXED radius;
  s32 angle;
  FIXED dx;
  FIXED dz;

  radius = Random16() + toFIXED(2);
  angle = effect->facing;
  dx = Iwram_MulFX(radius, Trig_Cos(angle));
  dz = Iwram_MulFX(radius, Trig_Sin(angle));
  effect->x.fixed += dx;
  effect->z.fixed += dz;
  // A small backwards drift, a 4096th of a turn a frame.
  effect->facing += -(TRIG_TURN / 4096);
  if (effect->turnFrames != 0) {
    effect->turnFrames--;
    effect->facing += TRIG_TURN / 32;
  } else if ((Random16() << 5) >> 16 == 0) {            // one chance in 32
    effect->turnFrames = ((Random16() << 4) >> 16) + 8; // 8 to 23 frames
  }
  if (++effect->swirlFrames == WHIRLWIND_WANDER_FRAMES)
    Actor_StartScriptFar(effect, gFieldEffect_ReleaseScript);
}

/// Per-frame update of a Whirlwind particle: circles the anchor at 8 pixels,
/// a 32nd of a turn a frame; after 121 frames it starts to wander
/// (FieldEffect_WanderAroundAnchor) with upward speed and gravity. As The
/// Broken Seal's FieldEffect_CircleAnchor.
void FieldEffect_CircleAnchor(struct Actor *effect) {
  struct Actor *anchor;
  s32 angle;
  FIXED radius;
  FIXED dx;
  FIXED dz;

  anchor = effect->watchedActor;
  radius = WHIRLWIND_ORBIT_RADIUS;
  angle = effect->facing;
  dx = Iwram_MulFX(radius, Trig_Cos(angle));
  dz = Iwram_MulFX(radius, Trig_Sin(angle));
  effect->x.fixed = anchor->x.fixed + dx;
  effect->z.fixed = anchor->z.fixed + dz;
  effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
  if (++effect->swirlFrames == WHIRLWIND_ORBIT_FRAMES) {
    effect->update = (ObjectUpdate)FieldEffect_WanderAroundAnchor;
    effect->swirlFrames = 0;
    effect->turnFrames = 0;
    effect->gravity = toFIXED(1) / 10;
    effect->velocityY = toFIXED(3);
    effect->facing = Random16();
  }
}

/// Per-frame update of Whirlwind's target: it shrinks a 512th a frame and,
/// below half its size, is put away at the map's origin.
void Whirlwind_UpdateTarget(struct Actor *actor) {
  s32 scale;

  scale = actor->scaleX - WHIRLWIND_TARGET_SCALE_STEP;
  actor->scaleY = scale;
  actor->scaleX = scale;
  if (scale < WHIRLWIND_TARGET_MIN_SCALE) {
    Actor_SetPositionAndResetMotionFar(actor, 0, 0, 0);
    actor->update = 0;
  }
}

/// Runs Whirlwind cast on a target, using the target already stored in the
/// field effect.
void FieldPsynergy_WhirlwindOnTarget(s32 unused) { FieldPsynergy_Whirlwind(); }

void Vector_AddPolarOffset(FIXED radius, s32 angle, s32 *position);
void Whirlwind_UpdateSpentOrb(struct Actor *actor);

/// Whirlwind: a whirlwind orb flies from the caster to the spot over ten
/// frames, growing from a quarter size. With nothing to blow it whooshes away;
/// on a target, leaves circle it while the target shrinks away; on sprouting
/// leaves the leaves scatter, and there or on a seedling the vine swings
/// (HangingVine_Swing).
void FieldPsynergy_Whirlwind(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct Actor *caster = scene->caster;
  struct Actor *target = scene->target;
  struct Sprite *sprite = target->sprite;
  struct Vector3 position;
  struct Vector3 from;
  struct Vector3 to;
  struct Actor *orb;
  struct Actor *leaf;
  s32 kind = ORB_PLAIN;
  s32 step;
  s32 i;
  s32 frames = WHIRLWIND_TRAVEL_FRAMES + 1;

  if (scene->sceneVariant != 0)
    return;
  if (scene->aimAhead == 0) {
    s32 animation;

    kind = ORB_ORBITING;
    animation = sprite->entries[0]->animationId;
    if (animation == SPRITE_SPROUTING_LEAVES)
      kind = ORB_WANDER;
    if (animation == SPRITE_SEEDLING)
      kind = ORB_HANDOFF;
  }

  SET_POSITION(&from, caster->x.fixed, caster->y.fixed, caster->z.fixed);
  SET_POSITION(&to, scene->x, scene->y - toFIXED(4), scene->z);
  orb = Actor_Spawn(SPRITE_WHIRLWIND, 0, 0, 0);
  if (orb == 0)
    return;

  FieldPsynergy_FadeToCastColor();
  Actor_SelectAnimationFar(orb, 2);
  step = 0;
  do {
    FIXED scale;

    SET_ACTOR_POSITION(
        orb, from.x + (to.x - from.x) * step / WHIRLWIND_TRAVEL_FRAMES,
        from.y + (to.y - from.y) * step / WHIRLWIND_TRAVEL_FRAMES,
        from.z + (to.z - from.z) * step / WHIRLWIND_TRAVEL_FRAMES);
    scale = step * (FIXED_ONE * 21 / 20) / WHIRLWIND_TRAVEL_FRAMES +
            FIXED_ONE / 4; // from 0.25 to 1.3
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    step++;
  } while (step < frames);
  orb->scaleX = FIXED_ONE * 17 / 10;
  orb->scaleY = FIXED_ONE * 13 / 10;
  Sound_PlayFar(SE_RUSH);

  if (kind == ORB_PLAIN) {
    Actor_SelectAnimationFar(orb, 1);
    wait(30);
    Sound_PlayFar(SE_WHOOSH);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  } else if (kind == ORB_ORBITING) {
    // Sixteen leaves, one every six frames, 4 pixels above the orb and 0.8
    // higher each, 3 to 8 pixels out, circle it while the target shrinks.
    wait(20);
    if (target != 0)
      target->update = (ObjectUpdate)Whirlwind_UpdateTarget;
    for (i = 0; i < WHIRLWIND_LEAF_COUNT; i++) {
      s32 distance;

      SET_POSITION(&position, orb->x.fixed,
                   orb->y.fixed + i * (toFIXED(4) / 5) + toFIXED(4),
                   orb->z.fixed);
      distance = Random16() * 5 + toFIXED(3);
      Vector_AddPolarOffset(distance, Random16(), &position);
      leaf = Actor_Spawn(SPRITE_LEAF, position.x, position.y, position.z);
      if (leaf != 0) {
        leaf->update = (ObjectUpdate)FieldEffect_CircleAnchor;
        leaf->watchedActor = orb;
        leaf->swirlFrames = 0;
        leaf->turnFrames = 0;
        leaf->facing = Random16();
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
      // Sixteen leaves that wander off the orb, the first handed to the shared
      // particle script soonest.
      for (i = 0; i < WHIRLWIND_LEAF_COUNT; i++) {
        s32 distance;

        SET_POSITION(&position, orb->x.fixed,
                     orb->y.fixed + i * (toFIXED(4) / 5) + toFIXED(4),
                     orb->z.fixed);
        distance = Random16() * 5;
        distance += toFIXED(3);
        Vector_AddPolarOffset(distance, Random16(), &position);
        leaf = Actor_Spawn(SPRITE_LEAF, position.x, position.y, position.z);
        if (leaf != 0) {
          leaf->update = (ObjectUpdate)FieldEffect_WanderAroundAnchor;
          leaf->swirlFrames = 20 + i * -2;
          leaf->turnFrames = 0;
          leaf->gravity = toFIXED(1) / 10;
          leaf->velocityY = toFIXED(3);
          leaf->facing = Random16();
        }
      }
    }
    BattleEffect_SetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
    FieldPsynergy_FadeFromCastColor();
    orb->timer = 0;
    orb->update = (ObjectUpdate)Whirlwind_UpdateSpentOrb;
    HangingVine_Swing(scene->targetId);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  }
  FieldPsynergy_FadeFromCastColor();
}

/// Per-frame update of Whirlwind's orb once it has done its work: it whooshes
/// on its 80th frame.
void Whirlwind_UpdateSpentOrb(struct Actor *actor) {
  if (actor->timer == 80) {
    actor->update = 0;
    Sound_PlayFar(SE_WHOOSH);
  }
  actor->timer++;
}

/// Field effect sprite callback (enum OutAndBackState): the particle heads for
/// a point 40 to 70 pixels from where it appeared, then for a point 4 pixels
/// round 8 pixels above the Psynergy's position on screen, and is freed on
/// arrival.
void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
  struct FieldEffectState *effectState;
  struct Vector3 pos;
  s16 angle;
  s32 state;

  effectState = Ram_HeapSlots->fieldEffectState;
top:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + toFIXED(40), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = toFIXED(4);
    effect->speedMax = toFIXED(4);
    effect->turnStepLimited = state;
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
    pos.x = effectState->x;
    pos.y = effectState->y + toFIXED(8);
    pos.z = effectState->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(toFIXED(4), Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->turnStepMax = TRIG_TURN / 16;
    effect->turnStepLimited = 1;
    effect->state++;
    return;
  }
  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// Pauses the target (effectState->child) and runs FieldPsynergy_Halt.
void FieldPsynergy_HaltOnTarget(s32 unused) {
  struct Actor *target =
      ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->target;

  target->paused = 1;
  FieldPsynergy_Halt();
}

/// A thrown stopwatch's or hand's steps, a tenth of the way a frame: it
/// arrives on the 11th frame.
#define CAST_THROW_STEPS 10
#define HALT_PARTICLES 16

/// Halt: throws a stopwatch (SPRITE_STOPWATCH) from the caster to the
/// Psynergy's position over CAST_THROW_STEPS frames, ticks three low tones
/// and a high one, and scatters it into HALT_PARTICLES particles.
void FieldPsynergy_Halt(void) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;
  struct Actor *caster = effectState->caster;
  struct Actor *orb;
  struct FieldParticle *effect;
  struct Vector3 pos;
  struct Vector3 from;
  struct Vector3 to;
  s32 step;
  s32 i;

  effectState->y = caster->y.fixed;
  orb = Actor_Spawn(SPRITE_STOPWATCH, 0, 0, 0);
  step = 0;
  Actor_SelectAnimationFar(orb, 0);
  if (orb == 0)
    return;

  // From 16 pixels above the caster to 8 pixels above the Psynergy's position.
  FieldPsynergy_FadeToCastColor();
  from.x = caster->x.fixed;
  from.y = caster->y.fixed + toFIXED(16);
  from.z = caster->z.fixed;
  to.x = effectState->x;
  to.y = effectState->y + toFIXED(8);
  to.z = effectState->z;
  // Grown to full size as it arrives.
  for (;;) {
    s32 steps = CAST_THROW_STEPS + 1;
    s32 scale;

    SET_ACTOR_POSITION(orb, from.x + (to.x - from.x) * step / CAST_THROW_STEPS,
                       from.y + (to.y - from.y) * step / CAST_THROW_STEPS,
                       from.z + (to.z - from.z) * step / CAST_THROW_STEPS);
    scale = FieldEffect_Interpolate(FIXED_ONE, FIXED_ONE / 4,
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
  effect = &effectState->particles[0];
  for (i = 0; i < HALT_PARTICLES; i++) {
    pos.x = orb->x.fixed;
    pos.y = orb->y.fixed + toFIXED(8);
    pos.z = orb->z.fixed;
    Camera_WorldToScreen((struct Vector3 *)&pos.x);
    Vector_AddPolarOffset(toFIXED(4), Random16(), &pos.x);
    FieldParticle_Initialize(effect, SPRITE_SPARKLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateHaltParticle);
    Sprite_SetAnimationColorModesFar(effect->sprite, SPRITE_COLOR_WHITE);
    effect++;
  }

  pos.x = orb->x.fixed;
  pos.y = orb->y.fixed + toFIXED(8);
  pos.z = orb->z.fixed;
  wait(8);
  Actor_ReleaseFar(orb);
  wait(4);
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

/// Per-frame update of an actor held by Halt: its sprite flickers through the
/// colour modes of gHalt_FlickerColorModes at random.
void FieldPsynergy_UpdateHaltedActor(struct Actor *actor) {
  const s8 *table = gHalt_FlickerColorModes;
  s32 random = Random16();
  Actor_SetSpriteColorModeFar(actor, table[(u32)(random * 8) >> 16]);
}

/// Holds an actor still under Halt, keeping its per-frame update and its first
/// animation's colour mode in gMainRoutineState for
/// FieldPsynergy_ResumeHaltedActor.
void FieldPsynergy_HaltActor(s32 actorId) {
  struct Actor *actor;
  struct SpriteAnimation *entry;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    gMainRoutineState.pausedUpdate = actor->update;
    gMainRoutineState.pausedColorMode = 0;
    if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
      entry = ((struct Sprite *)actor->sprite)->entries[0];
      if (entry != NULL) {
        gMainRoutineState.pausedColorMode = entry->colorMode;
      }
    }
    actor->update = (ObjectUpdate)FieldPsynergy_UpdateHaltedActor;
    actor->paused = 1;
    Actor_SetAnimationSpeedFar(actor, 0);
  }
}

/// Lets a halted actor move again, with the update and colour mode kept by
/// FieldPsynergy_HaltActor.
void FieldPsynergy_ResumeHaltedActor(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    if (actor->update == (ObjectUpdate)FieldPsynergy_UpdateHaltedActor) {
      actor->update = gMainRoutineState.pausedUpdate;
      gMainRoutineState.pausedUpdate = NULL;
      Actor_SetSpriteColorModeFar(actor, (s8)gMainRoutineState.pausedColorMode);
    }
    actor->paused = 0;
    Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL);
  }
}

s32 Speaker_GetHaltedLines(s32 id);

/// Returns no target (-1) when Halt passes actor id over (its sprite has no
/// halted lines, SPEAKER_HALTED_LINES_NONE), else id.
s32 FieldPsynergy_FilterHaltTarget(s32 id) {
  if (Speaker_GetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
    return -1;
  return id;
}

/// Catch cast on a target: runs FieldPsynergy_Catch.
void FieldPsynergy_CatchOnTarget(s32 unused) { FieldPsynergy_Catch(); }

/// The value step tenths of the way from from to to.
static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
  return from + (to - from) * step / CAST_THROW_STEPS;
}

/// The frames Catch's hand presses down, 2 pixels a frame, and rises again,
/// 1.5 pixels a frame.
#define CATCH_PRESS_FRAMES 10
#define CATCH_RISE_FRAMES 13

/// Catch: carries the hand (SPRITE_MOVE_HAND) from 16 pixels above the caster
/// to the goal in 10 steps, growing from a quarter to full size, presses down
/// 20 pixels moving the target to (-9, -9), rises 19.5 pixels and shrinks back.
void FieldPsynergy_Catch(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct Actor *mainActor = scene->caster;
  struct Actor *secondary = scene->target;
  struct Actor *actor;
  struct Vector3 spawn;
  struct Vector3 origin;
  struct Vector3 target;
  s32 step;
  s32 index;
  s32 steps = CAST_THROW_STEPS + 1;

  SET_POSITION(&origin, mainActor->x.fixed, mainActor->y.fixed + toFIXED(16),
               mainActor->z.fixed);
  if (scene->aimAhead) {
    SET_POSITION(&target, mainActor->x.fixed, mainActor->y.fixed + toFIXED(32),
                 mainActor->z.fixed);
    Vector_AddPolarOffset(toFIXED(32), scene->angle, &target.x);
  } else {
    SET_POSITION(&target, scene->x, scene->y + toFIXED(32), scene->z);
  }
  SET_POSITION(&spawn, scene->x, scene->y + toFIXED(32), scene->z);
  actor = Actor_Spawn(SPRITE_MOVE_HAND, spawn.x, spawn.y, spawn.z);
  if (actor == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  actor->facing = mainActor->facing;
  actor->speedLimit = toFIXED(13) / 10;
  actor->motionFlags = 0;
  Actor_SelectAnimationFar(actor, 5);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_GRB);
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(
        actor, origin.x + (target.x - origin.x) * step / CAST_THROW_STEPS,
        origin.y + (target.y - origin.y) * step / CAST_THROW_STEPS,
        origin.z + (target.z - origin.z) * step / CAST_THROW_STEPS);
    scale = Interpolate(FIXED_ONE / 4, FIXED_ONE, step);
    actor->scaleX = scale;
    actor->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  wait(10);
  Actor_SelectAnimationFar(actor, 6);
  wait(15);
  for (index = 0; index < CATCH_PRESS_FRAMES; index++) {
    actor->y.fixed -= toFIXED(2);
    wait(1);
  }
  Actor_SelectAnimationFar(actor, 5);
  Sound_PlayFar(SE_WATER);
  if (secondary != 0)
    Actor_SetPositionAndResetMotionFar(secondary, -toFIXED(9),
                                       secondary->y.fixed, -toFIXED(9));
  wait(20);
  for (index = 0; index < CATCH_RISE_FRAMES; index++) {
    actor->y.fixed += toFIXED(3) / 2;
    wait(1);
  }
  wait(10);
  Sound_PlayFar(SE_MENU_ERROR);
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(
        actor, target.x + (origin.x - target.x) * step / CAST_THROW_STEPS,
        target.y + (origin.y - target.y) * step / CAST_THROW_STEPS,
        target.z + (origin.z - target.z) * step / CAST_THROW_STEPS);
    scale = Interpolate(FIXED_ONE, FIXED_ONE / 4, step);
    actor->scaleX = scale;
    actor->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  Actor_ReleaseFar(actor);
  FieldPsynergy_FadeFromCastColor();
}

void Rope_GetEndNodePosition(s32 target, struct Vector3 *position);

/// The stages of Lash's hand on a rope (ScatterState.phase).
enum LashPhase {
  LASH_HAND_SHAKES = 0, ///< trembling over the rope's end
  LASH_HAND_STILL = 1,
  LASH_HAND_PULLS = 2, ///< on the rope's end until the rope is tied
  LASH_HAND_RISES = 3,
  LASH_HAND_GONE = 4 ///< its sparkles die away
};

/// Task of the Lash on a rope: the hand trembles, takes the rope's end and
/// rises away, scattering sparkles all the while.
void FieldPsynergy_UpdateLash(void) {
  struct ScatterState *state =
      (struct ScatterState *)Ram_HeapSlots->battleEffect;
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *actor = state->actor;
  struct Vector3 position;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  s32 i;

  state->flash = 0;
  for (i = 0; i < state->spawnCount; i++) {
    FIXED distance;
    s32 speed;

    particle = &state->particles[state->next];
    SET_POSITION(particle, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    distance = Random16() * 12 + toFIXED(4);
    Vector_AddPolarOffset(distance, Random16(), particle);
    particle->velocityX = 0;
    particle->velocityY = toFIXED(2) - Random16() * 2;
    particle->velocityZ = 0;
    speed = Random16() + toFIXED(3) / 2;
    Vector_AddPolarOffset(speed, Random16(), &particle->velocityX);
    particle->timer = 0;
    state->next++;
    state->next &= SCATTER_PARTICLE_COUNT - 1;
  }

  for (i = 0, particle = state->particles; i < SCATTER_PARTICLE_COUNT;
       i++, particle++) {
    s32 timer = particle->timer;

    if (timer >= 0) {
      sprite = &state->sprites[i];
      if (i & 1)
        sprite->oam.index = state->tileA + (timer & 3) * 2;
      else
        sprite->oam.index = state->tileB + (timer & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      Field_AdvanceWithGravity3D(particle, 63, -(toFIXED(5) / 64));
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  switch (state->phase) {
  case LASH_HAND_SHAKES: {
    s32 a = Random16();
    s32 b = Random16();
    s32 c, d;

    position.x = scene->x + a * 3 - b * 3;
    c = Random16();
    d = Random16();
    position.y = scene->y + c * 3 - d * 3;
    position.z = scene->z;
    Actor_SetPositionAndResetMotionFar(actor, position.x, position.y,
                                       position.z);
    if (state->timer == 20) {
      state->phase++;
      state->timer = -1;
    }
    break;
  }
  case LASH_HAND_STILL:
    Actor_SetPositionAndResetMotionFar(actor, scene->x, scene->y, scene->z);
    if (state->timer == 6) {
      state->phase++;
      state->timer = -1;
    }
    break;
  case LASH_HAND_PULLS:
    if (state->timer == 0)
      state->flash = 1;
    Rope_GetEndNodePosition(field->targetX, &position);
    SET_ACTOR_POSITION(actor, position.x, position.y - toFIXED(4), position.z);
    if (state->hit != 0) {
      state->phase++;
      state->timer = -1;
    }
    if (state->timer == 90) {
      state->phase++;
      state->timer = -1;
    }
    break;
  case LASH_HAND_RISES:
    Actor_SelectAnimationFar(actor, 3);
    state->spawnCount = 3;
    actor->y.fixed += state->timer * toFIXED(2);
    if (state->timer == 10) {
      state->phase++;
      state->timer = -1;
    }
    break;
  case LASH_HAND_GONE:
    actor->scaleX = 0;
    state->spawnCount = 0;
    if (state->timer == 30)
      state->phase = FIELD_PSYNERGY_FINISHED;
    break;
  }
  state->timer++;
}

/// Lash cast on a target: finds the rope's two ends (Rope_FindById); with none,
/// Lash runs as if cast on nothing (enabled), else the field state keeps the
/// rope and its ends. Then runs FieldPsynergy_Lash.
void FieldPsynergy_LashOnTarget(s32 value) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;
  s32 second;
  s32 first;

  if (Rope_FindById(value, &first, &second)) {
    state->aimAhead = 1;
  } else {
    field->targetY = value;
    field->targetZ = first;
    field->targetX = second;
  }
  FieldPsynergy_Lash();
}

/// The frames Lash's hand comes down, 2 pixels a frame, and the sparkles it
/// bursts into with no target.
#define LASH_FALL_FRAMES 16
#define LASH_SPARKLES 16

/// Lash: a hand forms over the spot and comes down on it. With no target it
/// bursts into sparkles; cast on a rope's end it whips out and ties the rope
/// (Rope_Lash) at the event's target.
void FieldPsynergy_Lash(void) {
  struct ScatterState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct ScatterState));
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *caster = scene->caster;
  struct Actor *actor;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  s32 slotA;
  s32 slotB;
  s32 tileA;
  s32 i;

  if (scene->target != 0) {
    scene->y += toFIXED(4);
    scene->z += toFIXED(4);
  }
  FieldPsynergy_FadeToCastColor();
  actor = Actor_Spawn(SPRITE_LASH_HAND, 0, 0, 0);
  if (actor != 0) {
    actor->facing = caster->facing;
    actor->motionFlags = 0;
    if (caster->priorityFlags & ACTOR_PRIORITY_IGNORES_RISES)
      actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    Actor_SelectAnimationFar(actor, 0);
    Sound_PlayFar(SE_ITEM_BREAK);
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_SPARKLE_FADE), state);
    slotA = VramCache_FindFreeEntry();
    tileA = VramCache_Load(slotA, 256, state);
    state->tileA = tileA;
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_STAR_SPARKLE_FADE),
                        state);
    slotB = VramCache_FindFreeEntry();
    state->tileB = VramCache_Load(slotB, 256, state);

    particle = state->particles;
    sprite = state->sprites;
    for (i = 0; i < SCATTER_PARTICLE_COUNT; i++) {
      EffectSprite_Init(sprite, 4, 4, 0, tileA);
      sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
      sprite->oam.priority = 0;
      sprite->oam.fullColor = 1;
      sprite->oam.palette = 0;
      SET_POSITION(particle, actor->x.fixed, actor->y.fixed, actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(12), Random16(), particle);
      particle->timer = -1;
      sprite++;
      particle++;
    }

    FieldPsynergy_GrowFromCaster(actor, scene->x, scene->y + toFIXED(32),
                                 scene->z);
    Actor_SelectAnimationFar(actor, 1);
    wait(10);
    for (i = 0; i < LASH_FALL_FRAMES; i++) {
      actor->y.fixed += -toFIXED(2);
      wait(1);
    }
    Sound_PlayFar(SE_WATER);
    if (scene->aimAhead != 0) {
      for (i = 0; i < LASH_SPARKLES; i++)
        BattleEffect_SpawnScatterParticle(actor->x.fixed, actor->y.fixed,
                                          actor->z.fixed);
      wait(1);
    } else {
      wait(6);
      state->actor = actor;
      state->phase = LASH_HAND_SHAKES;
      state->timer = 0;
      state->next = 0;
      state->spawnCount = 1;
      state->hit = 0;
      Task_Add(FieldPsynergy_UpdateLash, TASK_PASS_FRAME);
      while (state->phase != FIELD_PSYNERGY_FINISHED) {
        if (state->flash == 1) {
          Sound_PlayFar(SE_LASH);
          Rope_Lash(field->targetX, field->targetY, field->targetZ);
          state->hit = 1;
          Sound_PlayFar(SOUND_FADE_OUT_EXIT_SE);
        }
        wait(1);
      }
      Task_Remove(FieldPsynergy_UpdateLash);
    }
    VramCache_FreeEntry(slotA);
    VramCache_FreeEntry(slotB);
    Actor_ReleaseFar(actor);
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
/// frame; its sprite is hidden once its scale falls below 3/16.
void FieldEffect_UpdateRetreatCaster(struct Actor *actor) {
  u32 scale;

  actor->scaleY -= FIXED_ONE * 5 / 512;
  scale = actor->scaleX - FIXED_ONE * 5 / 512;
  actor->facing += DEGtoANG(45);
  actor->y.fixed += toFIXED(1);
  actor->scaleX = scale;
  if ((s32)scale < FIXED_ONE * 3 / 16)
    actor->spriteStorageKind = ACTOR_SPRITE_NONE;
}

/// Field effect sprite callback: a particle flies 120 pixels from the main
/// actor's screen position (8 pixels lower) in a direction within 1/8 turn of
/// up the screen, at up to 5 pixels a frame, and starts again from the main
/// actor on arrival until it takes its last flight.
void Retreat_UpdateSpark(struct FieldParticle *effect) {
  struct Vector3 pos;
  struct Actor *main;
  s32 state;
  u32 spread;

  main = ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->caster;
  state = effect->state;

  if (state == RETREAT_PARTICLE_AIM) {
    pos.x = main->x.fixed;
    pos.y = main->y.fixed;
    pos.z = main->z.fixed;
    Camera_WorldToScreen(&pos);

    effect->x = effect->originX = pos.x;
    effect->z = effect->originZ = pos.z + toFIXED(8);

    // Up the screen, within 1/8 turn either way.
    spread = Random16();
    Vector_AddPolarOffset(toFIXED(120),
                          FIXEDtoINT(spread * (TRIG_TURN / 8)) -
                              Random_Below(TRIG_TURN / 8) + FACING_NORTH,
                          &pos);

    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = toFIXED(5);
    effect->speedMax = toFIXED(5);
    effect->turnStepLimited = state;
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

/// Retreat ("Return to the dungeon's entrance."): the caster turns to face the
/// player, raises their arms, spins faster and faster and shrinks away while
/// sparks fly up the screen, one a frame.
void FieldPsynergy_Retreat(void) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;
  struct Actor *actor = effectState->caster;
  struct Vector3 pos;
  struct FieldParticle *effect;
  struct FieldParticle *effects;
  u32 i;

  Event_Start();
  FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldPsynergy_FadeToCastColor();
  wait(10);
  Actor_FaceDirectionById(effectState->casterId, FACING_SOUTH, 0);
  wait(30);
  actor->update = (ObjectUpdate)Actor_CycleSpriteColors;
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_SelectAnimationFar(actor, ANIM_LEADER_ARMS_RAISED);
  wait(40);

  // Start spinning.
  Sound_PlayFar(SE_SHIMMER);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  Actor_SelectAnimationFar(actor, 3);
  actor->update = (ObjectUpdate)FieldEffect_UpdateSpinAngle;
  actor->timer = 0;
  wait(70);

  // Start shrinking, and send particles from where it is on screen.
  Actor_SetSpriteFlagsFar(actor, 0);
  actor->motionFlags = 0;
  actor->update = (ObjectUpdate)FieldEffect_UpdateRetreatCaster;
  actor->targetX = ACTOR_NO_TARGET;
  SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Camera_WorldToScreen(&pos);
  i = 0;
  effect = effectState->particles;
  effects = effect;
  do {
    FIXED scale;

    FieldParticle_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, Retreat_UpdateSpark);
    FieldParticle_SetAnimation(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, Random_Below(7));
    scale = (Random16() >> 1) + FIXED_ONE * 6 / 5; // 1.2 to 1.7
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

/// The back half of one of Avoid's falling rings, a golden arc
/// (gFieldEffect_ArcSparkTiles) behind the caster: it grows to full size by
/// 3/64 a frame as it falls 2 pixels a frame around the caster, 9 pixels south
/// of them and up to 5 more while small, and is gone 10 pixels above the ground
/// (4 on the World Map).
void Avoid_UpdateRingBackHalf(struct Actor *particle) {
  struct Actor *source;
  FIXED y;
  FIXED limit;
  FIXED scale;

  limit = particle->groundHeight + toFIXED(10);
  source = particle->watchedActor;
  if (gMainRoutineState.mapId == SCENE_CLEAR)
    limit = particle->groundHeight + toFIXED(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + FIXED_ONE * 3 / 64;
  if (scale > FIXED_ONE)
    scale = FIXED_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - toFIXED(2);
  particle->z.fixed = source->z.fixed + ((FIXED_ONE - scale) * 5) + toFIXED(9);
}

/// The ring's front half: the same arc drawn upside down in front of the
/// caster, 16 pixels south and up to 5 nearer while small.
void Avoid_UpdateRingFrontHalf(struct Actor *particle) {
  struct Actor *source;
  FIXED y;
  FIXED limit;
  FIXED scale;

  limit = particle->groundHeight + toFIXED(10);
  source = particle->watchedActor;
  if (gMainRoutineState.mapId == SCENE_CLEAR)
    limit = particle->groundHeight + toFIXED(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + FIXED_ONE * 3 / 64;
  if (scale > FIXED_ONE)
    scale = FIXED_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - toFIXED(2);
  particle->z.fixed = source->z.fixed - ((FIXED_ONE - scale) * 5) + toFIXED(16);
}

/// Drops one of Avoid's rings onto source: its two halves start 64 pixels
/// above it at 0.4 of full size, drawn with the arc's tiles
/// (FieldEffectState.arcTileSlot), the front half over every sprite.
void Avoid_DropRing(struct Actor *source) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;
  struct Actor *leader = effectState->caster;
  struct Actor *actors[2];
  struct Actor *actor;
  struct DescendingArcSprite *sprite;
  s32 arcIndex;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    actor = Actor_CreateFar(SPRITE_GOLDEN_RINGS, source->x.fixed,
                            source->y.fixed + toFIXED(64), source->z.fixed);
    actors[arcIndex] = actor;
    if (actor == NULL)
      continue;
    actor->groundHeight = source->groundHeight;
    sprite = (struct DescendingArcSprite *)actor->sprite;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->watchedActor = source;
    actor->scaleY = FIXED_ONE * 2 / 5;
    actor->scaleX = FIXED_ONE * 2 / 5;
    if (sprite == NULL)
      continue;
    Sprite_StartAnimationFar((struct Sprite *)sprite, 0);
    sprite->flags = 0;
    VramCache_FreeEntry(sprite->slot);
    sprite->slot = effectState->arcTileSlot;
    sprite->retainCacheEntry = 1;
    sprite->attr.half.tile = gVramCacheEntries[sprite->slot].offset >> 5;
    sprite->fullColor = 0;
    sprite->shape = 1;
    sprite->size = 2;
    sprite->firstEntry->picture = 0;
  }
  // BUG: failed allocations above leave a missing arc or sprite here.
  actors[0]->update = (ObjectUpdate)Avoid_UpdateRingFrontHalf;
  ((struct DescendingArcSprite *)actors[0]->sprite)->attr.byte.priority = 0;
  actors[1]->update = (ObjectUpdate)Avoid_UpdateRingBackHalf;
  ((struct DescendingArcSprite *)actors[1]->sprite)->attr.byte.priority =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.priority;
}

/// The caster during Avoid (FieldPsynergy_Avoid): drops a ring onto itself
/// every 5 frames (7 on the World Map), and spins in phase 1, 3/64 turn a
/// frame.
void FieldEffect_UpdateAvoidCaster(struct Actor *caster) {
  s16 phase;
  s32 elapsedFrames;

  phase = caster->arcPhase;
  elapsedFrames = caster->arcFrames++;

  if (gMainRoutineState.mapId == SCENE_CLEAR) {
    if (elapsedFrames % 7 == 0)
      Avoid_DropRing(caster);
  } else if (elapsedFrames % 5 == 0) {
    Avoid_DropRing(caster);
  }

  if (phase == 1)
    caster->facing += TRIG_TURN * 3 / 64;
}

/// Avoid (or the Sacred Feather) has worn off, and the game says so.
/// "Avoid's effects wore off." (or "Sacred Feather's ...").
///
/// Clears gMainRoutineState.avoidDistance. Reached through
/// FieldPsynergy_RunExtraHandler when the Psynergy request the leader's walking
/// raises as Avoid runs out (field_sequence_walking.c) is handled.
void FieldPsynergy_ShowAvoidWoreOff(void) {
  gMainRoutineState.avoidDistance = 0;
  WorldMap_Freeze();
  if (gMainRoutineState.avoidByItem == 0) {
    MessageArgument_PushFar(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
  } else {
    MessageArgument_PushFar(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)&MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
  }
  WorldMap_ReloadPendingAreaGraphics();
}

/// Avoid's flashes, 5 frames each; then the frames its outline blinks, the
/// chime coming on the 15th.
#define AVOID_FLASHES 20
#define AVOID_BLINK_FRAMES 70
#define AVOID_CHIME_FRAME 15

/// Avoid: the caster spins and flashes, then blinks, and encounters are kept
/// away until Avoid wears off.
void FieldPsynergy_Avoid(void) {
  struct FieldEffectState *effectState;
  struct Actor *actor;
  struct Sprite *group;
  struct SpriteAnimation *entry;
  const u8 *tiles;
  u32 angle;
  s32 flags;
  s32 slot;
  s32 count;
  s32 phase;

  effectState = Ram_HeapSlots->fieldEffectState;
  actor = effectState->caster;
  group = actor->sprite;
  entry = group->entries[0];
  angle = actor->facing;
  flags = group->flags;
  slot = VramCache_FindFreeEntry();
  // The arcs' tiles (256 bytes) stay loaded until the Psynergy's end, and
  // monsters stay away for the next 2,400 pixels the leader walks.
  effectState->arcTileSlot = slot;
  tiles = gFieldEffect_ArcSparkTiles;
  VramCache_Load((s16)slot, 256, tiles);
  gMainRoutineState.avoidDistance = toFIXED(2400);
  gMainRoutineState.avoidByItem = GameFlag_Get(FLAG_PSYNERGY_FROM_ITEM);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  actor->update = (ObjectUpdate)FieldEffect_UpdateAvoidCaster;
  actor->arcPhase = 0;
  actor->arcFrames = 0;
  Sound_PlayFar(SE_SPIN);
  wait(15);
  actor->arcPhase = 1;
  wait(10);
  // Flash white for 2 frames in every 5.
  for (count = 0; count < AVOID_FLASHES; count++) {
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

  // Blink its outline beside its own flags on a five-frame cycle.
  for (count = 0; count < AVOID_BLINK_FRAMES; count++) {
    phase = count % 5;
    if (phase == 0) {
      group->dirty = 1;
      group->flags = flags | SPRITE_FLAG_OUTLINED;
    } else if (phase == 2) {
      group->dirty = 1;
      group->flags = flags;
      entry->colorMode = SPRITE_COLOR_NORMAL;
    }
    if (count == AVOID_CHIME_FRAME)
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

  VramCache_FreeEntry(effectState->arcTileSlot);
  FieldMessage_ShowFar((s32)&MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
}

#define ARC_GROUPS_WAIT_FRAMES 90

/// Waits, at most ARC_GROUPS_WAIT_FRAMES, until both pairs of swelling arcs
/// have run their 32 frames.
void BattleEffect_WaitForArcGroups(void) {
  struct ArcEffectState *state = Ram_HeapSlots->arcEffect;
  s32 i = 0;

  while (state->age[0] < ARC_GROUP_FRAMES || state->age[1] < ARC_GROUP_FRAMES) {
    wait(1);
    i++;
    if (i >= ARC_GROUPS_WAIT_FRAMES)
      return;
  }
}

/// Per-frame update: flashes an actor between colour mode 7 and 0 every 2
/// frames and spawns the swelling arcs on it every 16.
void FieldEffect_FlickerActorAndSpawnArcs(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    BattleEffect_SpawnScaledArcObjects(actor);
  }
}

/// Per-frame update: cycles an actor through colour modes 0 to 5, one step
/// every 2 frames, and spawns the swelling arcs on it every 16.
void FieldEffect_CycleActorAndSpawnArcs(struct Actor *actor) {
  if (gFrameCount & 1)
    Actor_SetSpriteColorModeFar(actor, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    BattleEffect_SpawnScaledArcObjects(actor);
}

/// Gives the caster its look while it casts (enum CasterLook).
void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode) {
  if (mode == CASTER_LOOK_NORMAL) {
    actor->update = 0;
    Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  } else if (mode == CASTER_LOOK_FLASHING_WHITE) {
    actor->update = (ObjectUpdate)FieldEffect_FlickerActorAndSpawnArcs;
  } else {
    actor->update = (ObjectUpdate)FieldEffect_CycleActorAndSpawnArcs;
  }
}

/// Sets up the current group's two arc sprites from the effect's tiles, at the
/// target's priority, then restarts that group's age, records the target and
/// switches to the other group.
void BattleEffect_SpawnScaledArcObjects(struct Actor *target) {
  struct ArcEffectState *state = Ram_HeapSlots->arcEffect;
  s32 tiles = gVramCacheEntries[state->resource].offset >> 5;
  s32 group = state->group;
  struct EffectSprite *sprite;
  s32 i;

  for (i = 0; i < 2; i++) {
    sprite = &state->sprites[group][i];
    EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tiles);
    sprite->oam.fullColor = 0;
    sprite->oam.palette = 0;
    sprite->oam.priority = Actor_GetSpritePriority(target);
  }
  state->age[group] = 0;
  state->target = target;
  state->group ^= 1;
}

/// Task: draws the two pairs of swelling arcs on their actor: each pair swells
/// and shrinks over 32 frames (scale sin(age / 64 turn)) as it rises a pixel a
/// frame, the second arc of a pair upside down, one in front of the actor and
/// one behind.
void BattleEffect_UpdateArcEffect(void) {
  struct ArcEffectState *state = Ram_HeapSlots->arcEffect;
  struct Actor *target = state->target;
  s32 group;
  FIXED position[3];
  s32 i;

  for (group = 0; group < 2; group++) {
    FIXED scale = Trig_Sin(state->age[group] * (TRIG_TURN / 64));

    if (state->age[group] < 0)
      continue;
    if (state->age[group] < ARC_GROUP_FRAMES) {
      struct EffectSprite *sprite;
      FIXED x, y;

      position[0] = target->x.fixed;
      position[1] = target->y.fixed + toFIXED(state->age[group]);
      position[2] = target->z.fixed;
      Camera_WorldToScreen(position);
      x = position[0];
      y = position[2];
      for (i = 0; i < 2; i++) {
        sprite = &state->sprites[group][i];
        sprite->x = x;
        sprite->y = y;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        if (i == 0) {
          sprite->depth = Actor_GetSpriteDepth(state->target) - 1;
        } else {
          sprite->depth = Actor_GetSpriteDepth(state->target) + 1;
          sprite->y -= toFIXED(1);
          sprite->scaleY = -sprite->scaleY;
        }
        EffectSprite_Draw(sprite);
      }
      state->age[group]++;
    }
  }
}

/// Starts the swelling arcs over a caster: loads their tiles and starts
/// BattleEffect_UpdateArcEffect with no pair showing.
void BattleEffect_StartArcEffect(void) {
  struct ArcEffectState *state =
      HeapSlot_Alloc(HEAP_SLOT_ARC_EFFECT, sizeof(*state));

  state->resource = VramCache_FindFreeEntry();
  VramCache_Load(state->resource, 256, gFieldEffect_SparkTiles);
  state->group = 0;
  state->age[0] = ARC_GROUP_UNSPAWNED;
  state->age[1] = ARC_GROUP_UNSPAWNED;
  state->target = 0;
  Task_Add((s32)BattleEffect_UpdateArcEffect, TASK_PASS_FRAME);
}

/// Ends the swelling arcs: frees their tiles and their state, and stops their
/// task.
void BattleEffect_EndArcEffect(void) {
  struct ArcEffectState *state = Ram_HeapSlots->arcEffect;

  VramCache_FreeEntry(state->resource);
  Task_Remove((u32)BattleEffect_UpdateArcEffect);
  HeapSlot_Release(HEAP_SLOT_ARC_EFFECT);
}

/// Moves the state on to its next phase, its timer restarted.
#define BOW_NEXT_PHASE(state) ((state)->phase++, (state)->timer = -1)
/// Of the BOW_SPARK_COUNT sparks, those the bow throws off.
#define BOW_SPARKS_THROWN 10
/// While the thrown sparks repeat, each starts again after this many frames.
#define BOW_SPARK_FRAMES 10

/// The stages of the Arrow (BowState.phase).
enum ArrowPhase {
  ARROW_BOW_GROWS = 0, ///< the bow grows and turns to face ahead
  ARROW_BOW_DRAWN = 1, ///< the arrow is drawn back and loosed
  ARROW_FLIES = 2,     ///< until it strikes or has flown 80 pixels
  ARROW_STOPS = 3,
  ARROW_FALLS = 4,      ///< a struck arrow quivers; a spent one shrinks away
  ARROW_BOW_SHRINKS = 5 ///< ends once the bow is gone
};

/// Task of the Arrow, the field Psynergy of the unused ability "A??" (named
/// here for the bow it draws and the arrow it looses): the bow and the flaming
/// arrow.
void FieldPsynergy_UpdateArrow(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct BowState *state = (struct BowState *)Ram_HeapSlots->battleEffect;
  struct CanvasSpriteState *canvas = Ram_HeapSlots->canvasSprite;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *actor;
  struct BowMotion *motion;
  struct EffectSprite *sprite;
  s32 pos[3];
  s32 objectIndex;
  s32 i;
  s32 dx;
  s32 dz;

again:
  switch (state->phase) {
  case ARROW_BOW_GROWS:
    if (state->timer == 0) {
      state->sparking = 0;
      state->sparksRepeat = 1;
      state->arrowMoving = 0;
      state->bowGrowing = 1;
      state->frame = BOW_FRAME_IDLE;
    } else if (state->bowGrowing == 0) {
      state->frame = 0;
      BOW_NEXT_PHASE(state);
    }
    break;
  case ARROW_BOW_DRAWN:
    if (state->timer == 0) {
      sprite = &state->arrow;
      sprite->scaleX = sprite->scaleY = FIXED_ONE;
      motion = &state->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset(toFIXED(3) / 20, scene->angle8 + DEGtoANG(180),
                            pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      state->arrowMoving = 1;
    }
    if (state->timer == 20)
      state->sparking = 1;
    if (state->timer == 56) {
      motion = &state->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset(toFIXED(5), scene->angle8, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      BOW_NEXT_PHASE(state);
    }
    break;
  case ARROW_FLIES:
    motion = &state->arrowMotion;
    if (state->timer == 90) {
      BOW_NEXT_PHASE(state);
      break;
    }
    dx = state->target[0] - state->start[0];
    if (dx < 0)
      dx = state->start[0] - state->target[0];
    dz = motion->x - state->start[0];
    if (dz < 0)
      dz = state->start[0] - motion->x;
    if (dx < dz) {
      BOW_NEXT_PHASE(state);
      goto again;
    }
    dx = state->target[2] - state->start[2];
    if (dx < 0)
      dx = state->start[2] - state->target[2];
    dz = motion->z - state->start[2];
    if (dz < 0)
      dz = state->start[2] - motion->z;
    if (dx < dz) {
      BOW_NEXT_PHASE(state);
      goto again;
    }
    pos[0] = motion->x;
    pos[1] = motion->y;
    pos[2] = motion->z;
    if (MapGrid_CheckCollisionFar(scene->caster, pos) > 0) {
      state->done = 1;
      state->result = -1;
      state->phase++;
      state->timer = -1;
      goto again;
    }
    for (objectIndex = 0; objectIndex < FIELD_ACTOR_COUNT; objectIndex++) {
      actor = field->actors[objectIndex];
      if (actor != NULL && actor->script != NULL &&
          (actor->collisionFlags & ACTOR_SOLID) && actor != scene->caster &&
          FieldPsynergy_CheckOverlap(&motion->x, 2, &actor->x.fixed,
                                     actor->radius - 2) >= 0) {
        state->done = 1;
        state->result = objectIndex;
        BOW_NEXT_PHASE(state);
        goto again;
      }
    }
    break;
  case ARROW_STOPS:
    if (state->done != 0) {
      state->arrowMoving = 0;
    } else {
      state->sparksRepeat = 0;
      state->bowShrinking = 1;
    }
    motion = &state->arrowMotion;
    pos[0] = pos[1] = pos[2] = 0;
    Vector_AddPolarOffset(toFIXED(2), scene->angle8, pos);
    motion->velocityX = pos[0];
    motion->velocityY = pos[1];
    motion->velocityZ = pos[2];
    BOW_NEXT_PHASE(state);
    break;
  case ARROW_FALLS:
    sprite = &state->arrow;
    if (state->done != 0) {
      if (state->timer < 32) {
        s32 wobble[8];
        s32 fade[4];
        s32 w;

        memcpy(wobble, gArrow_WobbleAngles, sizeof(wobble));
        memcpy(fade, gArrow_WobbleFades, sizeof(fade));
        w = wobble[state->timer & 7];
        sprite->angle =
            scene->angle8 + w * fade[state->timer / 8] / (TRIG_TURN);
      }
      if (state->timer == 32)
        state->bowShrinking = 1;
      if (state->timer == 50) {
        sprite->scaleX = sprite->scaleY = 0;
        state->arrowMoving = 1;
        state->sparksRepeat = 0;
        BOW_NEXT_PHASE(state);
      }
    } else {
      sprite->scaleX += -(FIXED_ONE * 3 / 16);
      sprite->scaleY = sprite->scaleX;
      if (sprite->scaleX < 0) {
        sprite->scaleX = sprite->scaleY = 0;
        BOW_NEXT_PHASE(state);
      }
    }
    break;
  case ARROW_BOW_SHRINKS:
    if (state->bowShrinking == 0)
      state->phase = FIELD_PSYNERGY_FINISHED;
    break;
  }

  if (state->bowGrowing != 0) {
    sprite = &state->bow;
    sprite->scaleX += FIXED_ONE / 64;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += DEGtoANG(45);
    if (sprite->scaleX >= FIXED_ONE) {
      sprite->scaleX = sprite->scaleY = FIXED_ONE;
      sprite->angle = scene->angle8;
      state->bowGrowing = 0;
    }
  }
  if (state->bowShrinking != 0) {
    sprite = &state->bow;
    sprite->scaleX += -(FIXED_ONE / 64);
    sprite->scaleY = sprite->scaleX;
    sprite->angle += DEGtoANG(45);
    if (sprite->scaleX <= toFIXED(0.4) - 1) {
      sprite->scaleX = sprite->scaleY = 0;
      state->bowShrinking = 0;
    }
  }

  // The bow's drawing pose, by frame: drawn back, then loosed and recovered.
  if (state->frame != BOW_FRAME_IDLE) {
    state->frame++;
    sprite = &state->bow;
    switch (state->frame) {
    case 10:
      sprite->oam.index = state->bowTile + BOW_POSE_TILES;
      break;
    case 20:
      sprite->oam.index = state->bowTile + BOW_POSE_TILES * 2;
      break;
    case 54:
      sprite->oam.index = state->bowTile + BOW_POSE_TILES;
      break;
    case 62:
      sprite->oam.index = state->bowTile + BOW_POSE_TILES * 3;
      break;
    case 65:
      sprite->oam.index = state->bowTile + BOW_POSE_TILES * 4;
      break;
    case 68:
      sprite->oam.index = state->bowTile + BOW_POSE_TILES * 5;
      break;
    case 58:
    case 71:
      sprite->oam.index = state->bowTile;
      break;
    case 72:
      state->frame = BOW_FRAME_IDLE;
      break;
    }
  }

  motion = &state->bowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  sprite = &state->bow;
  Camera_WorldToScreen(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];
  EffectSprite_Draw(sprite);
  motion = &state->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Camera_WorldToScreen(pos);
  sprite = &state->arrow;
  sprite->x = pos[0];
  sprite->y = pos[2];
  EffectSprite_Draw(sprite);
  if (state->arrowMoving != 0)
    Field_AdvanceWithGravity3D(&motion->x, 64, 0);

  if (state->sparking != 0) {
    motion = state->sparks;
    for (i = 0; i < BOW_SPARKS_THROWN; i++) {
      if (motion->delay == 0) {
        s32 speed;
        s32 angle;

        speed = (Random16() & 255) + 256;
        angle = (Random16() & 0x1fff) + TRIG_TURN * 7 / 16;
        motion->x = toFIXED(60);
        motion->y = toFIXED(28);
        motion->velocityX = Trig_Cos(angle) * speed >> 6;
        motion->velocityY = Trig_Sin(angle) * speed >> 6;
      }
      if (motion->delay > 0) {
        s32 x = (s16)FIXEDtoINT(motion->x);
        s32 y = (s16)FIXEDtoINT(motion->y);

        BattleEffect_DrawCanvasBlock(
            gArrow_GlintPixels + (motion->delay & 1) * 64, x, y, 8, 8);
        Field_AdvanceWithGravity2D(&motion->x, 60, 0);
      }
      motion->delay++;
      if (state->sparksRepeat == 1 && motion->delay == BOW_SPARK_FRAMES)
        motion->delay = 0;
      motion++;
    }
  }

  motion = &state->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Vector_AddPolarOffset(-toFIXED(36), scene->angle8, pos);
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIXEDtoINT(pos[0]);
  canvas->y = (s16)FIXEDtoINT(pos[2]);
  canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
  state->timer++;
}

/// The Arrow cast on a target: runs FieldPsynergy_Arrow. Nothing calls it, as
/// the dispatch of Psynergy cast on a target has no case for the Arrow.
void FieldPsynergy_ArrowOnTarget(void) { FieldPsynergy_Arrow(); }

/// The Arrow: a bow grows in front of the caster and draws, throwing off
/// glints, and looses a flaming arrow that flies up to 80 pixels; an actor it
/// strikes becomes the field state's psynergyHitObject.
void FieldPsynergy_Arrow(void) {
  struct BowState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BowState));
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct CanvasSpriteState *canvas;
  struct EffectSprite *sprite;
  struct BowMotion *motion;
  FIXED pos[3];
  s32 arrowSlot;
  s32 bowSlot;
  s32 tile;
  u16 *palette;
  s32 blue;
  s32 shade;
  s32 i;

  FieldPsynergy_FadeToCastColor();
  // The spark pointer and the sprite pointer are one variable.
  sprite = (struct EffectSprite *)state->sparks;
  for (i = 0; i < BOW_SPARK_COUNT; i++) {
    ((struct BowMotion *)sprite)->delay = -i;
    sprite = (struct EffectSprite *)((struct BowMotion *)sprite + 1);
  }

  state->tiles = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, BOW_TILE_BYTES);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_BOW_DRAW), state->tiles);
  bowSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(bowSlot, BOW_TILE_BYTES, state->tiles);
  state->bowTile = tile;
  sprite = &state->bow;
  EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X32, tile);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = Actor_GetSpritePriority(scene->caster);
  sprite->depth = Actor_GetSpriteDepth(scene->caster) + 1;
  sprite->scaleX = FIXED_ONE * 2 / 5;
  sprite->scaleY = FIXED_ONE * 2 / 5;
  sprite->angle = scene->angle8;
  pos[0] = scene->caster->x.fixed;
  pos[2] = scene->caster->z.fixed;
  motion = &state->bowMotion;
  Vector_AddPolarOffset(toFIXED(8), scene->angle8, pos);
  SET_POSITION(motion, pos[0], toFIXED(16), pos[2]);

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FLAMING_ARROW),
                      state->tiles);
  arrowSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(arrowSlot, 256, state->tiles);
  sprite = &state->arrow;
  EffectSprite_Init(sprite, 12, 8, OBJ_SIZE_16X16, tile);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = Actor_GetSpritePriority(scene->caster);
  sprite->depth = Actor_GetSpriteDepth(scene->caster) + 2;
  sprite->scaleX = 0;
  sprite->scaleY = 0;
  sprite->angle = scene->angle8;
  pos[0] = scene->caster->x.fixed;
  pos[2] = scene->caster->z.fixed;
  motion = &state->arrowMotion;
  Vector_AddPolarOffset(toFIXED(18), scene->angle8, pos);
  SET_POSITION(motion, pos[0], toFIXED(16), pos[2]);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);

  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  canvas->priority = Actor_GetSpritePriority(scene->caster);
  canvas->depth = Actor_GetSpriteDepth(scene->caster) + 3;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->command = 0;
  canvas->angle = scene->angle8;
  state->start[0] = scene->caster->x.fixed;
  state->start[1] = scene->caster->y.fixed;
  state->start[2] = scene->caster->z.fixed;
  blue = RGB(0, 0, RGB_LEVEL_MAX);
  state->target[0] = scene->caster->x.fixed;
  state->target[1] = scene->caster->y.fixed;
  state->target[2] = scene->caster->z.fixed;
  Vector_AddPolarOffset(toFIXED(80), scene->angle8, state->target);

  // The canvas palette: blue full, red and green rising from half to full.
  palette = (u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
  for (i = 0; i < PALETTE_COLORS; i++) {
    shade = i + (RGB_LEVEL_MAX + 1) / 2;
    Io_Write16(RGB(0, shade, 0) | blue | shade, palette);
    palette++;
  }

  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  state->bowGrowing = 0;
  state->bowShrinking = 0;
  state->done = 0;
  state->result = -1;
  state->phase = ARROW_BOW_GROWS;
  state->timer = 0;
  Task_Add(FieldPsynergy_UpdateArrow, TASK_PASS_FRAME);
  while (state->phase != FIELD_PSYNERGY_FINISHED)
    wait(1);
  Task_Remove(FieldPsynergy_UpdateArrow);
  if (state->done != 0 && state->result != -1)
    field->psynergyHitObject = state->result;
  VramCache_FreeEntry(arrowSlot);
  VramCache_FreeEntry(bowSlot);
  FieldPsynergy_FadeFromCastColor();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Finds what Cyclone reaches: of the 21 cells round the caster
/// (gFieldPsynergy_NearbyCellOffsets) on its layer, those of a CycloneCellKind
/// within reach are listed, as cells, in the field state's points, which end
/// with -1, -1. A cell to blow away (CYCLONE_CELL_BURST) also sets
/// CycloneState.kind; the last carry and lift cells are kept as the pixels of
/// their centres, in cycloneCarryX, Z and cycloneLiftX, Z.
void FieldPsynergy_GatherNearbyCells(void) {
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct PointState *field = Ram_HeapSlots->fieldState;
  struct CycloneState *state = Ram_HeapSlots->battleEffect;
  s16 *points = field->points;
  struct Actor *owner = effect->caster;
  FIXED position[3];
  FIXED x, z;
  s32 kind;
  u32 i;
  s32 count = 0;

  state->kind = count;
  field->cycloneCarryX = -1;
  field->cycloneCarryZ = -1;
  field->cycloneLiftX = -1;
  field->cycloneLiftZ = -1;
  for (i = 0; i < FIELD_PSYNERGY_NEARBY_CELLS; i++) {
    x = owner->x.fixed +
        FIELD_CELL_ORIGIN(gFieldPsynergy_NearbyCellOffsets[i * 2]);
    z = owner->z.fixed +
        FIELD_CELL_ORIGIN(gFieldPsynergy_NearbyCellOffsets[i * 2 + 1]);
    kind = MapGrid_GetCycloneCellKind(x, z, owner->layer);
    // position[1] is never set: only x and z reach Field_MeasurePsynergyHeight.
    position[0] = x;
    position[2] = z;
    if (kind == CYCLONE_CELL_BURST &&
        Field_MeasurePsynergyHeight(effect->fieldPsynergy, owner, position) !=
            -1) {
      points[count * 2] = x / toFIXED(16);
      points[count * 2 + 1] = z / toFIXED(16);
      count++;
      state->kind = kind;
    }
    if (kind == CYCLONE_CELL_CARRY &&
        Field_MeasurePsynergyHeight(effect->fieldPsynergy, owner, position) !=
            -1) {
      field->cycloneCarryX = FIXEDtoINT(x);
      field->cycloneCarryZ = FIXEDtoINT(z);
      field->cycloneCarryX =
          (field->cycloneCarryX & ~(MAPGRID_CELL_PIXELS - 1)) +
          MAPGRID_CELL_PIXELS / 2;
      field->cycloneCarryZ =
          (field->cycloneCarryZ & ~(MAPGRID_CELL_PIXELS - 1)) +
          MAPGRID_CELL_PIXELS / 2;
      points[count * 2] = x / toFIXED(16);
      points[count * 2 + 1] = z / toFIXED(16);
      count++;
    }
    if (kind == CYCLONE_CELL_LIFT &&
        Field_MeasurePsynergyHeight(effect->fieldPsynergy, owner, position) !=
            -1) {
      field->cycloneLiftX = FIXEDtoINT(x);
      field->cycloneLiftZ = FIXEDtoINT(z);
      field->cycloneLiftX = (field->cycloneLiftX & ~(MAPGRID_CELL_PIXELS - 1)) +
                            MAPGRID_CELL_PIXELS / 2;
      field->cycloneLiftZ = (field->cycloneLiftZ & ~(MAPGRID_CELL_PIXELS - 1)) +
                            MAPGRID_CELL_PIXELS / 2;
      points[count * 2] = x / toFIXED(16);
      points[count * 2 + 1] = z / toFIXED(16);
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

/// Runs the place's Psynergy event once for each kind of cell Cyclone touched
/// (FieldState.psynergyCells), lowest cell behaviour first.
void FieldPsynergy_RunTouchedEvents(void) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;
  struct TouchCell *cells = (struct TouchCell *)gFieldState->psynergyCells;
  struct Actor *caster = effectState->caster;
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
                                          effectState->fieldPsynergy, id);

    if (event != 0)
      Field_RunPsynergyEvent(event, effectState->casterId,
                             effectState->targetId);
  }
}

struct MapCell *MapGrid_GetCellAt(s32 x, s32 z, s32 layer);

/// Clears what Cyclone blew away: each cell of FieldState.psynergyCells that
/// falls in the width by height window at (windowX, windowZ) of layerIndex
/// (counted from the layer's scroll and the leader's height) takes the cell
/// picture from the same place of the window at (sourceX, sourceZ), is opened
/// on layer 2, and is recorded (up to 64) in gTouchCopies to be made again.
void TileCopies_CopyTouchedCells(s32 sourceX, s32 sourceZ, s32 width,
                                 s32 height, s32 windowX, s16 windowZ,
                                 s32 layerIndex) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchCell *cell = (struct TouchCell *)field->psynergyCells;
  s32 ownerHeight = FIELD_CELL(owner->groundHeight);
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[layerIndex];
  s32 i;

  /* FAKEMATCH(signature): gSceneLocalState typed as 1491 TouchCells before the
   * copies. */
  for (i = 0; cell[i].x != -1; i++) {
    s32 x = cell[i].x - windowX;
    s32 z = cell[i].z - windowZ - ownerHeight;

    x += FIELD_CELL(layer->offsetX);
    z += FIELD_CELL(layer->offsetY);

    if (x >= 0 && x < width && z >= 0 && z < height) {
      s32 n;

      MapGrid_CopyCellTilesCellsRectFar(sourceX + x, sourceZ + z, 1, 1,
                                        windowX + x, windowZ + z);
      MapGrid_GetCellAt(FIELD_CELL_ORIGIN(cell[i].x),
                        FIELD_CELL_ORIGIN(cell[i].z), 2)
          ->collisionCode = 0;
      n = gTouchCopies.count;
      if (n < (s32)ARRAY_COUNT(gTouchCopies.copies)) {
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

/// Makes again each copy TileCopies_CopyTouchedCells recorded: copies the cell
/// picture back and opens the cell on layer 2.
void TileCopies_Restore(void) {
  struct TouchCopies *base = &gTouchCopies;
  s32 i;

  for (i = 0; i < gTouchCopies.count; i++) {
    struct TouchCopy *entry = &base->copies[i];

    MapGrid_CopyCellTilesCellsRectFar(entry->toX, entry->toZ, 1, 1,
                                      entry->fromX, entry->fromZ);
    MapGrid_GetCellAt(FIELD_CELL_ORIGIN(entry->cellX),
                      FIELD_CELL_ORIGIN(entry->cellZ), 2)
        ->collisionCode = 0;
  }
}
