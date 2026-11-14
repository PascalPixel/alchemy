/// @file
/// Move: a hand pushes the target, steered with the d-pad.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Returns the direction of the d-pad (TRIG_TURN a full turn): right
/// FACING_EAST, down FACING_SOUTH, left FACING_WEST, up FACING_NORTH;
/// DPAD_ANGLE_NONE when none or a diagonal is held.
s32 Input_GetDpadAngle(u32 unused) {
  // Indexed by the right, left, up and down bits.
  return gFieldEffect_CyclePatternWords[(gKeysHeld >> 4) & 15];
}

/// Per-frame update of Move's hand (FieldEffect_SpawnMoveHand): turns it
/// toward the actor it watches, at most 1/16 turn a frame, and sheds a
/// particle (SPRITE_SPARKLE) round it.
void FieldEffect_UpdateMoveHand(struct Actor *source) {
  struct Actor *target;
  struct Vector3 position;
  struct Actor *fragment;
  s32 turn;
  s32 radius;

  target = source->watchedActor;
  if (target != 0) {
    s32 dx = target->x.fixed - source->x.fixed;
    s32 dz = target->z.fixed - source->z.fixed;

    if (dx != 0 || dz != 0) {
      turn = (s16)(ArcTan2(dz, dx) - source->facing);
      if (turn > FACING_STEP)
        turn = FACING_STEP;
      if (turn < -FACING_STEP)
        turn = -FACING_STEP;
      source->facing += turn;
    }
    // ACTOR_FACES_MOTION off: its motion must not undo the turn.
    source->steeringFlags = 0;
  }

  // The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
  // random direction.
  position.x = source->x.fixed;
  position.y = source->y.fixed - (Random16() << 4) - px(8);
  position.z = source->z.fixed;
  radius = Random16() * 3;
  radius <<= 4;
  Vector_AddPolarOffset(radius, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

/// When Move is cast on a target, the pushing hand appears beside it and pushes
/// it 16 pixels the way the d-pad points, for as long as the player pushes; A,
/// B, L or R end it.
void FieldPsynergy_SteerPushedTarget(s32 targetId) {
  struct Vector3 position;
  struct Vector3 beyond;
  struct FieldEffectWork *effectWork;
  struct Actor *user;
  struct Actor *target;
  struct Actor *pusher;
  struct Actor *hit;
  s32 x;
  s32 z;
  s32 angle;
  s32 moved;
  s32 direction;
  s32 keys;
  s32 cellX;
  s32 cellZ;

  effectWork = gFieldEffectWork;
  user = effectWork->mainActor;
  target = effectWork->child;
  // work->angle is the way the caster faces; the hand's side of the target
  // is the opposite way, toward the caster.
  angle = effectWork->angle + TRIG_TURN / 2;
  moved = 0;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  user->watchedActor = target;
  Actor_StartScriptFar(user, gFieldEffect_SourceHoldScript);
  pusher = FieldEffect_SpawnMoveHand(user);
  if (pusher == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }

  // The hand goes 16 pixels above the target and 16 pixels toward angle.
  pusher->watchedActor = target;
  SET_POSITION(&position, target->x.fixed, target->y.fixed + px(16),
               target->z.fixed);
  Vector_AddPolarOffset(px(16), angle, &position);
  Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
  FieldEffect_SnapScaleToFull(pusher);
  // The hand floats (motion flag bit 2) at up to 4 pixels a frame.
  pusher->speedLimit = px(4);
  pusher->acceleration = px(1) / 2;
  pusher->motionFlags = ACTOR_FLOATS;
  target->update = Actor_CycleSpriteColors;
  target->speedLimit = px(2) / 5;
  target->acceleration = px(1) / 5;
  target->steeringFlags = moved;
  target->layer = 2;
  for (;;) {
    wait(1);
    keys = gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS);
    if (keys != 0)
      break;

    // With the d-pad released, the hand goes back to its place.
    direction = (u16)Input_GetDpadAngle(gKeysHeld);
    if (direction == DPAD_ANGLE_NONE) {
      position.x = target->x.fixed;
      position.y = target->y.fixed + px(16);
      position.z = target->z.fixed;
      Vector_AddPolarOffset(px(16), angle, &position);
      Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_OPEN);
      SET_VELOCITY(pusher, 0, 0, 0);
      continue;
    }

    // The hand leans 2 pixels the way it pushes; check the cells 16 and 32
    // pixels beyond the target.
    position.x = target->x.fixed;
    position.y = target->y.fixed + px(16);
    position.z = target->z.fixed;
    Vector_AddPolarOffset(px(16), angle, &position);
    Vector_AddPolarOffset(px(2), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Actor_WaitForMoveFar(pusher);
    position.x = target->x.fixed;
    position.y = target->y.fixed;
    position.z = target->z.fixed;
    Vector_AddPolarOffset(px(16), direction, &position);
    beyond.x = target->x.fixed;
    beyond.y = target->y.fixed;
    beyond.z = target->z.fixed;
    Vector_AddPolarOffset(px(32), direction, &beyond);
    // A wall (2) or ground more than 8 pixels higher (1) stops the push;
    // level ground or a drop does not.
    if (MapGrid_CheckCollisionFar(target, (struct FieldPosition *)&position) >
        0) {
      Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_FLEX);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    hit = ActorScript_FindOverlappingEntryFar(target, (s32 *)&position);
    if (hit != NULL) {
      if (hit != user) {
        Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_FLEX);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }

      // The push runs into the caster: if the caster stands on the cell the
      // target would move to, it cannot be pushed; if the caster stands on the
      // cell 32 pixels on, the caster moves along when the cell beyond is free.
      cellX = user->x.fixed & FIELD_CELL_MASK;
      cellZ = user->z.fixed & FIELD_CELL_MASK;
      if (cellX == (position.x & FIELD_CELL_MASK) &&
          cellZ == (position.z & FIELD_CELL_MASK)) {
        Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_FLEX);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & FIELD_CELL_MASK) &&
          cellZ == (beyond.z & FIELD_CELL_MASK)) {
        // The caster can be pushed only onto flat ground.
        if (MapGrid_GetCellAttributeLowNibbleFar(user->layer, beyond.x,
                                                 beyond.z) != TERRAIN_FLAT) {
          Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_FLEX);
          if ((gFrameCount & 15) == 0)
            Sound_PlayFar(SE_MENU_ERROR);
          continue;
        }
        moved = 1;
      }
    }

    // Push: the hand's animation comes from the turn between angle and the push
    // (in quarter turns): 2 pushing away from its side, 3 drawing toward it, 1
    // sideways.
    Sound_PlayFar(SE_NOTE_HIGH);
    x = position.x;
    z = position.z;
    Actor_SelectAnimationFar(
        pusher, gFieldEffect_QuadrantModes[(u16)(angle - direction) >> 14]);
    wait(15);
    target->paused = 0;
    target->speedLimit = px(1) / 5;
    target->acceleration = px(1) / 5;
    Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = px(1) / 5;
    pusher->acceleration = px(1) / 5;
    Vector_AddPolarOffset(px(16), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y + px(16), position.z);
    if (moved == 1) {
      Actor_GetById(effectWork->firstActorId)->steeringFlags &=
          ~ACTOR_FACES_MOTION;
      user->speedLimit = px(1) / 5;
      user->acceleration = px(1) / 5;
      Actor_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
    }
    // Land exactly on the cell, whatever the braking left.
    Actor_WaitForMoveFar(target);
    target->x.fixed = x;
    target->z.fixed = z;
    target->velocityX = 0;
    target->velocityZ = 0;
    break;
  }

  // Restore the target's colours, script and update, and remove the hand.
  Actor_SetSpriteColorModeFar(target, effectWork->savedPalette);
  Actor_StartScriptFar(target, effectWork->savedScript);
  target->update = effectWork->savedCallback;
  FieldPsynergy_StopCasterUpdate();
  if (moved == 1)
    (Actor_GetById(effectWork->firstActorId))->steeringFlags |= 1;
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(pusher);
}

