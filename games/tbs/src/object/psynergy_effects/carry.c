/// @file
/// Carry: two hands lift the target, move it with the d-pad and drop it.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

STATIC_ASSERT(sizeof(struct EffectVector) == 0x0c);

/// Scheduler callback while Carry holds its target: drops one particle a
/// frame round the lifted object, 8 to 24 pixels above it and 0 to 48 pixels
/// away in a random direction (FieldEffect_InitializeFallingFragment).
void FieldEffect_SpawnFallingParticles(void) {
  struct FieldActor *liftedObject;
  struct EffectVector position;
  struct ItemBreakFragmentObject *fragment;
  u32 radius;

  liftedObject = ((struct FieldFxScene *)gEffectWork)->child;
  position.x = liftedObject->x.fixed;
  position.y = liftedObject->y.fixed - Random16() * 16 + (24 << 16);
  position.z = liftedObject->z.fixed;

  radius = Random16() * 3;
  Vector_AddPolarOffset(radius * 16, Random16(), &position);

  fragment = (struct ItemBreakFragmentObject *)Object_Spawn(
      SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

#define CARRY_SHARD_MAX 20 ///< shards

/// Tests the target's move with its terrain reference raised 16 pixels, so a
/// carried target passes over ground up to 24 pixels above its own (a step
/// allows 8). Restores the reference before returning the collision result.
static __inline__ s32
FieldEffect_CheckRaisedTargetCollision(struct FieldActor *target,
                                       struct EffectVector *position) {
  s32 collision;

  target->terrainHeight += 16 << 16;
  collision = Map_CheckObjectStepFar((struct ObjectRuntime *)target,
                                     (struct FieldPosition *)position);
  target->terrainHeight -= 16 << 16;
  return collision;
}

/// When Carry is cast on a target, two hands lift it to 24 pixels above the
/// ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it
/// where it is.
///
/// Abilities: Carry (FIELD_KIND_CARRY).
///
/// Where the way is blocked a buzzer sounds.
void FieldEffect_SteerLiftedTarget(s32 targetId) {
  struct FieldFxScene *work;
  struct FieldActor *src;
  struct FieldActor *target;
  struct FieldActor *shards[CARRY_SHARD_MAX];
  struct EffectVector pos;
  struct FieldActor *hands[2];
  struct FieldActor *obj;
  struct FieldActor *right;
  s32 i;
  s32 angle;
  s32 dist;
  struct EffectVector *p;

  work = gEffectWork;
  src = work->mainObject;
  target = work->child;
  if (target == NULL)
    return;
  FieldEffect_FadeToEffectColor();
  src->linkedObject = target;
  ObjectDispatch_InitializeFar((struct DispatchObject *)src,
                               gFieldEffect_SourceHoldScript);

  // The hands appear 32 pixels to either side and move to 16 pixels.
  /* FAKEMATCH(temporary): the position reached through p everywhere but in
   * the steering loop's first part, which names it; as pos named
   * everywhere, 10 of 531 instructions differ in tbs-en. */
  p = &pos;
  SET_POSITION(p, work->x, work->y + (16 << 16), work->z);
  obj = (struct FieldActor *)FieldEffect_SpawnItemBreakMode1(
      p->x + (32 << 16), p->y, p->z, 0x8000);
  hands[0] = obj;
  right = (struct FieldActor *)FieldEffect_SpawnItemBreakMode1(
      p->x - (32 << 16), p->y, p->z, 0);
  hands[1] = right;
  if (hands[0] == NULL || right == NULL) {
    FieldEffect_FadeFromEffectColor();
    return;
  }
  WaitFrames(15);
  SET_POSITION(p, target->x.fixed, target->y.fixed + (16 << 16),
               target->z.fixed);
  Object_SetMoveTargetFar((struct ObjectRuntime *)obj, p->x + (16 << 16), p->y,
                          p->z);
  Object_SetMoveTargetFar((struct ObjectRuntime *)right, p->x - (16 << 16),
                          p->y, p->z);
  FieldObject_WaitForMoveFar(obj);
  FieldObject_WaitForMoveFar(right);
  obj->x.fixed = p->x + (16 << 16);
  obj->velocityX = 0;
  right->x.fixed = p->x - (16 << 16);
  right->velocityX = 0;

  // Lift to 24 pixels above the ground, 3/8 pixel a frame, floating (motion
  // flag bit 2) with no shadow and its colours cycling.
  target->update = ObjectGroup_ApplyRandomChildValues;
  Scheduler_AddOrUpdateCallback(FieldEffect_SpawnFallingParticles, 0xc80);
  AudioCommand_PlayFar(SE_130);
  target->motionFlags = 4;
  ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)target, 0);
  if (hands[0] != NULL && hands[1] != NULL) {
    while (target->y.fixed - target->terrainHeight <= 24 << 16) {
      obj->y.fixed += 0x6000;
      right->y.fixed += 0x6000;
      target->y.fixed += 0x6000;
      WaitFrames(1);
    }
  }
  hands[0]->speed = 4 << 16;
  hands[0]->acceleration = 0x8000;
  hands[1]->speed = 4 << 16;
  hands[1]->acceleration = 0x8000;
  // As in Move: the target keeps its facing and reads its ground from layer
  // 2 while it is carried.
  target->speed = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = 0;
  target->terrainId = 2;

  //------------------------------------------------------------------
  // Move with the d-pad.
  //------------------------------------------------------------------
  for (;;) {
    WaitFrames(1);
    if ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;

    // No direction held: the hands go back to the target's sides.
    angle = (u16)Input_GetDpadAngle(gKeysHeld);
    if (angle == 0xffff) {
      pos.x = target->x.fixed;
      pos.y = target->y.fixed + (16 << 16);
      pos.z = target->z.fixed;
      Object_SetMoveTargetFar((struct ObjectRuntime *)hands[0],
                              pos.x + (16 << 16), pos.y, pos.z);
      Object_SetMoveTargetFar((struct ObjectRuntime *)hands[1],
                              pos.x - (16 << 16), pos.y, pos.z);
      ObjectDispatch_SelectAnimationFar(hands[0], 1);
      ObjectDispatch_SelectAnimationFar(hands[1], 1);
      continue;
    }

    // The hands lean 2 pixels that way; check the ground 16 pixels on.
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + (16 << 16);
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(2 << 16, angle, &pos);
    Object_SetMoveTargetFar((struct ObjectRuntime *)hands[0],
                            pos.x + (16 << 16), pos.y, pos.z);
    Object_SetMoveTargetFar((struct ObjectRuntime *)hands[1],
                            pos.x - (16 << 16), pos.y, pos.z);
    FieldObject_WaitForMoveFar(hands[0]);
    FieldObject_WaitForMoveFar(hands[1]);
    pos.x = target->x.fixed;
    pos.y = target->terrainHeight;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &pos);
    i = ScriptObject_CheckOverlapFar((struct ScriptObjectEntry *)target,
                                     (s32 *)&pos);
    // Both tests jump into the blocked path, which keeps it ahead of the drop
    // path as the ROM lays them out; written as one || test, 407 of 531
    // instructions differ in tbs-en.
    if (i != 0)
      goto blocked;
    if (FieldEffect_CheckRaisedTargetCollision(target, p) > 0) {
    blocked: // the way is blocked: a buzzer every 16 frames
      ObjectDispatch_SelectAnimationFar(hands[0], 4);
      ObjectDispatch_SelectAnimationFar(hands[1], 4);
      if ((gFrameCount & 15) == 0)
        AudioCommand_PlayFar(SE_MENU_ERROR);
      continue;
    }
    // The way is free: move the target there.
    {
      struct FieldActor *second;
      s32 x;
      s32 z;

      AudioCommand_PlayFar(SE_175);
      x = p->x;
      z = p->z;
      ObjectDispatch_SelectAnimationFar(hands[0], 4);
      ObjectDispatch_SelectAnimationFar(hands[1], 4);
      WaitFrames(15);
      // i is the overlap test's 0 here: unpause the target and, once it is
      // on the tile, stop it dead. It moves at 0.2 pixels a frame.
      target->paused = i;
      target->speed = 0x3333;
      target->acceleration = 0x3333;
      Object_SetMoveTargetFar((struct ObjectRuntime *)target, p->x, p->y, p->z);
      hands[0]->speed = 0x3333;
      hands[0]->acceleration = 0x3333;
      second = hands[1];
      second->speed = 0x3333;
      second->acceleration = 0x3333;
      Object_SetMoveTargetFar((struct ObjectRuntime *)hands[0],
                              p->x + (16 << 16), p->y, p->z);
      Object_SetMoveTargetFar((struct ObjectRuntime *)second, p->x - (16 << 16),
                              p->y, p->z);
      FieldObject_WaitForMoveFar(target);
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = i;
      target->velocityZ = i;
      WaitFrames(10);
      break;
    }
  }

  //------------------------------------------------------------------
  // Shards fly from the target, the hands vanish, and the target hops and
  // drops.
  //------------------------------------------------------------------
  ObjectDispatch_SelectAnimationFar(hands[0], 4);
  ObjectDispatch_SelectAnimationFar(hands[1], 4);
  Scheduler_RemoveCallback(FieldEffect_SpawnFallingParticles);
  AudioCommand_PlayFar(SE_135);
  WaitFrames(15);
  AudioCommand_PlayFar(SE_135);
  WaitFrames(15);
  SET_POSITION(p, target->x.fixed, target->y.fixed + (16 << 16),
               target->z.fixed);
  {
    struct FieldActor **walk = shards;

    // Shards: 2 to 3 pixels a frame, 8 to 32 pixels in a random direction.
    for (i = 0; i < CARRY_SHARD_MAX; i++) {
      obj = (struct FieldActor *)Object_Spawn(SPRITE_SPARKLE, p->x, p->y, p->z);
      *walk++ = obj;
      if (obj != NULL) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                     gFieldEffect_FragmentScript);
        obj->speed = Random16() + (2 << 16);
        obj->acceleration = 2 << 16;
        obj->motionFlags = 0;
        dist = Random16() * 24 + (8 << 16);
        Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)obj,
                                                   dist, Random16());
      }
    }
  }
  AudioCommand_PlayFar(SE_131);
  ObjectDispatch_ReleaseFar((struct DispatchObject *)hands[0]);
  ObjectDispatch_ReleaseFar((struct DispatchObject *)hands[1]);

  // Restore the target's look and motion, and throw it up at 10 pixels a
  // frame: it falls and follows the terrain again, keeping 0.2 of its speed
  // as it bounces, and reads its ground from the ground layer.
  Animation_ApplyChildValuesFar((struct DispatchObject *)target,
                                work->savedPalette);
  ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                               work->savedScript);
  target->update = work->savedCallback;
  target->motionFlags = 3;
  target->velocityY = 10 << 16;
  target->bounce = 0x3333;
  target->terrainId = 0;
  src->update = 0;
  Animation_ApplyChildValuesFar((struct DispatchObject *)src,
                                SPRITE_COLOR_NORMAL);
  if (work->childOption != 0) {
    // Wait for it to rise and land (at most 90 frames each), then a ring.
    for (i = 0; i < 90 && target->velocityY >= 0; i++)
      WaitFrames(1);
    WaitFrames(1);
    for (i = 0; i < 90 && target->velocityY < 0; i++)
      WaitFrames(1);
    FieldEffect_SpawnRadialParticleRing((struct MotionObject *)target);
    FieldEffect_FadeFromEffectColor();
    WaitFrames(30);
  } else {
    FieldEffect_FadeFromEffectColor();
  }
}

