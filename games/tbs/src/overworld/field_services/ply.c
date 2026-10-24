/// @file
/// Ply.
// Part of field_services.c.

#include "../field_services_internal.h"

/// Per-frame update: sways 4 pixels either side of anchorX, once every 128
/// frames. The update of each spark FieldEffect_RunSparkEmitter sheds; it
/// writes the spark's x and advances its phase.
void FieldEffect_SwayParticle(union ObjectEventOrEffect *fieldObject) {
  struct Object *obj = (struct Object *)fieldObject;
  s16 *phase;

  phase = &obj->action;
  obj->x = obj->targetX + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

/// Per-frame update: spirals in on a point 10 pixels above the effect's
/// position, at a radius of as many pixels as frames are left; released when
/// timer reaches 0. The update of FieldEffect_RunPly's eight spiralling
/// particles; each one's work holds its starting angle.
void FieldEffect_UpdateShrinkingOrbitObject(
    union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *particle = (struct ObjectEvent *)fieldObject;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Vector3 pos;
  s16 remainingFrames;
  s32 decrementedTimer;

  if (particle != NULL) {
    decrementedTimer = (u16)particle->timer - 1;
    particle->timer = decrementedTimer;
    remainingFrames = decrementedTimer;
    if (remainingFrames != 0) {
      // The angle turns 1/32 of a circle (0x800) each frame left.
      pos.x = effectWork->x;
      pos.y = effectWork->y + (10 << 16);
      pos.z = effectWork->z;
      Vector_AddPolarOffset(remainingFrames << 16,
                            (s16)particle->work + (remainingFrames << 11),
                            &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      Object_InitializeFar((struct Object *)particle,
                           gFieldEffect_CommonParticleScript);
    }
  }
}

/// Per-frame update: sways 6 pixels either side (a 64-frame cycle) 16 pixels
/// above the effect's position, and every 3 frames sheds a spark that sways
/// as it slowly falls. With phase -1 it stays where it is. The update
/// FieldEffect_RunPly gives the centre particle; spawns SPRITE_SPARKLE for each
/// spark.
void FieldEffect_RunSparkEmitter(union ObjectEventOrEffect *fieldObject) {
  struct Object *emitter = (struct Object *)fieldObject;
  struct FieldEffectWork *effectWork;
  s16 *phase;
  struct Object *spark;
  s32 pos[3];

  phase = &emitter->action;
  effectWork = gFieldEffectWork;
  if (*phase != -1) {
    SET_POSITION(emitter,
                 effectWork->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10)),
                 effectWork->y + ABOVE, effectWork->z);
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    // From 2 pixels up, 0 to 6 pixels in a random direction.
    SET_VECTOR(pos, emitter->x, emitter->y + (2 << 16), emitter->z);
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark =
        (struct Object *)Object_Spawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
    // Each falls under a gravity of 458 (0.007 pixels a frame, each frame)
    // and is freed after 72 frames, its script's wait (lifetime).
    if (spark != 0) {
      spark->update = FieldEffect_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999; // 0.6
      spark->motionFlags = OBJ_EVENT_FALLS;
      spark->gravity = 458;
      spark->action = Random16() >> 9;
      spark->targetX = spark->x;
      Anim_ApplyChildValuesFar((struct Object *)spark, SPRITE_COLOR_BLUE_TINT);
      spark->wait = 72;
      Object_InitializeFar((struct Object *)spark,
                           gFieldEffect_CommonParticleScript);
    }
  }
}

/// Runs FieldEffect_RunPly, then stops the caster's hook.
///
/// Abilities: Ply, Ply Well, Pure Ply (FIELD_PSYNERGY_PLY).
///
/// This is Ply cast on a target, which FieldEffect_DispatchRequestKind calls.
void FieldEffect_RunPlyOnTarget(s32 unused) {
  FieldEffect_RunPly();
  FieldEffect_StopCasterHook();
}

