/// @file
/// Move: a hand pushes the target, steered with the d-pad.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Returns the direction of the d-pad (0x10000 a full turn): right 0, down
/// 0x4000, left 0x8000, up 0xc000; 0xffff when none or a diagonal is held.
/// Opposite directions cancel, so a third held direction still counts.
s32 Input_GetDpadAngle(u32 unused) {
  // Indexed by the right, left, up and down bits.
  return gFieldEffect_CyclePatternWords[(gKeysHeld >> 4) & 15];
}

/// Per-frame update of Move's hand (FieldEffect_StartItemBreak): turns it
/// toward the actor it watches, at most 1/16 turn a frame, and sheds a
/// particle (SPRITE_SPARKLE) round it.
void FieldEffect_UpdateItemBreakFragment(struct Actor *actor) {
  struct Actor *source = (struct Actor *)actor;
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
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      source->facing += turn;
    }
    // ACTOR_FACES_MOTION off: its motion must not undo the turn.
    source->steeringFlags = 0;
  }

  // The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
  // random direction.
  position.x = source->x.fixed;
  position.y = source->y.fixed - (Random16() << 4) - 0x80000;
  position.z = source->z.fixed;
  radius = Random16() * 3;
  radius <<= 4;
  Vector_AddPolarOffset(radius, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

#define MOVE_CELL_MASK 0xfff00000 ///< a 16-pixel cell

/// When Move is cast on a target, the pushing hand appears beside it and pushes
/// it 16 pixels the way the d-pad points, for as long as the player pushes; A,
/// B, L or R end it.
///
/// Abilities: Move (FIELD_PSYNERGY_MOVE).
///
/// Where the target cannot go, an error sound plays every 16 frames. When the
/// push would run into the caster, the caster blocks it or, when the cell
/// beyond is free, is pushed along too.
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
  struct Vector3 *candidate;

  effectWork = gFieldEffectWork;
  user = effectWork->mainActor;
  target = effectWork->child;
  // work->angle is the way the caster faces; the hand's side of the target
  // is the opposite way, toward the caster.
  angle = effectWork->angle + 0x8000;
  moved = 0;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  user->watchedActor = target;
  Actor_StartScriptFar(user, gFieldEffect_SourceHoldScript);
  pusher = FieldEffect_StartItemBreak(user);
  if (pusher == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }

  // The hand goes 16 pixels above the target and 16 pixels toward angle.
  pusher->watchedActor = target;
  candidate = &position;
  SET_POSITION(candidate, target->x.fixed, target->y.fixed + (16 << 16),
               target->z.fixed);
  Vector_AddPolarOffset(16 << 16, angle, candidate);
  Actor_SetMoveTargetFar(pusher, candidate->x, candidate->y, candidate->z);
  FieldEffect_SnapScaleToFull(pusher);
  // The hand floats (motion flag bit 2) at up to 4 pixels a frame. The
  // target cycles its colours, keeps its facing while it moves, and reads
  // its ground from the overhead layer.
  pusher->speedLimit = 4 << 16;
  pusher->acceleration = 0x8000;
  pusher->motionFlags = ACTOR_FLOATS;
  target->update = Actor_CycleSpriteColors;
  target->speedLimit = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = moved;
  target->layer = 2;
  for (;;) {
    WaitFrames(1);
    keys = gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS);
    if (keys != 0)
      break;

    // With the d-pad released, the hand goes back to its place.
    direction = (u16)Input_GetDpadAngle(gKeysHeld);
    if (direction == 0xffff) {
      position.x = target->x.fixed;
      position.y = target->y.fixed + (16 << 16);
      position.z = target->z.fixed;
      Vector_AddPolarOffset(16 << 16, angle, &position);
      Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      Actor_SelectAnimationFar(pusher, 1);
      SET_VELOCITY(pusher, 0, 0, 0);
      continue;
    }

    // The hand leans 2 pixels the way it pushes; check the cells 16 and 32
    // pixels beyond the target.
    position.x = target->x.fixed;
    position.y = target->y.fixed + (16 << 16);
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &position);
    Vector_AddPolarOffset(2 << 16, direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Actor_WaitForMoveFar(pusher);
    position.x = target->x.fixed;
    position.y = target->y.fixed;
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, direction, &position);
    beyond.x = target->x.fixed;
    beyond.y = target->y.fixed;
    beyond.z = target->z.fixed;
    Vector_AddPolarOffset(32 << 16, direction, &beyond);
    // A wall (2) or ground more than 8 pixels higher (1) stops the push;
    // level ground or a drop does not.
    if (MapGrid_CheckCollisionFar(target, (struct FieldPosition *)&position) >
        0) {
      Actor_SelectAnimationFar(pusher, 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlaySongFar(SE_MENU_ERROR);
      continue;
    }
    hit = ActorScript_FindOverlappingEntryFar(target, (s32 *)candidate);
    if (hit != NULL) {
      if (hit != user) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlaySongFar(SE_MENU_ERROR);
        continue;
      }

      // The push runs into the caster: if the caster stands on the cell the
      // target would move to, it cannot be pushed; if the caster stands on the
      // cell 32 pixels on, the caster moves along when the cell beyond is free.
      cellX = user->x.fixed & MOVE_CELL_MASK;
      cellZ = user->z.fixed & MOVE_CELL_MASK;
      if (cellX == (candidate->x & MOVE_CELL_MASK) &&
          cellZ == (candidate->z & MOVE_CELL_MASK)) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlaySongFar(SE_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & MOVE_CELL_MASK) &&
          cellZ == (beyond.z & MOVE_CELL_MASK)) {
        // The caster can be pushed only onto flat ground.
        if (MapGrid_GetCellAttributeLowNibbleFar(user->layer, beyond.x,
                                                 beyond.z) != TERRAIN_FLAT) {
          Actor_SelectAnimationFar(pusher, 4);
          if ((gFrameCount & 15) == 0)
            Sound_PlaySongFar(SE_MENU_ERROR);
          continue;
        }
        moved = 1;
      }
    }

    // Push: the hand's animation comes from the turn between angle and the
    // push (in quarter turns): 2 pushing away from its side, 3 drawing toward
    // it, 1 sideways. Target and hand then move 16 pixels at 0.2 pixels a
    // frame, and a caster in the way is carried along with them.
    Sound_PlaySongFar(SE_NOTE_HIGH);
    x = position.x;
    z = position.z;
    Actor_SelectAnimationFar(
        pusher, gFieldEffect_QuadrantModes[(u16)(angle - direction) >> 14]);
    WaitFrames(15);
    target->paused = 0;
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset(16 << 16, direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16),
                           position.z);
    if (moved == 1) {
      Actor_GetById(effectWork->firstActorId)->steeringFlags &= 0xfe;
      user->speedLimit = 0x3333;
      user->acceleration = 0x3333;
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
/// where it started, comes back and is freed. It goes out straight at up to 4
/// pixels a frame and comes back turning at most 1/64 turn a frame, so it
/// curves home.
void FieldEffect_UpdateOrbitAndReturn(struct FieldParticle *effect) {
  // Forward OUT_AND_BACK_FLYING_OUT fallthrough keeps the callback extent but
  // changes native instructions; retain the immediate retry until a plain form
  // matches.
  struct Vector3 position;
  s8 *statePtr = &effect->state;
  s32 state;

next_state:
  state = *statePtr;
  if (state == OUT_AND_BACK_AIM_OUT) {
    u32 angle;

    position.x = effect->originX;
    position.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 4 << 16;
    effect->speedMax = 4 << 16;
    effect->turnStepLimited = state;
    (*statePtr)++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_OUT) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      (*statePtr)++;
      goto next_state;
    }
    return;
  }

  if (state == OUT_AND_BACK_AIM_BACK) {
    effect->targetX = effect->originX;
    effect->targetZ = effect->originZ;
    effect->turnStepMax = 0x400;
    effect->turnStepLimited = 1;
    (*statePtr)++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// When Move is cast with no target, the hand appears beside the caster for 30
/// frames and then turns into particles.
///
/// Abilities: Move (FIELD_PSYNERGY_MOVE).
void FieldPsynergy_Move(void) {
  UNUSED s32 work[3];
  struct FieldEffectWork *effectWork;
  void *object;

  effectWork = gFieldEffectWork;
  object = effectWork->mainActor;

  /* FAKEMATCH(one-pass): the call in a do { } while (0), which keeps the
   * mainActor load below the stack frame's sub sp; as a plain call, 2 of 24
   * instructions differ in tbs-en. */
  do {
    FieldPsynergy_FadeToCastColor();
  } while (0);
  object = FieldEffect_StartItemBreak(object);
  FieldEffect_SnapScaleToFull(object);
  if (object != 0) {
    Actor_SelectAnimationFar(object, 4);
    WaitFrames(30);
  }
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(object);
}

/// Spawns Move's hand (sprite 215) 16 pixels above object, at quarter size,
/// and moves it 16 pixels the way object faces (to the nearest quarter
/// turn), with 8 particles round it. Returns the hand, or 0 when it cannot be
/// spawned (Actor_Spawn); the caller releases it (UpdateRisingParticleBurst).
void *FieldEffect_StartItemBreak(void *object) {
  struct Actor *source = object;
  struct Actor *parent;
  struct Actor *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->facing + 0x2000) & 0xc000; // to the nearest quarter turn
  parent = Actor_Spawn(SPRITE_MOVE_HAND, source->x.fixed,
                       source->y.fixed + (16 << 16), source->z.fixed);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000;
  parent->scaleX = 0x4000;
  parent->update = FieldEffect_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  Actor_SelectAnimationFar(parent, 3);
  Actor_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  // Particles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
  // pixels a frame.
  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Actor_Spawn(SPRITE_SPARKLE, source->x.fixed,
                        source->y.fixed + (16 << 16), source->z.fixed);
    if (child != 0) {
      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16() + Q16_ONE;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->gravity = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Actor_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->facing);
    }
  }
  Sound_PlaySongFar(SE_ITEM_BREAK);
  return parent;
}