/// When Carry is cast with no target, two hands appear 16 pixels above the spot
/// in front of the caster and 32 pixels to either side, close in 24 pixels and
/// scatter into shards.
///
/// Abilities: Carry (FIELD_KIND_CARRY).
DEFINE_ITEM_BREAK_EFFECT(FieldEffect_RunCarry, FieldEffect_SpawnItemBreakMode1)

/// Spawns a hand (object 215) with the item break sound, in animation 1, and
/// grows it, spinning 1/8 turn a frame, from a quarter to full size over 24
/// frames; angle is the way it faces once grown. Returns the hand, or NULL
/// when it cannot be spawned (Object_Spawn); Carry and its no-target effect
/// release it.
struct MotionObject *FieldEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                     s32 angle) {
  struct MotionObject *fragment;
  s32 scale;

  AudioCommand_PlayFar(SE_ITEM_BREAK);
  fragment = (struct MotionObject *)Object_Spawn(215, x, y, z);
  if (fragment != NULL) {
    fragment->scaleY = 0x4000;
    fragment->scaleX = 0x4000;
    fragment->speedLimit = 0x30000;
    fragment->acceleration = 0x30000;
    fragment->autoFaceMotion = 0;
    ObjectDispatch_SelectAnimationFar(fragment, 1);
    scale = fragment->scaleX;
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        fragment->scaleY = scale;
        fragment->scaleX = scale;
        fragment->angle += 0x2000;
        WaitFrames(1);
        scale = fragment->scaleX;
      } while (scale <= 0xffff);
    }
    fragment->angle = angle;
  }
  return fragment;
}

#undef CARRY_SHARD_MAX
