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
/// of the caster with effectWork->enabled (no trigger named the target),
/// otherwise the effect's position, 32 pixels up in both cases. Called by
/// FieldEffect_Run and FieldEffect_RunCatchOnTarget. Moves the field effect's
/// child to (-9, -9) when there is one, and releases object 215 at the end.
void FieldEffect_RunCatch(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *actor = effectWork->mainActor;
  struct Actor *child = effectWork->child;
  struct Actor *object;
  struct FixedPointPosition spawn;
  struct FixedPointPosition origin;
  struct FixedPointPosition target;
  s32 step;
  s32 i;
  s32 steps = 11;

  origin.x = actor->x.fixed;
  origin.y = actor->y.fixed + (16 << 16);
  origin.z = actor->z.fixed;
  if (effectWork->enabled) {
    target.x = actor->x.fixed;
    target.y = actor->y.fixed + (32 << 16);
    target.z = actor->z.fixed;
    Vector_AddPolarOffset(32 << 16, effectWork->angle, &target.x);
  } else {
    target.x = effectWork->x;
    target.y = effectWork->y + (32 << 16);
    target.z = effectWork->z;
  }
  spawn.x = effectWork->x;
  spawn.y = effectWork->y + (32 << 16);
  spawn.z = effectWork->z;
  object = Actor_Spawn(SPRITE_MOVE_HAND, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  FieldEffect_FadeToEffectColor();
  Sound_PlaySongFar(SE_ITEM_BREAK);
  object->facing = actor->facing;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  Actor_SelectAnimationFar(object, 5);
  Anim_ApplyChildValuesFar(object, SPRITE_COLOR_GRB);

  // Reach out to the goal: steps 0 to 10 of an 11-step loop, a tenth of the
  // way each frame.
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(object, origin.x + (target.x - origin.x) * step / 10,
                       origin.y + (target.y - origin.y) * step / 10,
                       origin.z + (target.z - origin.z) * step / 10);
    scale = FieldEffect_Interpolate(Q16_ONE, 0x4000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  WaitFrames(10);
  Actor_SelectAnimationFar(object, 6);
  WaitFrames(15);

  // Press down 2 pixels a frame for 10 frames.
  for (i = 0; i < 10; i++) {
    object->y.fixed -= 2 << 16;
    WaitFrames(1);
  }
  Actor_SelectAnimationFar(object, 5);
  Sound_PlaySongFar(SE_WATER);
  // As the hand presses down, the caught object vanishes: (-9, -9) pixels is
  // just off the map's corner, where no view reaches. The map's after event
  // then puts it away for good, as Vale's EventScript_CatchNut moves the Nut
  // to (0, 0) and sets FLAG_VALE_NUT_CAUGHT so that it is not placed again.
  if (child != 0)
    Actor_SetPositionAndResetMotionFar(child, -(9 << 16), child->y.fixed,
                                       -(9 << 16));
  WaitFrames(20);

  // Rise 1.5 pixels a frame for 13 frames.
  for (i = 0; i < 13; i++) {
    object->y.fixed += 0x18000;
    WaitFrames(1);
  }
  WaitFrames(10);
  Sound_PlaySongFar(SE_MENU_ERROR);

  // Shrink back to the caster.
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(object, target.x + (origin.x - target.x) * step / 10,
                       target.y + (origin.y - target.y) * step / 10,
                       target.z + (origin.z - target.z) * step / 10);
    scale = FieldEffect_Interpolate(0x4000, Q16_ONE, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  Actor_ReleaseFar(object);
  FieldEffect_FadeFromEffectColor();
}