/// Field effect sprite callback of a particle that flies 30 pixels out from
/// where it started, comes back and is freed.
void FieldEffect_UpdateOutAndBack(struct FieldParticle *effect) {
  struct Vector3 position;
  s32 state;

next_state:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    u32 angle;

    position.x = effect->originX;
    position.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(px(30), (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = px(4);
    effect->speedMax = px(4);
    effect->turnStepLimited = state;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_OUT) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto next_state;
    }
    return;
  }

  if (state == OUT_AND_BACK_AIM_BACK) {
    effect->targetX = effect->originX;
    effect->targetZ = effect->originZ;
    effect->turnStepMax = TRIG_TURN / 64;
    effect->turnStepLimited = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// When Move is cast with no target, the hand appears beside the caster for 30
/// frames and then turns into particles.
void FieldPsynergy_Move(void) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 work[3];
  struct FieldEffectWork *effectWork;
  struct Actor *caster;
  struct Actor *hand;

  effectWork = gFieldEffectWork;
  caster = effectWork->mainActor;

  /* FAKEMATCH(dead): an empty once-only loop before the call. */
  do {
  } while (0);
  FieldPsynergy_FadeToCastColor();
  hand = FieldEffect_SpawnMoveHand(caster);
  FieldEffect_SnapScaleToFull(hand);
  if (hand != 0) {
    Actor_SelectAnimationFar(hand, MOVE_HAND_ANIMATION_FLEX);
    wait(30);
  }
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(hand);
}

