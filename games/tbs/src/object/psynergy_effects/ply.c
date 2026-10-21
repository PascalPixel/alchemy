/// @file
/// Ply.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Per-frame update: sways 4 pixels either side of anchorX, once every 128
/// frames. The update of each spark FieldFx_RunSparkEmitter sheds; it writes
/// the spark's x and advances its phase.
void FieldFx_SwayParticle(union FieldObject *fieldObject) {
  struct SparkObject *obj = (struct SparkObject *)fieldObject;
  s16 *phase;

  phase = &obj->phase;
  obj->x = obj->anchorX + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

/// Per-frame update: spirals in on a point 10 pixels above the effect's
/// position, at a radius of as many pixels as frames are left; released when
/// timer reaches 0. The update of FieldFx_RunPly's eight spiralling particles;
/// each one's work holds its starting angle.
void FieldFx_UpdateShrinkingOrbitObject(union FieldObject *fieldObject) {
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  struct FieldFxScene *scene = gEffectWork;
  struct EffectVector pos;
  s16 remainingFrames;
  s32 decrementedTimer;

  if (particle != NULL) {
    decrementedTimer = (u16)particle->timer - 1;
    particle->timer = decrementedTimer;
    remainingFrames = decrementedTimer;
    if (remainingFrames != 0) {
      // The angle turns 1/32 of a circle (0x800) each frame left.
      pos.x = scene->x;
      pos.y = scene->y + (10 << 16);
      pos.z = scene->z;
      Vector_AddPolarOffset(remainingFrames << 16,
                            (s16)particle->work + (remainingFrames << 11),
                            &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                   gFieldFx_CommonParticleScript);
    }
  }
}

/// Per-frame update: sways 6 pixels either side (a 64-frame cycle) 16 pixels
/// above the effect's position, and every 3 frames sheds a spark that sways
/// as it slowly falls. With phase -1 it stays where it is. The update
/// FieldFx_RunPly gives the centre particle; spawns SPRITE_SPARKLE for each
/// spark.
void FieldFx_RunSparkEmitter(union FieldObject *fieldObject) {
  struct SparkObject *emitter = (struct SparkObject *)fieldObject;
  struct FieldFxScene *scene;
  s16 *phase;
  struct SparkObject *spark;
  s32 pos[3];

  phase = &emitter->phase;
  scene = gEffectWork;
  if (*phase != -1) {
    SET_POSITION(emitter,
                 scene->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10)),
                 scene->y + ABOVE, scene->z);
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    // From 2 pixels up, 0 to 6 pixels in a random direction.
    SET_VECTOR(pos, emitter->x, emitter->y + (2 << 16), emitter->z);
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark = (struct SparkObject *)Object_Spawn(SPRITE_SPARKLE, pos[0], pos[1],
                                               pos[2]);
    // Each falls under a gravity of 458 (0.007 pixels a frame, each frame)
    // and is freed after 72 frames, its script's wait (lifetime).
    if (spark != 0) {
      spark->update = FieldFx_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999; // 0.6
      spark->motionFlags = ACTOR_FALLS;
      spark->gravity = 458;
      spark->phase = Random16() >> 9;
      spark->anchorX = spark->x;
      Animation_ApplyChildValuesFar((struct DispatchObject *)spark,
                                    SPRITE_COLOR_BLUE_TINT);
      spark->lifetime = 72;
      ObjectDispatch_InitializeFar((struct DispatchObject *)spark,
                                   gFieldFx_CommonParticleScript);
    }
  }
}

/// Runs FieldFx_RunPly, then stops the caster's hook.
///
/// Abilities: Ply, Ply Well, Pure Ply (FIELD_KIND_PLY).
///
/// This is Ply cast on a target, which FieldFx_DispatchRequestKind calls.
void FieldFx_RunPlyOnTarget(s32 unused) {
  FieldFx_RunPly();
  FieldFx_StopCasterHook();
}