/// Sets an actor's scale at once to full size (its first step of 1/16 past
/// 0xffff: 0x10000 from a multiple of 1/16) when it is smaller, then waits,
/// at most 600 frames, for it to arrive at its target. Called by both Move
/// casts on the hand.
void FieldEffect_SnapScaleToFull(struct Actor *actor) {
  s32 next;
  s32 scale;

  if (actor != NULL) {
    scale = actor->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + 0x1000;
        scale = next;
      } while (next <= 0xffff);
      actor->scaleX = next;
      actor->scaleY = next;
    }
    Actor_WaitForMoveFar(actor);
  }
}

/// Ends a hand: over 31 frames it rises 1 pixel, turns 1/8 turn and shrinks
/// by 1/32 of full size a frame, then bursts into 8 particles and is
/// released. Not a per-frame update: it waits the frames itself. Called by
/// the Move casts and Lift.
void UpdateRisingParticleBurst(void *object) {
  struct Actor *src = object;
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlaySongFar(SE_CAST);
  for (i = 30; i >= 0; i--) {
    src->y.fixed += 1 << 16;
    src->facing += 0x2000; // 1/8 turn
    src->scaleX += -0x800;
    src->scaleY += -0x800;
    WaitFrames(1);
  }
  for (i = 7; i >= 0; i--) {
    child =
        Actor_Spawn(SPRITE_SPARKLE, src->x.fixed, src->y.fixed, src->z.fixed);
    if (child != 0) {
      s32 speed;
      /* FAKEMATCH(temporary): 1.0 in a block local set before the block's
       * calls, which loop.c hoists for both uses; as the constant at both
       * uses, 24 of 93 instructions differ in tbs-en. */
      s32 unit = Q16_ONE;

      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->gravity = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Actor_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlaySongFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(src);
}

#undef MOVE_CELL_MASK