/// Spawns Move's hand (SPRITE_MOVE_HAND) 16 pixels above the caster, at quarter
/// size, and moves it 16 pixels the way the caster faces (to the nearest
/// quarter turn), with 8 particles round it.
struct Actor *FieldEffect_SpawnMoveHand(struct Actor *caster) {
  struct Actor *parent;
  struct Actor *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle =
      (caster->facing + TRIG_TURN / 8) & 0xc000; // to the nearest quarter turn
  parent = Actor_Spawn(SPRITE_MOVE_HAND, caster->x.fixed,
                       caster->y.fixed + px(16), caster->z.fixed);
  if (parent == 0)
    return 0;
  parent->scaleY = Q16_ONE / 4;
  parent->scaleX = Q16_ONE / 4;
  parent->update = FieldEffect_UpdateMoveHand;
  parent->speedLimit = px(2);
  parent->acceleration = px(2);
  parent->motionFlags = 0;
  Actor_SelectAnimationFar(parent, MOVE_HAND_ANIMATION_GRIP);
  Actor_SetTargetPositionFromMagnitudeAngle(parent, px(16), angle);

  // Particles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
  // pixels a frame.
  for (fragmentIndex = 0; fragmentIndex < 8; fragmentIndex++) {
    child = Actor_Spawn(SPRITE_SPARKLE, caster->x.fixed,
                        caster->y.fixed + px(16), caster->z.fixed);
    if (child != 0) {
      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16() + Q16_ONE;
      child->acceleration = px(1);
      child->speedLimit = speed;
      child->motionFlags = ACTOR_FALLS;
      child->gravity = px(1) / 50;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + px(8);
      jitter = Random16();
      Actor_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + caster->facing);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return parent;
}

/// Sets an actor's scale at once to full size (its first step of 1/16 past
/// Q16_ONE - 1: exactly Q16_ONE from a multiple of 1/16) when it is smaller,
/// then waits, at most 600 frames, for it to arrive at its target.
void FieldEffect_SnapScaleToFull(struct Actor *actor) {
  s32 next;
  s32 scale;

  if (actor != NULL) {
    scale = actor->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + Q16_ONE / 16;
        scale = next;
      } while (next <= 0xffff);
      actor->scaleX = next;
      actor->scaleY = next;
    }
    Actor_WaitForMoveFar(actor);
  }
}

/// Ends a hand: over 31 frames it rises 1 pixel, turns 1/8 turn and shrinks by
/// 1/32 of full size a frame, then bursts into 8 particles and is released.
void UpdateRisingParticleBurst(struct Actor *hand) {
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlayFar(SE_CAST);
  for (i = 0; i < 31; i++) {
    hand->y.fixed += px(1);
    hand->facing += TRIG_TURN / 8;
    hand->scaleX += -Q16_ONE / 32;
    hand->scaleY += -Q16_ONE / 32;
    wait(1);
  }
  for (i = 0; i < 8; i++) {
    child = Actor_Spawn(SPRITE_SPARKLE, hand->x.fixed, hand->y.fixed,
                        hand->z.fixed);
    if (child != 0) {
      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      child->speedLimit = Random16() + Q16_ONE;
      child->acceleration = Q16_ONE;
      child->motionFlags = ACTOR_FALLS;
      child->gravity = px(1) / 25;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24 + px(8);
      Actor_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(hand);
}