/// Eight particles spiral into the centre, where one appears, sheds sparks and
/// flies far away.
///
/// Abilities: Ply, Ply Well, Pure Ply (FIELD_KIND_PLY).
///
/// Called by FieldFx_Run and FieldFx_RunPlyOnTarget; frees the particles'
/// shared sprite resource slot at the end and runs the effect scene's
/// finishCallback.
void FieldFx_RunPly(void) {
  struct FieldFxScene *scene = gEffectWork;
  struct FieldActor *target = scene->mainObject;
  struct FieldActor *object;
  struct AnimationObject *last;
  u8 resourceSlot;
  s32 i;

  FieldFx_FadeToEffectColor();

  // The spiralling particles: one a frame, each 1/8 turn on from the last.
  last = 0;
  for (i = 0; i < 8; i++) {
    object = (struct FieldActor *)Object_Spawn(
        233, target->x.fixed, target->y.fixed + 0x200000, target->z.fixed);
    if (object != 0) {
      object->scaleY = 0xb333; // 0.7
      object->scaleX = 0xb333;
      object->update = FieldFx_UpdateShrinkingOrbitObject;
      object->timer = 120;
      object->work = i << 13;
      object->motionFlags = ACTOR_FLOATS;
      last = Sprite_ShareVramCache(object->sprite, last);
    }
    WaitFrames(1);
  }

  // The particles share one sprite cache; keep its slot to free at the end.
  // XXX: last is read with no check that any particle spawned; a scene with
  // no free object slots would show whether this can be 0.
  resourceSlot = last->slot;
  AudioCommand_PlayFar(SE_130);
  // The last particle spawned 7 frames ago with 120 frames to live: wait
  // until the spiral is nearly closed.
  WaitFrames(110);

  // The one in the centre.
  target = (struct FieldActor *)Object_Spawn(233, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    target->x.fixed = scene->x;
    target->y.fixed = scene->y + 0x100000;
    target->z.fixed = scene->z;
    target->motionFlags = ACTOR_FLOATS;
    Animation_ApplyChildValuesFar((struct DispatchObject *)target,
                                  SPRITE_COLOR_WHITE);
  }

  AudioCommand_PlayFar(SE_131);
  WaitFrames(12);
  if (target != 0) {
    // Switch its sprite param every 2 frames (10 one time in 4).
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH(temporary): a redundant copy keeps object as a second
       * register copy of the spark (r5 beside r6); without it, 129 of 177
       * instructions differ in tbs-en. */
      target = object;
      if (i & 3)
        Animation_ApplyChildValuesFar((struct DispatchObject *)object,
                                      SPRITE_COLOR_BLUE_TINT);
      else
        Animation_ApplyChildValuesFar((struct DispatchObject *)object,
                                      SPRITE_COLOR_RED_TINT);
      WaitFrames(2);
    }
  }

  // Sparks.
  Animation_ApplyChildValuesFar((struct DispatchObject *)object,
                                SPRITE_COLOR_NORMAL);
  AudioCommand_PlayFar(MUS_084);
  if (object != 0) {
    target->update = FieldFx_RunSparkEmitter;
    target->timer = 0;
    // Cast on a target (enabled cleared by FieldFx_DispatchRequestKind), the
    // sparks fall for 192 frames instead of 128.
    if (scene->enabled != 0)
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
    Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)target,
                                               0xc00000, 0xe800);
    FieldObject_WaitForMoveFar(target);
    ObjectDispatch_ReleaseFar((struct DispatchObject *)target);
  }
  // 0x60 is the VRAM cache's entry count, the value Resource_FindFreeEntry
  // returns for no entry.
  if (resourceSlot != 0x60)
    Resource_ResetEntry(resourceSlot);
  if (scene->finishCallback != 0)
    scene->finishCallback();
  FieldFx_FadeFromEffectColor();
}
