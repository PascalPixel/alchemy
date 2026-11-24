/// @file
/// Carry: two hands lift the target, move it with the d-pad and drop it.
// Part of event_commands.c.

#include "event_commands_internal.h"

STATIC_ASSERT(sizeof(struct Vector3) == 12);

/// Task while Carry holds its target: drops one particle a
/// frame round the lifted actor, 8 to 24 pixels above it and 0 to 48 pixels
/// away in a random direction (FieldEffect_InitializeFallingFragment).
void FieldEffect_SpawnFallingParticles(void) {
  struct Actor *liftedActor;
  struct Vector3 position;
  struct Actor *fragment;
  u32 radius;

  liftedActor = ((struct FieldEffectState *)gFieldEffectState)->target;
  position.x = liftedActor->x.fixed;
  position.y = liftedActor->y.fixed - Random16() * 16 + toFIXED(24);
  position.z = liftedActor->z.fixed;

  radius = Random16() * 3;
  Vector_AddPolarOffset(radius * 16, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

/// The sparkles Carry scatters from the target, 8 to 32 pixels out.
#define CARRY_SPARKLE_COUNT 20

/// Tests the target's move with its ground height raised 16 pixels, so a
/// carried target passes over ground up to 24 pixels above its own (a step
/// allows 8). Restores the height before returning the collision result.
static __inline__ s32
FieldEffect_CheckRaisedTargetCollision(struct Actor *target,
                                       struct Vector3 *position) {
  s32 collision;

  target->groundHeight += toFIXED(16);
  collision =
      MapGrid_CheckCollisionFar(target, (struct FieldPosition *)position);
  target->groundHeight -= toFIXED(16);
  return collision;
}

/// When Carry is cast on a target, two hands lift it to 24 pixels above the
/// ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it
/// where it is.
void FieldPsynergy_SteerLiftedTarget(s32 targetId) {
  struct FieldEffectState *effectState;
  struct Actor *caster;
  struct Actor *target;
  struct Actor *sparkles[CARRY_SPARKLE_COUNT];
  struct Vector3 pos;
  struct Actor *hands[2];
  struct Actor *actor;
  struct Actor *westHand;
  s32 i;
  s32 angle;
  FIXED dist;

  effectState = gFieldEffectState;
  caster = effectState->caster;
  target = effectState->target;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  caster->watchedActor = target;
  Actor_StartScriptFar(caster, gFieldEffect_SourceHoldScript);

  // The hands appear 32 pixels to either side and move to 16 pixels: the east
  // one, hands[0], is held in actor until the sparkles.
  SET_POSITION(&pos, effectState->x, effectState->y + toFIXED(16),
               effectState->z);
  actor = FieldEffect_SpawnCarryHand(pos.x + toFIXED(32), pos.y, pos.z,
                                     FACING_WEST);
  hands[0] = actor;
  westHand = FieldEffect_SpawnCarryHand(pos.x - toFIXED(32), pos.y, pos.z,
                                        FACING_EAST);
  hands[1] = westHand;
  if (hands[0] == NULL || westHand == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  wait(15);
  SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  Actor_SetMoveTargetFar(actor, pos.x + toFIXED(16), pos.y, pos.z);
  Actor_SetMoveTargetFar(westHand, pos.x - toFIXED(16), pos.y, pos.z);
  Actor_WaitForMoveFar(actor);
  Actor_WaitForMoveFar(westHand);
  actor->x.fixed = pos.x + toFIXED(16);
  actor->velocityX = 0;
  westHand->x.fixed = pos.x - toFIXED(16);
  westHand->velocityX = 0;

  // Lift to 24 pixels above the ground, 3/8 pixel a frame, floating with no
  // shadow and its colours cycling.
  target->update = Actor_CycleSpriteColors;
  Task_Add(FieldEffect_SpawnFallingParticles, TASK_PASS_FRAME);
  Sound_PlayFar(SE_PSYNERGY_START);
  target->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlagsFar(target, 0);
  if (hands[0] != NULL && hands[1] != NULL) {
    while (target->y.fixed - target->groundHeight <= toFIXED(24)) {
      actor->y.fixed += toFIXED(3) / 8;
      westHand->y.fixed += toFIXED(3) / 8;
      target->y.fixed += toFIXED(3) / 8;
      wait(1);
    }
  }
  hands[0]->speedLimit = toFIXED(4);
  hands[0]->acceleration = toFIXED(1) / 2;
  hands[1]->speedLimit = toFIXED(4);
  hands[1]->acceleration = toFIXED(1) / 2;
  // As in Move: the target keeps its facing and reads its ground from the
  // overhead layer while it is carried.
  target->speedLimit = toFIXED(2) / 5;
  target->acceleration = toFIXED(1) / 5;
  target->steeringFlags = 0;
  target->layer = MAPGRID_LAYER_OVERHEAD;

  // Move with the d-pad.
  for (;;) {
    wait(1);
    if ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;

    // No direction held: the hands go back to the target's sides.
    angle = (u16)Input_GetDpadAngle(gKeysHeld);
    if (angle == DPAD_ANGLE_NONE) {
      pos.x = target->x.fixed;
      pos.y = target->y.fixed + toFIXED(16);
      pos.z = target->z.fixed;
      Actor_SetMoveTargetFar(hands[0], pos.x + toFIXED(16), pos.y, pos.z);
      Actor_SetMoveTargetFar(hands[1], pos.x - toFIXED(16), pos.y, pos.z);
      Actor_SelectAnimationFar(hands[0], MOVE_HAND_ANIMATION_OPEN);
      Actor_SelectAnimationFar(hands[1], MOVE_HAND_ANIMATION_OPEN);
      continue;
    }

    // The hands lean 2 pixels that way; check the ground 16 pixels on.
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + toFIXED(16);
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(2), angle, &pos);
    Actor_SetMoveTargetFar(hands[0], pos.x + toFIXED(16), pos.y, pos.z);
    Actor_SetMoveTargetFar(hands[1], pos.x - toFIXED(16), pos.y, pos.z);
    Actor_WaitForMoveFar(hands[0]);
    Actor_WaitForMoveFar(hands[1]);
    pos.x = target->x.fixed;
    pos.y = target->groundHeight;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), angle, &pos);
    i = ActorScript_CheckOverlapFar(target, (s32 *)&pos);
    // A solid actor or raised ground in the way: a buzzer every 16 frames.
    if (i != 0) {
      Actor_SelectAnimationFar(hands[0], MOVE_HAND_ANIMATION_FLEX);
      Actor_SelectAnimationFar(hands[1], MOVE_HAND_ANIMATION_FLEX);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    if (FieldEffect_CheckRaisedTargetCollision(target, &pos) > 0) {
      Actor_SelectAnimationFar(hands[0], MOVE_HAND_ANIMATION_FLEX);
      Actor_SelectAnimationFar(hands[1], MOVE_HAND_ANIMATION_FLEX);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    // The way is free: move the target there.
    {
      struct Actor *second;
      FIXED x;
      FIXED z;

      Sound_PlayFar(SE_NOTE_HIGH);
      x = pos.x;
      z = pos.z;
      Actor_SelectAnimationFar(hands[0], MOVE_HAND_ANIMATION_FLEX);
      Actor_SelectAnimationFar(hands[1], MOVE_HAND_ANIMATION_FLEX);
      wait(15);
      // i is the overlap test's 0 here: unpause the target and, once it is
      // on the cell, stop it dead. It moves at 0.2 pixels a frame.
      target->paused = i;
      target->speedLimit = toFIXED(1) / 5;
      target->acceleration = toFIXED(1) / 5;
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
      hands[0]->speedLimit = toFIXED(1) / 5;
      hands[0]->acceleration = toFIXED(1) / 5;
      second = hands[1];
      second->speedLimit = toFIXED(1) / 5;
      second->acceleration = toFIXED(1) / 5;
      Actor_SetMoveTargetFar(hands[0], pos.x + toFIXED(16), pos.y, pos.z);
      Actor_SetMoveTargetFar(second, pos.x - toFIXED(16), pos.y, pos.z);
      Actor_WaitForMoveFar(target);
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = i;
      target->velocityZ = i;
      wait(10);
      break;
    }
  }

  // Sparkles fly from the target, the hands vanish, and the target hops and
  // drops.
  Actor_SelectAnimationFar(hands[0], MOVE_HAND_ANIMATION_FLEX);
  Actor_SelectAnimationFar(hands[1], MOVE_HAND_ANIMATION_FLEX);
  Task_Remove(FieldEffect_SpawnFallingParticles);
  Sound_PlayFar(SE_CLICK);
  wait(15);
  Sound_PlayFar(SE_CLICK);
  wait(15);
  SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  // Sparkles: 2 to 3 pixels a frame, 8 to 32 pixels in a random direction.
  for (i = 0; i < CARRY_SPARKLE_COUNT; i++) {
    actor = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    // DEAD: nothing reads the sparkles back; the ROM keeps the stores.
    sparkles[i] = actor;
    if (actor != NULL) {
      Actor_StartScriptFar(actor, gFieldEffect_FragmentScript);
      actor->speedLimit = Random16() + toFIXED(2);
      actor->acceleration = toFIXED(2);
      actor->motionFlags = 0;
      dist = Random16() * 24 + toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(actor, dist, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(hands[0]);
  Actor_ReleaseFar(hands[1]);

  // Restore the target's look and motion, and throw it up at 10 pixels a
  // frame: it falls and follows the ground again, keeping 0.2 of its speed
  // as it bounces, and reads its ground from the ground layer.
  Actor_SetSpriteColorModeFar(target, effectState->savedPalette);
  Actor_StartScriptFar(target, effectState->savedScript);
  target->update = effectState->savedCallback;
  target->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  target->velocityY = toFIXED(10);
  target->bounce = FIXED_ONE / 5;
  target->layer = MAPGRID_LAYER_GROUND;
  caster->update = 0;
  Actor_SetSpriteColorModeFar(caster, SPRITE_COLOR_NORMAL);
  if (effectState->sceneVariant != 0) {
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
/// clap, and sparkles scatter.
DEFINE_CLAPPING_HANDS_CAST(FieldPsynergy_Carry, FieldEffect_SpawnCarryHand)

/// Spawns a hand (SPRITE_MOVE_HAND) with the item break sound and grows it,
/// spinning 1/8 turn a frame, from a quarter to full size over 24 frames; angle
/// is the way it faces once grown.
struct Actor *FieldEffect_SpawnCarryHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *hand;
  FIXED scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  hand = Actor_Spawn(SPRITE_MOVE_HAND, x, y, z);
  if (hand != NULL) {
    hand->scaleY = FIXED_ONE / 4;
    hand->scaleX = FIXED_ONE / 4;
    hand->speedLimit = toFIXED(3);
    hand->acceleration = toFIXED(3);
    hand->steeringFlags = 0;
    Actor_SelectAnimationFar(hand, MOVE_HAND_ANIMATION_OPEN);
    scale = hand->scaleX;
    if (scale < FIXED_ONE) {
      do {
        scale += FIXED_ONE / 32;
        hand->scaleY = scale;
        hand->scaleX = scale;
        hand->facing += DEGtoANG(45);
        wait(1);
        scale = hand->scaleX;
      } while (scale < FIXED_ONE);
    }
    hand->facing = angle;
  }
  return hand;
}

#undef CARRY_SPARKLE_COUNT
