/// @file
/// Carry: two hands lift the target, move it with the d-pad and drop it.
// Part of event_commands.c.

#include "../event_commands_internal.h"

STATIC_ASSERT(sizeof(struct Vector3) == 0x0c);

/// Task while Carry holds its target: drops one particle a
/// frame round the lifted actor, 8 to 24 pixels above it and 0 to 48 pixels
/// away in a random direction (FieldEffect_InitializeFallingFragment).
void FieldEffect_SpawnFallingParticles(void) {
  struct Actor *liftedActor;
  struct Vector3 position;
  struct Actor *fragment;
  u32 radius;

  liftedActor = ((struct FieldEffectWork *)gFieldEffectWork)->child;
  position.x = liftedActor->x.fixed;
  position.y = liftedActor->y.fixed - Random16() * 16 + (24 << 16);
  position.z = liftedActor->z.fixed;

  radius = Random16() * 3;
  Vector_AddPolarOffset(radius * 16, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

#define CARRY_SHARD_MAX 20 ///< shards

/// Tests the target's move with its ground height raised 16 pixels, so a
/// carried target passes over ground up to 24 pixels above its own (a step
/// allows 8). Restores the height before returning the collision result.
static __inline__ s32
FieldEffect_CheckRaisedTargetCollision(struct Actor *target,
                                       struct Vector3 *position) {
  s32 collision;

  target->groundHeight += 16 << 16;
  collision =
      MapGrid_CheckCollisionFar(target, (struct FieldPosition *)position);
  target->groundHeight -= 16 << 16;
  return collision;
}

/// When Carry is cast on a target, two hands lift it to 24 pixels above the
/// ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it
/// where it is.
///
/// Abilities: Carry (FIELD_PSYNERGY_CARRY).
///
/// Where the way is blocked a buzzer sounds.
void FieldPsynergy_SteerLiftedTarget(s32 targetId) {
  struct FieldEffectWork *effectWork;
  struct Actor *src;
  struct Actor *target;
  struct Actor *shards[CARRY_SHARD_MAX];
  struct Vector3 pos;
  struct Actor *hands[2];
  struct Actor *actor;
  struct Actor *right;
  s32 i;
  s32 angle;
  s32 dist;

  effectWork = gFieldEffectWork;
  src = effectWork->mainActor;
  target = effectWork->child;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  src->watchedActor = target;
  Actor_StartScriptFar(src, gFieldEffect_SourceHoldScript);

  // The hands appear 32 pixels to either side and move to 16 pixels.
  SET_POSITION(&pos, effectWork->x, effectWork->y + (16 << 16), effectWork->z);
  actor = FieldEffect_SpawnItemBreakMode1(pos.x + (32 << 16), pos.y, pos.z,
                                          FACING_WEST);
  hands[0] = actor;
  right = FieldEffect_SpawnItemBreakMode1(pos.x - (32 << 16), pos.y, pos.z, 0);
  hands[1] = right;
  if (hands[0] == NULL || right == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  wait(15);
  SET_POSITION(&pos, target->x.fixed, target->y.fixed + (16 << 16),
               target->z.fixed);
  Actor_SetMoveTargetFar(actor, pos.x + (16 << 16), pos.y, pos.z);
  Actor_SetMoveTargetFar(right, pos.x - (16 << 16), pos.y, pos.z);
  Actor_WaitForMoveFar(actor);
  Actor_WaitForMoveFar(right);
  actor->x.fixed = pos.x + (16 << 16);
  actor->velocityX = 0;
  right->x.fixed = pos.x - (16 << 16);
  right->velocityX = 0;

  // Lift to 24 pixels above the ground, 3/8 pixel a frame, floating (motion
  // flag bit 2) with no shadow and its colours cycling.
  target->update = Actor_CycleSpriteColors;
  Task_Add(FieldEffect_SpawnFallingParticles, TASK_PASS_FRAME);
  Sound_PlayFar(SE_PSYNERGY_START);
  target->motionFlags = 4;
  Actor_SetSpriteFlagsFar(target, 0);
  if (hands[0] != NULL && hands[1] != NULL) {
    while (target->y.fixed - target->groundHeight <= 24 << 16) {
      actor->y.fixed += Q16_ONE * 3 / 8;
      right->y.fixed += Q16_ONE * 3 / 8;
      target->y.fixed += Q16_ONE * 3 / 8;
      wait(1);
    }
  }
  hands[0]->speedLimit = 4 << 16;
  hands[0]->acceleration = Q16_ONE / 2;
  hands[1]->speedLimit = 4 << 16;
  hands[1]->acceleration = Q16_ONE / 2;
  // As in Move: the target keeps its facing and reads its ground from layer
  // 2 while it is carried.
  target->speedLimit = Q16_ONE * 2 / 5;
  target->acceleration = Q16_ONE / 5;
  target->steeringFlags = 0;
  target->layer = 2;

  //------------------------------------------------------------------
  // Move with the d-pad.
  //------------------------------------------------------------------
  for (;;) {
    wait(1);
    if ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;

    // No direction held: the hands go back to the target's sides.
    angle = (u16)Input_GetDpadAngle(gKeysHeld);
    if (angle == DPAD_ANGLE_NONE) {
      pos.x = target->x.fixed;
      pos.y = target->y.fixed + (16 << 16);
      pos.z = target->z.fixed;
      Actor_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
      Actor_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
      Actor_SelectAnimationFar(hands[0], 1);
      Actor_SelectAnimationFar(hands[1], 1);
      continue;
    }

    // The hands lean 2 pixels that way; check the ground 16 pixels on.
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + (16 << 16);
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(2 << 16, angle, &pos);
    Actor_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
    Actor_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
    Actor_WaitForMoveFar(hands[0]);
    Actor_WaitForMoveFar(hands[1]);
    pos.x = target->x.fixed;
    pos.y = target->groundHeight;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &pos);
    i = ActorScript_CheckOverlapFar(target, (s32 *)&pos);
    // Both tests jump into the blocked path, which keeps it ahead of the drop
    // path as the ROM lays them out; written as one || test, 407 of 531
    // instructions differ in tbs-en.
    if (i != 0)
      goto blocked;
    if (FieldEffect_CheckRaisedTargetCollision(target, &pos) > 0) {
    blocked: // the way is blocked: a buzzer every 16 frames
      Actor_SelectAnimationFar(hands[0], 4);
      Actor_SelectAnimationFar(hands[1], 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    // The way is free: move the target there.
    {
      struct Actor *second;
      s32 x;
      s32 z;

      Sound_PlayFar(SE_NOTE_HIGH);
      x = pos.x;
      z = pos.z;
      Actor_SelectAnimationFar(hands[0], 4);
      Actor_SelectAnimationFar(hands[1], 4);
      wait(15);
      // i is the overlap test's 0 here: unpause the target and, once it is
      // on the cell, stop it dead. It moves at 0.2 pixels a frame.
      target->paused = i;
      target->speedLimit = Q16_ONE / 5;
      target->acceleration = Q16_ONE / 5;
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
      hands[0]->speedLimit = Q16_ONE / 5;
      hands[0]->acceleration = Q16_ONE / 5;
      second = hands[1];
      second->speedLimit = Q16_ONE / 5;
      second->acceleration = Q16_ONE / 5;
      Actor_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
      Actor_SetMoveTargetFar(second, pos.x - (16 << 16), pos.y, pos.z);
      Actor_WaitForMoveFar(target);
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = i;
      target->velocityZ = i;
      wait(10);
      break;
    }
  }

  //------------------------------------------------------------------
  // Shards fly from the target, the hands vanish, and the target hops and
  // drops.
  //------------------------------------------------------------------
  Actor_SelectAnimationFar(hands[0], 4);
  Actor_SelectAnimationFar(hands[1], 4);
  Task_Remove(FieldEffect_SpawnFallingParticles);
  Sound_PlayFar(SE_CLICK);
  wait(15);
  Sound_PlayFar(SE_CLICK);
  wait(15);
  SET_POSITION(&pos, target->x.fixed, target->y.fixed + (16 << 16),
               target->z.fixed);
  // Shards: 2 to 3 pixels a frame, 8 to 32 pixels in a random direction.
  for (i = 0; i < CARRY_SHARD_MAX; i++) {
    actor = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    shards[i] = actor;
    if (actor != NULL) {
      Actor_StartScriptFar(actor, gFieldEffect_FragmentScript);
      actor->speedLimit = Random16() + (2 << 16);
      actor->acceleration = 2 << 16;
      actor->motionFlags = 0;
      dist = Random16() * 24 + (8 << 16);
      Actor_SetTargetPositionFromMagnitudeAngle(actor, dist, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(hands[0]);
  Actor_ReleaseFar(hands[1]);

  // Restore the target's look and motion, and throw it up at 10 pixels a
  // frame: it falls and follows the ground again, keeping 0.2 of its speed
  // as it bounces, and reads its ground from the ground layer.
  Actor_SetSpriteColorModeFar(target, effectWork->savedPalette);
  Actor_StartScriptFar(target, effectWork->savedScript);
  target->update = effectWork->savedCallback;
  target->motionFlags = 3;
  target->velocityY = 10 << 16;
  target->bounce = Q16_ONE / 5;
  target->layer = 0;
  src->update = 0;
  Actor_SetSpriteColorModeFar(src, SPRITE_COLOR_NORMAL);
  if (effectWork->childOption != 0) {
    // Wait for it to rise and land (at most 90 frames each), then a ring.
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

/// When Carry is cast with no target, two hands appear 16 pixels above the spot
/// in front of the caster and 32 pixels to either side, close in 24 pixels and
/// scatter into shards.
///
/// Abilities: Carry (FIELD_PSYNERGY_CARRY).
DEFINE_ITEM_BREAK_EFFECT(FieldPsynergy_Carry, FieldEffect_SpawnItemBreakMode1)

/// Spawns a hand (SPRITE_MOVE_HAND) with the item break sound, in animation 1,
/// and grows it, spinning 1/8 turn a frame, from a quarter to full size over 24
/// frames; angle is the way it faces once grown. Returns the hand, or NULL
/// when it cannot be spawned (Actor_Spawn); Carry and its no-target cast
/// release it.
struct Actor *FieldEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *fragment;
  s32 scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  fragment = Actor_Spawn(SPRITE_MOVE_HAND, x, y, z);
  if (fragment != NULL) {
    fragment->scaleY = Q16_ONE / 4;
    fragment->scaleX = Q16_ONE / 4;
    fragment->speedLimit = PIXELS(3);
    fragment->acceleration = PIXELS(3);
    fragment->steeringFlags = 0;
    Actor_SelectAnimationFar(fragment, 1);
    scale = fragment->scaleX;
    if (scale < Q16_ONE) {
      do {
        scale += Q16_ONE / 32;
        fragment->scaleY = scale;
        fragment->scaleX = scale;
        fragment->facing += TRIG_TURN / 8;
        wait(1);
        scale = fragment->scaleX;
      } while (scale <= 0xffff);
    }
    fragment->facing = angle;
  }
  return fragment;
}

#undef CARRY_SHARD_MAX
