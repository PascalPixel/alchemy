/// @file
/// Move: a hand pushes the target, steered with the d-pad.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Returns the direction of the d-pad (TRIG_TURN a full turn): right
/// FACING_EAST, down FACING_SOUTH, left FACING_WEST, up FACING_NORTH;
/// DPAD_ANGLE_NONE when none or a diagonal is held.
s32 Input_GetDpadAngle(u32 unused) {
  // Indexed by the right, left, up and down bits.
  return gInput_DpadAngles[DPAD_INDEX(gKeysHeld)];
}

/// Per-frame update of Move's hand (FieldEffect_SpawnMoveHand): turns it
/// toward the actor it watches, at most 1/16 turn a frame, and sheds a
/// particle (SPRITE_SPARKLE) round it.
void FieldEffect_UpdateMoveHand(struct Actor *hand) {
  struct Actor *target;
  struct Vector3 position;
  struct Actor *sparkle;
  s32 turn;
  s32 radius;

  target = (struct Actor *)hand->watchedActor;
  if (target != 0) {
    FIXED dx = target->x.fixed - hand->x.fixed;
    FIXED dz = target->z.fixed - hand->z.fixed;

    if (dx != 0 || dz != 0) {
      turn = (s16)(Trig_Atan(dz, dx) - hand->facing);
      if (turn > FACING_STEP)
        turn = FACING_STEP;
      if (turn < -FACING_STEP)
        turn = -FACING_STEP;
      hand->facing += turn;
    }
    // ACTOR_FACES_MOTION off: its motion must not undo the turn.
    hand->steeringFlags = 0;
  }

  // The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
  // random direction.
  position.x = hand->x.fixed;
  position.y = hand->y.fixed - (Random16() << 4) - toFIXED(8);
  position.z = hand->z.fixed;
  radius = Random16() * 3;
  radius <<= 4;
  Vector_AddPolarOffset(radius, Random16(), &position);

  sparkle = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(sparkle);
}