/// Eight particles spiral into the centre, where one appears, sheds sparks and
/// flies far away.
///
/// Abilities: Ply, Ply Well, Pure Ply (FIELD_PSYNERGY_PLY).
///
/// Called by FieldEffect_Run and FieldEffect_RunPlyOnTarget; frees the
/// particles' shared VRAM cache entry at the end and runs the effect
/// scene's finishCallback.
void FieldEffect_RunPly(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ObjectEvent *target = effectWork->mainObject;
  struct ObjectEvent *objectEvent;
  struct Sprite *last;
  u8 resourceSlot;
  s32 i;

  FieldEffect_FadeToEffectColor();

  // The spiralling particles: one a frame, each 1/8 turn on from the last.
  last = 0;
  for (i = 0; i < 8; i++) {
    objectEvent = (struct ObjectEvent *)Object_Spawn(
        233, target->x.fixed, target->y.fixed + 0x200000, target->z.fixed);
    if (objectEvent != 0) {
      objectEvent->scaleY = 0xb333; // 0.7
      objectEvent->scaleX = 0xb333;
      objectEvent->update = FieldEffect_UpdateShrinkingOrbitObject;
      objectEvent->timer = 120;
      objectEvent->work = i << 13;
      objectEvent->motionFlags = OBJ_EVENT_FLOATS;
      last = Sprite_ShareVramCacheEntry(objectEvent->sprite, last);
    }
    WaitFrames(1);
  }

  // The particles share one sprite cache; keep its slot to free at the end.
  // XXX: last is read with no check that any particle spawned; a map with
  // no free object slots would show whether this can be 0.
  resourceSlot = last->slot;
  Sound_PlaySongFar(SE_PSYNERGY_START);
  // The last particle spawned 7 frames ago with 120 frames to live: wait
  // until the spiral is nearly closed.
  WaitFrames(110);

  // The one in the centre.
  target = (struct ObjectEvent *)Object_Spawn(233, 0, 0, 0);
  objectEvent = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    target->x.fixed = effectWork->x;
    target->y.fixed = effectWork->y + 0x100000;
    target->z.fixed = effectWork->z;
    target->motionFlags = OBJ_EVENT_FLOATS;
    Anim_ApplyChildValuesFar((struct Object *)target, SPRITE_COLOR_WHITE);
  }

  Sound_PlaySongFar(SE_PSYNERGY_SPARKLE);
  WaitFrames(12);
  if (target != 0) {
    // Switch its sprite param every 2 frames (10 one time in 4).
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH(temporary): a redundant copy keeps object as a second
       * register copy of the spark (r5 beside r6); without it, 129 of 177
       * instructions differ in tbs-en. */
      target = objectEvent;
      if (i & 3)
        Anim_ApplyChildValuesFar((struct Object *)objectEvent,
                                 SPRITE_COLOR_BLUE_TINT);
      else
        Anim_ApplyChildValuesFar((struct Object *)objectEvent,
                                 SPRITE_COLOR_RED_TINT);
      WaitFrames(2);
    }
  }

  // Sparks.
  Anim_ApplyChildValuesFar((struct Object *)objectEvent, SPRITE_COLOR_NORMAL);
  Sound_PlaySongFar(MUS_CURE);
  if (objectEvent != 0) {
    target->update = FieldEffect_RunSparkEmitter;
    target->timer = 0;
    // Cast on a target (enabled cleared by FieldEffect_DispatchRequestKind),
    // the sparks fall for 192 frames instead of 128.
    if (effectWork->enabled != 0)
      WaitFrames(128);
    else
      WaitFrames(192);
  }

  // Fly away: 192 pixels at angle 0xe800, and release it once it arrives.
  // Its timer is the emitter's phase: -1 stops the sway that would hold it
  // in place.
  if (target != 0) {
    target->timer = -1;
    target->speed = 0x50000;
    target->acceleration = 0x6666;
    target->steeringFlags = 0;
    ObjectMotion_SetTargetPositionFromMagnitudeAngle((struct Object *)target,
                                                     0xc00000, 0xe800);
    Object_WaitForMoveFar(target);
    Object_ReleaseFar((struct Object *)target);
  }
  // 0x60 is the VRAM cache's entry count, the value VramCache_FindFreeEntry
  // returns for no entry.
  if (resourceSlot != 0x60)
    VramCache_FreeEntry(resourceSlot);
  if (effectWork->finishCallback != 0)
    effectWork->finishCallback();
  FieldEffect_FadeFromEffectColor();
}
