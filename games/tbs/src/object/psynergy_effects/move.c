// Part of psynergy_effects.c, compiled only through it.
// Move: a hand pushes the target, steered with the d-pad.

#include "../psynergy_effects_internal.h"

// Returns the direction of the d-pad (0x10000 a full turn): right 0, down
// 0x4000, left 0x8000, up 0xc000; 0xffff when none or a diagonal is held.
s32 Input_GetDpadAngle(u32 unused) {
  // Indexed by the right, left, up and down bits.
  return gBattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15];
}

// Per-frame update of a broken item's main fragment: turns it toward the
// object it is linked to, at most 1/16 turn a frame, and sheds a particle
// (object 0x11d) round it.
void BattleFx_UpdateItemBreakFragment(union FieldObject *fieldObject) {
  struct FieldActor *source = (struct FieldActor *)fieldObject;
  struct FieldActor *target;
  struct EffectVector position;
  struct ItemBreakFragmentObject *fragment;
  s32 turn;
  s32 radius;

  target = source->linkedObject;
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

  fragment = Object_Spawn(0x11d, position.x, position.y, position.z);
  BattleFx_InitializeFallingFragment(fragment);
}

#define MOVE_CELL_MASK 0xfff00000 // a 16-pixel tile

// Abilities: Move (field kind 1), cast on a target.
//
// Puts the pushing hand beside the target and pushes the target 16 pixels the
// way the d-pad points, for as long as the player pushes; A, B, L or R end it.
// Where the target cannot go, an error sound plays every 16 frames. When the
// push would run into the caster, the caster blocks it or, when the tile beyond
// is free, is pushed along too.
void BattleFx_SteerPushedTarget(s32 targetId) {
  struct EffectVector position;
  struct EffectVector beyond;
  struct BattleFxScene *work;
  struct FieldActor *user;
  struct FieldActor *target;
  struct FieldActor *pusher;
  struct FieldActor *hit;
  s32 x;
  s32 z;
  s32 angle;
  s32 moved;
  s32 direction;
  s32 keys;
  s32 cellX;
  s32 cellZ;
  struct EffectVector *candidate;

  work = gEffectWork;
  user = work->mainObject;
  target = work->child;
  angle = work->angle + 0x8000; // opposite work->angle
  moved = 0;
  if (target == NULL)
    return;
  BattleEffect_InitializeSharedScene();
  user->linkedObject = target;
  ObjectDispatch_InitializeFar((struct DispatchObject *)user,
                               gBattleFx_SourceHoldScript);
  pusher = BattleFx_StartItemBreak(user);
  if (pusher == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }

  // The hand goes 16 pixels above the target and 16 pixels toward angle.
  pusher->linkedObject = target;
  candidate = &position;
  SET_POSITION(candidate, target->x.fixed, target->y.fixed + (16 << 16),
               target->z.fixed);
  Vector_AddPolarOffset(16 << 16, angle, candidate);
  Object_SetMoveTargetFar(pusher, candidate->x, candidate->y, candidate->z);
  BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
  pusher->speed = 4 << 16;
  pusher->acceleration = 0x8000;
  pusher->motionFlags = 4;
  target->update = ObjectGroup_ApplyRandomChildValues; // cycle its colours
  target->speed = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = moved;
  target->terrainId = 2;
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
      Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      ObjectDispatch_SelectAnimationFar(pusher, 1);
      SET_VELOCITY(pusher, 0, 0, 0);
      continue;
    }

    // The hand leans 2 pixels the way it pushes; check the tiles 16 and 32
    // pixels beyond the target.
    position.x = target->x.fixed;
    position.y = target->y.fixed + (16 << 16);
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &position);
    Vector_AddPolarOffset(2 << 16, direction, &position);
    Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    FieldObject_WaitForMoveFar(pusher);
    position.x = target->x.fixed;
    position.y = target->y.fixed;
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, direction, &position);
    beyond.x = target->x.fixed;
    beyond.y = target->y.fixed;
    beyond.z = target->z.fixed;
    Vector_AddPolarOffset(32 << 16, direction, &beyond);
    if (Map_CheckObjectStepFar(target, &position) > 0) {
      ObjectDispatch_SelectAnimationFar(pusher, 4);
      if ((gFrameCount & 15) == 0)
        AudioCommand_PlayFar(SE_MENU_ERROR);
      continue;
    }
    hit = ScriptObject_FindOverlappingEntryFar(target, candidate);
    if (hit != NULL) {
      if (hit != user) {
        ObjectDispatch_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          AudioCommand_PlayFar(SE_MENU_ERROR);
        continue;
      }

      // The push runs into the caster: if the caster stands on the tile the
      // target would move to, it cannot be pushed; if the caster stands on the
      // tile 32 pixels on, the caster moves along when the tile beyond is free.
      cellX = user->x.fixed & MOVE_CELL_MASK;
      cellZ = user->z.fixed & MOVE_CELL_MASK;
      if (cellX == (candidate->x & MOVE_CELL_MASK) &&
          cellZ == (candidate->z & MOVE_CELL_MASK)) {
        ObjectDispatch_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          AudioCommand_PlayFar(SE_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & MOVE_CELL_MASK) &&
          cellZ == (beyond.z & MOVE_CELL_MASK)) {
        if (Map_GetCellAttributeLowNibbleFar(user->terrainId, beyond.x,
                                             beyond.z) != 0) {
          ObjectDispatch_SelectAnimationFar(pusher, 4);
          if ((gFrameCount & 15) == 0)
            AudioCommand_PlayFar(SE_MENU_ERROR);
          continue;
        }
        moved = 1;
      }
    }

    // Push: the hand's animation comes from the turn between angle and the
    // push (in quarter turns).
    AudioCommand_PlayFar(SE_175);
    x = position.x;
    z = position.z;
    ObjectDispatch_SelectAnimationFar(
        pusher, gBattleFx_QuadrantModes[(u16)(angle - direction) >> 14]);
    WaitFrames(15);
    target->paused = 0;
    target->speed = 0x3333;
    target->acceleration = 0x3333;
    Object_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speed = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset(16 << 16, direction, &position);
    Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16),
                            position.z);
    if (moved == 1) {
      Object_GetById(work->firstObjectId)->steeringFlags &= 0xfe;
      user->speed = 0x3333;
      user->acceleration = 0x3333;
      Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
    }
    FieldObject_WaitForMoveFar(target);
    target->x.fixed = x;
    target->z.fixed = z;
    target->velocityX = 0;
    target->velocityZ = 0;
    break;
  }

  // Restore the target's colours, script and update, and remove the hand.
  Animation_ApplyChildValuesFar(target, work->savedPalette);
  ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                               work->savedScript);
  target->update = work->savedCallback;
  EffectRuntime_StopCurrentObject();
  if (moved == 1)
    (Object_GetById(work->firstObjectId))->steeringFlags |= 1;
  BattleFx_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(pusher);
}

