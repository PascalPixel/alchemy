/// @file
/// Halt.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The sparkles the stopwatch bursts into.
#define HALT_SPARKLES 16

/// Field effect sprite callback of a Halt sparkle (enum OutAndBackState): it
/// heads for a point 40 to 70 pixels from where it appeared, then for a point 4
/// pixels round 8 pixels above the Psynergy's position on screen, and is freed
/// on arrival.
void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
  struct FieldEffectWork *effectWork;
  struct Vector3 pos;
  s16 angle;
  s32 state;

  effectWork = gFieldEffectWork;
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
    pos.x = effectWork->x;
    pos.y = effectWork->y + toFIXED(8);
    pos.z = effectWork->z;
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

/// Pauses the target (effectWork->child) and runs FieldPsynergy_Halt.
void FieldPsynergy_HaltOnTarget(s32 unused) {
  struct Actor *target = ((struct FieldEffectWork *)gFieldEffectWork)->child;

  target->paused = 1;
  FieldPsynergy_Halt();
}

/// A stopwatch (SPRITE_STOPWATCH) flies from over the caster to the Psynergy's
/// position over GROW_STEPS frames, growing, sounds three low tones and a high
/// one, and bursts into HALT_SPARKLES sparkles that fly out and back.
void FieldPsynergy_Halt(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *caster = effectWork->mainActor;
  struct Actor *stopwatch;
  struct FieldParticle *effect;
  struct Vector3 pos;
  struct Vector3 from;
  struct Vector3 to;
  s32 step;
  s32 i;

  effectWork->y = caster->y.fixed;
  stopwatch = Actor_Spawn(SPRITE_STOPWATCH, 0, 0, 0);
  step = 0;
  Actor_SelectAnimationFar(stopwatch, 0);
  if (stopwatch == NULL)
    return;

  // From 16 pixels above the caster to 8 pixels above the Psynergy's position.
  FieldPsynergy_FadeToCastColor();
  from.x = caster->x.fixed;
  from.y = caster->y.fixed + toFIXED(16);
  from.z = caster->z.fixed;
  to.x = effectWork->x;
  to.y = effectWork->y + toFIXED(8);
  to.z = effectWork->z;
  // It arrives on the last step, grown from a quarter to full size.
  for (;;) {
    s32 steps = GROW_STEPS + 1;
    s32 scale;

    SET_ACTOR_POSITION(stopwatch, from.x + (to.x - from.x) * step / GROW_STEPS,
                       from.y + (to.y - from.y) * step / GROW_STEPS,
                       from.z + (to.z - from.z) * step / GROW_STEPS);
    scale = FieldEffect_Interpolate(FIXED_ONE, FIXED_ONE / 4, step);
    stopwatch->scaleX = scale;
    stopwatch->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  // Three low tones and a high one, 10 frames apart, as the stopwatch changes
  // to its second animation.
  wait(5);
  Actor_SelectAnimationFar(stopwatch, 1);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_LOW);
  wait(10);
  Sound_PlayFar(SE_TRIPLE_TONE_HIGH);

  // Sparkles 4 pixels round it fly out, then come back to the Psynergy's
  // position.
  effect = &effectWork->particles[0];
  for (i = 0; i < HALT_SPARKLES; i++) {
    pos.x = stopwatch->x.fixed;
    pos.y = stopwatch->y.fixed + toFIXED(8);
    pos.z = stopwatch->z.fixed;
    Camera_WorldToScreen((struct Vector3 *)&pos.x);
    Vector_AddPolarOffset(toFIXED(4), Random16(), &pos.x);
    FieldParticle_Initialize(effect, SPRITE_SPARKLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateHaltParticle);
    Sprite_SetAnimationColorModesFar(effect->actor, SPRITE_COLOR_WHITE);
    effect++;
  }

  pos.x = stopwatch->x.fixed;
  pos.y = stopwatch->y.fixed + toFIXED(8);
  pos.z = stopwatch->z.fixed;
  wait(8);
  Actor_ReleaseFar(stopwatch);
  wait(4);
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

/// Per-frame update of a paused actor: gives its sprite one of the table's
/// eight params at random.
void FieldPsynergy_UpdateHaltedActor(struct Actor *actor) {
  void *object = (void *)actor;
  const s8 *table = gFieldEffect_RandomChildValues;
  s32 random = Random16();
  Actor_SetSpriteColorModeFar(object, table[(u32)(random * 8) >> 16]);
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
      entry = actor->sprite->entries[0];
      if (entry != NULL) {
        gMainRoutineWork.pausedPose = entry->colorMode;
      }
    }
    actor->update = FieldPsynergy_UpdateHaltedActor;
    actor->paused = 1;
    Actor_SetAnimationSpeedFar(actor, 0);
  }
}

/// Resumes actor id with the update and param kept by FieldPsynergy_HaltActor.
void FieldPsynergy_ResumeHaltedActor(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    if (actor->update == FieldPsynergy_UpdateHaltedActor) {
      struct MainRoutineWork *state = &gMainRoutineWork;
      actor->update = (void (*)(struct Actor *))state->pausedUpdate;
      state->pausedUpdate = 0;
      Actor_SetSpriteColorModeFar(actor, state->pausedPose);
    }
    actor->paused = 0;
    Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL);
  }
}

/// Returns id, or -1 (no target) when Halt passes actor id's sprite over
/// (SPEAKER_HALTED_LINES_NONE in gSpeakerDefinitions): the talking monsters and
/// the things, the 20 rows before the default.
s32 FieldPsynergy_FilterHaltTarget(s32 id) {
  if (Speaker_GetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
    return -1;
  return id;
}

#undef HALT_SPARKLES
