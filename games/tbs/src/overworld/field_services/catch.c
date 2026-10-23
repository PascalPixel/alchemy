/// @file
/// Catch.
// Part of field_services.c.

#include "../field_services_internal.h"

/// Runs Catch cast on a target, through FieldEffect_RunCatch.
///
/// Abilities: Catch (FIELD_PSYNERGY_CATCH).
///
/// Called by FieldEffect_DispatchRequestKind; the target is ignored.
void FieldEffect_RunCatchOnTarget(s32 unused) { FieldEffect_RunCatch(); }

/// Carries object 215 from 16 pixels above the caster to the goal in 10 steps,
/// growing from a quarter to full size, presses down 20 pixels moving child to
/// (-9, -9), rises 19.5 pixels and shrinks back.
///
/// Abilities: Catch (FIELD_PSYNERGY_CATCH).
///
/// Object 215 is the hand of Move and Carry. The goal is 32 pixels in front
/// of the caster with scene->enabled (no map event named the target),
/// otherwise the effect's position, 32 pixels up in both cases. Called by
/// FieldEffect_Run and FieldEffect_RunCatchOnTarget. Moves the effect scene's
/// child to (-9, -9) when there is one, and releases object 215 at the end.
void FieldEffect_RunCatch(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Object *objectEvent = effectWork->mainObject;
  struct Object *child = effectWork->child;
  struct Object *object;
  struct FixedPointPosition spawn;
  struct FixedPointPosition origin;
  struct FixedPointPosition target;
  s32 step;
  s32 i;
  s32 steps = 11;

  origin.x = objectEvent->x;
  origin.y = objectEvent->y + (16 << 16);
  origin.z = objectEvent->z;
  if (effectWork->enabled) {
    target.x = objectEvent->x;
    target.y = objectEvent->y + (32 << 16);
    target.z = objectEvent->z;
    Vector_AddPolarOffset(32 << 16, effectWork->angle, &target.x);
  } else {
    target.x = effectWork->x;
    target.y = effectWork->y + (32 << 16);
    target.z = effectWork->z;
  }
  spawn.x = effectWork->x;
  spawn.y = effectWork->y + (32 << 16);
  spawn.z = effectWork->z;
  object = (struct Object *)Object_Spawn(215, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  FieldEffect_FadeToEffectColor();
  Sound_PlaySongFar(SE_ITEM_BREAK);
  object->facing = objectEvent->facing;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  ObjectDispatch_SelectAnimationFar(object, 5);
  Anim_ApplyChildValuesFar((struct Object *)object, SPRITE_COLOR_GRB);

  // Reach out to the goal: steps 0 to 10 of an 11-step loop, a tenth of the
  // way each frame.
  step = 0;
  do {
    s32 scale;

    SET_POSITION(object, origin.x + (target.x - origin.x) * step / 10,
                 origin.y + (target.y - origin.y) * step / 10,
                 origin.z + (target.z - origin.z) * step / 10);
    scale = FieldEffect_Interpolate(0x10000, 0x4000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  WaitFrames(10);
  ObjectDispatch_SelectAnimationFar(object, 6);
  WaitFrames(15);

  // Press down 2 pixels a frame for 10 frames.
  for (i = 0; i < 10; i++) {
    object->y -= 2 << 16;
    WaitFrames(1);
  }
  ObjectDispatch_SelectAnimationFar(object, 5);
  Sound_PlaySongFar(SE_132);
  // XXX: why the caught object goes to (-9, -9) pixels, just off the map's
  // corner; the map code that reads it after Catch would settle it.
  if (child != 0)
    Object_SetPositionAndResetMotionFar((struct Object *)child, -(9 << 16),
                                        child->y, -(9 << 16));
  WaitFrames(20);

  // Rise 1.5 pixels a frame for 13 frames.
  for (i = 0; i < 13; i++) {
    object->y += 0x18000;
    WaitFrames(1);
  }
  WaitFrames(10);
  Sound_PlaySongFar(SE_MENU_ERROR);

  // Shrink back to the caster.
  step = 0;
  do {
    s32 scale;

    SET_POSITION(object, target.x + (origin.x - target.x) * step / 10,
                 target.y + (origin.y - target.y) * step / 10,
                 target.z + (origin.z - target.z) * step / 10);
    scale = FieldEffect_Interpolate(0x4000, 0x10000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ObjectDispatch_ReleaseFar((struct Object *)object);
  FieldEffect_FadeFromEffectColor();
}