/// When Move is cast on a target, the pushing hand appears beside it and pushes
/// it 16 pixels the way the d-pad points, for as long as the player pushes; A,
/// B, L or R end it.
void FieldPsynergy_SteerPushedTarget(s32 targetId) {
  struct Vector3 position;
  struct Vector3 beyond;
  struct FieldEffectState *effectState;
  struct Actor *caster;
  struct Actor *target;
  struct Actor *pusher;
  struct Actor *hit;
  FIXED x;
  FIXED z;
  s32 angle;
  s32 moved;
  s32 direction;
  s32 keys;
  s32 cellX;
  s32 cellZ;

  effectState = gFieldEffectState;
  caster = effectState->caster;
  target = effectState->target;
  // effectState->angle is the way the caster faces; the hand's side of the
  // target is the opposite way, toward the caster.
  angle = effectState->angle + DEGtoANG(180);
  moved = 0;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  caster->watchedActor = (union ActorOrEffect *)target;
  Actor_StartScriptFar(caster, gFieldEffect_SourceHoldScript);
  pusher = FieldEffect_SpawnMoveHand(caster);
  if (pusher == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }

  // The hand goes 16 pixels above the target and 16 pixels toward angle.
  pusher->watchedActor = (union ActorOrEffect *)target;
  SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  Vector_AddPolarOffset(toFIXED(16), angle, &position);
  Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
  FieldEffect_GrowHandAndWait(pusher);
  // The hand floats at up to 4 pixels a frame.
  pusher->speedLimit = toFIXED(4);
  pusher->acceleration = toFIXED(1) / 2;
  pusher->motionFlags = ACTOR_FLOATS;
  target->update = Actor_CycleSpriteColors;
  target->speedLimit = toFIXED(2) / 5;
  target->acceleration = toFIXED(1) / 5;
  target->steeringFlags = moved;
  target->layer = MAPGRID_LAYER_OVERHEAD;
  for (;;) {
    wait(1);
    keys = gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS);
    if (keys != 0)
      break;

    // With the d-pad released, the hand goes back to its place.
    direction = (u16)Input_GetDpadAngle(gKeysHeld);
    if (direction == DPAD_ANGLE_NONE) {
      position.x = target->x.fixed;
      position.y = target->y.fixed + toFIXED(16);
      position.z = target->z.fixed;
      Vector_AddPolarOffset(toFIXED(16), angle, &position);
      Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_OPEN);
      SET_VELOCITY(pusher, 0, 0, 0);
      continue;
    }

    // The hand leans 2 pixels the way it pushes; check the cells 16 and 32
    // pixels beyond the target.
    position.x = target->x.fixed;
    position.y = target->y.fixed + toFIXED(16);
    position.z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), angle, &position);
    Vector_AddPolarOffset(toFIXED(2), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Actor_WaitForMoveFar(pusher);
    position.x = target->x.fixed;
    position.y = target->y.fixed;
    position.z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), direction, &position);
    beyond.x = target->x.fixed;
    beyond.y = target->y.fixed;
    beyond.z = target->z.fixed;
    Vector_AddPolarOffset(toFIXED(32), direction, &beyond);
    // A wall or higher ground stops the push; level ground or a drop does
    // not.
    if (MapGrid_CheckCollisionFar(target, (struct FieldPosition *)&position) >
        COLLISION_NONE) {
      Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_FLEX);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    hit = ActorScript_FindOverlappingActorFar(target, (s32 *)&position);
    if (hit != NULL) {
      if (hit != caster) {
        Actor_SelectAnimationFar(pusher, MOVE_HAND_ANIMATION_FLEX);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }

      // The push runs into the caster: if the caster stands on the cell the
      // target would move to, it cannot be pushed; if the caster stands on the
      // cell 32 pixels on, the caster moves along when the cell beyond is free.
      cellX = caster->x.fixed & FIELD_CELL_MASK;
      cellZ = caster->z.fixed & FIELD_CELL_MASK;
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
        if (MapGrid_GetTerrainKindAtFar(caster->layer, beyond.x, beyond.z) !=
            TERRAIN_FLAT) {
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
        pusher,
        gFieldEffect_QuadrantModes[(u16)(angle - direction) / (TRIG_TURN / 4)]);
    wait(15);
    target->paused = 0;
    target->speedLimit = toFIXED(1) / 5;
    target->acceleration = toFIXED(1) / 5;
    Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = toFIXED(1) / 5;
    pusher->acceleration = toFIXED(1) / 5;
    Vector_AddPolarOffset(toFIXED(16), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y + toFIXED(16),
                           position.z);
    if (moved == 1) {
      Actor_GetById(effectState->casterId)->steeringFlags &=
          ~ACTOR_FACES_MOTION;
      caster->speedLimit = toFIXED(1) / 5;
      caster->acceleration = toFIXED(1) / 5;
      Actor_SetMoveTargetFar(caster, beyond.x, beyond.y, beyond.z);
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
  Actor_SetSpriteColorModeFar(target, effectState->savedPalette);
  Actor_StartScriptFar(target, effectState->savedScript);
  target->update = effectState->savedCallback;
  FieldPsynergy_StopCasterUpdate();
  if (moved == 1)
    Actor_GetById(effectState->casterId)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldPsynergy_FadeFromCastColor();
  FieldEffect_BurstHand(pusher);
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
    Vector_AddPolarOffset(toFIXED(30), (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = toFIXED(4);
    effect->speedMax = toFIXED(4);
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
/// frames and then bursts into sparkles.
void FieldPsynergy_Move(void) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 work[3];
  struct FieldEffectState *effectState;
  struct Actor *caster;
  struct Actor *hand;

  effectState = gFieldEffectState;
  caster = effectState->caster;

  do {
    FieldPsynergy_FadeToCastColor();
    hand = FieldEffect_SpawnMoveHand(caster);
    FieldEffect_GrowHandAndWait(hand);
    // With no hand there is nothing to flex.
    if (hand == 0)
      break;
    Actor_SelectAnimationFar(hand, MOVE_HAND_ANIMATION_FLEX);
    wait(30);
  } while (0);
  FieldPsynergy_FadeFromCastColor();
  FieldEffect_BurstHand(hand);
}

/// Spawns Move's hand (SPRITE_MOVE_HAND) 16 pixels above the caster, at quarter
/// size, and moves it 16 pixels the way the caster faces (to the nearest
/// quarter turn), with 8 sparkles scattering from it.
struct Actor *FieldEffect_SpawnMoveHand(struct Actor *caster) {
  struct Actor *hand;
  struct Actor *sparkle;
  s32 angle;
  FIXED radius;
  FIXED speed;
  s32 sparkleIndex;
  u32 spread;
  u32 jitter;

  angle = FACING_NEAREST_CARDINAL(caster->facing);
  hand = Actor_Spawn(SPRITE_MOVE_HAND, caster->x.fixed,
                     caster->y.fixed + toFIXED(16), caster->z.fixed);
  if (hand == 0)
    return 0;
  hand->scaleY = FIXED_ONE / 4;
  hand->scaleX = FIXED_ONE / 4;
  hand->update = FieldEffect_UpdateMoveHand;
  hand->speedLimit = toFIXED(2);
  hand->acceleration = toFIXED(2);
  hand->motionFlags = 0;
  Actor_SelectAnimationFar(hand, MOVE_HAND_ANIMATION_GRIP);
  Actor_SetTargetPositionFromMagnitudeAngle(hand, toFIXED(16), angle);

  // Sparkles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
  // pixels a frame.
  for (sparkleIndex = 0; sparkleIndex < 8; sparkleIndex++) {
    sparkle = Actor_Spawn(SPRITE_SPARKLE, caster->x.fixed,
                          caster->y.fixed + toFIXED(16), caster->z.fixed);
    if (sparkle != 0) {
      Actor_StartScriptFar(sparkle, gFieldEffect_FragmentScript);
      speed = Random16() + FIXED_ONE;
      sparkle->acceleration = toFIXED(1);
      sparkle->speedLimit = speed;
      sparkle->motionFlags = ACTOR_FALLS;
      sparkle->gravity = toFIXED(1) / 50;
      spread = Random16();
      sparkle->velocityY = spread - Random16();
      radius = Random16() * 24 + toFIXED(8);
      jitter = Random16();
      Actor_SetTargetPositionFromMagnitudeAngle(
          sparkle, radius, ((jitter - Random16()) >> 3) + caster->facing);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return hand;
}

/// Sets an actor's scale at once to full size (its first step of 1/16 past
/// FIXED_ONE - 1: exactly FIXED_ONE from a multiple of 1/16) when it is
/// smaller, then waits, at most 600 frames, for it to arrive at its target.
void FieldEffect_GrowHandAndWait(struct Actor *actor) {
  FIXED next;
  FIXED scale;

  if (actor != NULL) {
    scale = actor->scaleX;
    if (scale < FIXED_ONE) {
      do {
        next = scale + FIXED_ONE / 16;
        scale = next;
      } while (next < FIXED_ONE);
      actor->scaleX = next;
      actor->scaleY = next;
    }
    Actor_WaitForMoveFar(actor);
  }
}

/// Ends a hand: over 31 frames it rises 1 pixel, turns 1/8 turn and shrinks by
/// 1/32 of full size a frame, then bursts into 8 sparkles and is released.
void FieldEffect_BurstHand(struct Actor *hand) {
  struct Actor *sparkle;
  s32 i;
  FIXED dist;
  u32 spread;

  Sound_PlayFar(SE_CAST);
  for (i = 0; i < 31; i++) {
    hand->y.fixed += toFIXED(1);
    hand->facing += DEGtoANG(45);
    hand->scaleX += -FIXED_ONE / 32;
    hand->scaleY += -FIXED_ONE / 32;
    wait(1);
  }
  for (i = 0; i < 8; i++) {
    sparkle = Actor_Spawn(SPRITE_SPARKLE, hand->x.fixed, hand->y.fixed,
                          hand->z.fixed);
    if (sparkle != 0) {
      Actor_StartScriptFar(sparkle, gFieldEffect_FragmentScript);
      sparkle->speedLimit = Random16() + FIXED_ONE;
      sparkle->acceleration = FIXED_ONE;
      sparkle->motionFlags = ACTOR_FALLS;
      sparkle->gravity = toFIXED(1) / 25;
      spread = Random16();
      sparkle->velocityY = spread - Random16();
      dist = Random16() * 24 + toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(sparkle, dist, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(hand);
}
