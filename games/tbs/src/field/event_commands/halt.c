/// @file
/// Halt.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Field effect sprite callback (enum OutAndBackState): the particle heads for
/// a point 40 to 70 pixels from where it appeared, then for a point 4 pixels
/// round 8 pixels above the Psynergy's position on screen, and is freed on
/// arrival.
void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
  struct FieldEffectWork *effectWork;
  struct Vector3 pos;
  s8 *statePointer;
  s16 angle;
  s32 state;

  effectWork = gFieldEffectWork;
  statePointer = &effect->state;
top:
  state = *statePointer;
  if (state == OUT_AND_BACK_AIM_OUT) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + px(40), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = px(4);
    effect->speedMax = px(4);
    effect->turnStepLimited = state;
    *statePointer = *statePointer + 1;
    return;
  }
  if (state == OUT_AND_BACK_FLYING_OUT) {
    if (FieldParticle_HasPendingTarget(effect) != 0)
      return;
    *statePointer = *statePointer + 1;
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
    effect->turnStepMax = TRIG_TURN / 16;
    effect->turnStepLimited = 1;
    *statePointer = *statePointer + 1;
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

/// Throws an actor from the caster to the Psynergy's position over 10 frames
/// and scatters it into 16 particles.
void FieldPsynergy_Halt(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
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
    Sprite_SetAnimationColorModesFar(effect->actor, SPRITE_COLOR_WHITE);
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
    if (actor->spriteStorageKind == 1) {
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
  if (actor != 0) {
    if (actor->update == FieldPsynergy_UpdateHaltedActor) {
      struct MainRoutineWork *state = &gMainRoutineWork;
      actor->update = (void (*)(struct Actor *))state->pausedUpdate;
      state->pausedUpdate = 0;
      Actor_SetSpriteColorModeFar(actor, state->pausedPose);
    }
    actor->paused = 0;
    Actor_SetAnimationSpeedFar(actor, 16);
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