// Effect slot callback of a particle that flies 30 pixels out from where it
// started, comes back and is freed.
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot) {
  // Forward state-1 fallthrough keeps the callback extent but changes native
  // instructions; retain the immediate retry until a plain form matches.
  struct EffectVector position;
  s8 *statePtr = &slot->state;
  s32 state;

next_state:
  state = *statePtr;
  if (state == 0) {
    u32 angle;

    position.x = slot->originX;
    position.z = slot->originZ;
    angle = Random16();
    Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
    slot->targetX = position.x;
    slot->targetZ = position.z;
    slot->acceleration = 4 << 16;
    slot->maxSpeed = 4 << 16;
    slot->limitTurnStep = state;
    (*statePtr)++;
    return;
  }

  if (state == 1) {
    if (EffectSlot_HasPendingTarget(slot) == 0) {
      (*statePtr)++;
      goto next_state;
    }
    return;
  }

  if (state == 2) {
    slot->targetX = slot->originX;
    slot->targetZ = slot->originZ;
    slot->maxTurnStep = 0x400;
    slot->limitTurnStep = 1;
    (*statePtr)++;
    return;
  }

  if (state == 3 && EffectSlot_HasPendingTarget(slot) == 0)
    BattleFx_ClearOwnedSlot(slot);
}

// Abilities: Move (field kind 1), cast with no target.
//
// Shows the broken item beside the caster for 30 frames, then turns it into
// particles.
void FieldFx_RunMove(void) {
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = gEffectWork;
  object = scene->mainObject;

  /* FAKEMATCH(one-pass): the call in a do { } while (0), which keeps the
   * mainObject load below the frame's sub sp; as a plain call, 2 of 24
   * instructions differ in tbs-en. */
  do {
    BattleEffect_InitializeSharedScene();
  } while (0);
  object = BattleFx_StartItemBreak(object);
  BattleFx_SnapScaleToFull(object);
  if (object != 0) {
    ObjectDispatch_SelectAnimationFar(object, 4);
    WaitFrames(30);
  }
  BattleFx_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(object);
}

// Spawns the broken item (object 215) 16 pixels above object and moves it 16
// pixels the way object faces (to the nearest quarter turn), with 8
// particles round it. Returns the item, or 0.
void *BattleFx_StartItemBreak(void *object) {
  struct MotionObject *source = object;
  struct MotionObject *parent;
  struct MotionObject *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->angle + 0x2000) & 0xc000; // to the nearest quarter turn
  parent = Object_Spawn(215, source->x, source->y + (16 << 16), source->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000; // from 0.25
  parent->scaleX = 0x4000;
  parent->update = BattleFx_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  ObjectDispatch_SelectAnimationFar(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  // Particles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
  // pixels a frame.
  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Object_Spawn(285, source->x, source->y + (16 << 16), source->z);
    if (child != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                   gBattleFx_FragmentScript);
      speed = Random16() + 0x10000;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->verticalMotionStrength = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Motion_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->angle);
    }
  }
  AudioCommand_PlayFar(SE_ITEM_BREAK);
  return parent;
}

// Per-frame update: grows an object's scale by 1/16 a frame up to full size
// and fixes its position.
void BattleFx_SnapScaleToFull(struct MotionObject *obj) {
  s32 next;
  s32 scale;

  if (obj != NULL) {
    scale = obj->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + 0x1000;
        scale = next;
      } while (next <= 0xffff);
      obj->scaleX = next;
      obj->scaleY = next;
    }
    FieldObject_WaitForMoveFar(obj);
  }
}

// Per-frame update: the object spins as it rises for 31 frames, shrinks, and
// bursts into 8 particles.
void UpdateRisingParticleBurst(void *object) {
  struct MotionObject *src = object;
  struct MotionObject *child;
  s32 i;
  s32 dist;
  u32 spread;

  AudioCommand_PlayFar(SE_CAST);
  for (i = 30; i >= 0; i--) {
    src->y += 1 << 16;
    src->angle += 0x2000; // 1/8 turn
    src->scaleX += -0x800;
    src->scaleY += -0x800;
    WaitFrames(1);
  }
  for (i = 7; i >= 0; i--) {
    child = Object_Spawn(285, src->x, src->y, src->z);
    if (child != 0) {
      s32 speed;
      /* FAKEMATCH(temporary): 1.0 in a block local set before the block's
       * calls, which loop.c hoists for both uses; as the constant at both
       * uses, 24 of 93 instructions differ in tbs-en. */
      s32 unit = 0x10000;

      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                   gBattleFx_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->verticalMotionStrength = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  AudioCommand_PlayFar(SE_131);
  ObjectDispatch_ReleaseFar(src);
}

#undef MOVE_CELL_MASK
