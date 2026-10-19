/// @file
/// Lift.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Abilities: Lift (field kind 7), cast on a target.
///
/// An object appears 32 pixels to either side, each moves 14 pixels in, and
/// they lift the target 32 pixels with them.
void FieldFx_RunLiftOnTarget(s32 unused) {
  struct BattleFxScene *work;
  struct DispatchObject *caster;
  struct FieldActor *target;
  struct MotionObject *pair[2];
  s32 pos[3];
  s32 x, y, z;
  s32 handIndex;
  s32 startY;

  work = gEffectWork;
  target = work->child;
  caster = work->mainObject;
  if (target == NULL)
    return;

  BattleEffect_InitializeSharedScene();
  caster->argument = (s32)target;
  ObjectDispatch_InitializeFar(caster, gBattleFx_CommonParticleScript + 3);

  x = work->x;
  pos[0] = x;
  y = work->y + 0x100000; // 16 pixels up
  pos[1] = y;
  z = work->z;
  pos[2] = z;
  pair[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
  pair[1] = BattleFx_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

  WaitFrames(15);
  for (handIndex = 0; handIndex < 2; handIndex++) {
    struct MotionObject *hand = pair[handIndex];
    if (hand != NULL)
      Motion_SetTargetPositionFromMagnitudeAngle(hand, 0xe0000, hand->angle);
  }

  FieldObject_WaitForMoveFar(pair[0]);
  target->update = ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(SE_130);
  target->motionFlags = 4;

  // Lift 32 pixels, a quarter pixel at a time.
  startY = target->y.fixed;
  if (pair[0] != NULL && pair[1] != NULL && startY <= startY + 0x200000) {
    do {
      pair[0]->y += 0x4000;
      pair[1]->y += 0x4000;
      target->y.fixed += 0x4000;
      WaitFrames(1);
    } while (target->y.fixed <= startY + 0x200000);
  }

  UpdateRisingParticleBurst(pair[0]);
  UpdateRisingParticleBurst(pair[1]);
  BattleFx_PrepareBufferInterpolation();
}

/// Abilities: Lift (field kind 7), cast with no target.
DEFINE_ITEM_BREAK_EFFECT(FieldFx_RunLift, BattleFx_SpawnItemBreakMode3)

/// Spawns object 215 and grows it, spinning, from a quarter to full size over
/// 24 frames; angle is the way it faces once grown.
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                  s32 angle) {
  struct MotionObject *fragment;
  s32 scale;

  AudioCommand_PlayFar(SE_ITEM_BREAK);
  fragment = Object_Spawn(215, x, y, z);
  if (fragment != NULL) {
    fragment->scaleX = fragment->scaleY = 0x4000;
    fragment->acceleration = fragment->speedLimit = 0x30000; // 3 pixels a frame
    ((struct AnimationObject *)fragment->records)->image.priority = 0;
    ObjectDispatch_SelectAnimationFar(fragment, 3);
    scale = fragment->scaleX;
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        fragment->scaleY = scale;
        fragment->scaleX = scale;
        fragment->angle += 0x2000; // 1/8 turn
        WaitFrames(1);
        scale = fragment->scaleX;
      } while (scale <= 0xffff);
    }
    fragment->angle = angle;
  }
  return fragment;
}

/// Effect slot callback of a particle that scatters 25 pixels from its origin
/// in a random direction, then gathers beside the main object and is freed.
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot) {
  // A for/continue retry keeps the same states but grows the native English
  // callback from 0xf8 to 0xfc bytes. State 1 must enter state 2 immediately.
  struct BattleFxScene *scene = gEffectWork;
  struct EffectVector pos;
  s32 state;

again:
  state = slot->state;
  if (state == 0) {
    // 25 pixels from the origin, in a random direction.
    pos.x = slot->originX;
    pos.z = slot->originZ;
    Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 3 << 16;
    slot->maxSpeed = 3 << 16;
    slot->limitTurnStep = 0;
    slot->state++;
    return;
  }

  if (state == 1) {
    // On arrival, go straight on.
    if (EffectSlot_HasPendingTarget(slot) == 0) {
      slot->state++;
      goto again;
    }
    return;
  }

  if (state == 2) {
    // 8 pixels toward scene->angle from 16 pixels above the main object, on
    // screen, give or take 4 pixels.
    struct MotionObject *caster = scene->mainObject;

    pos.x = caster->x;
    pos.y = caster->y + (16 << 16);
    pos.z = caster->z;
    Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->maxTurnStep = 0x800;
    slot->limitTurnStep = 1;
    slot->state++;
    return;
  }

  if (state == 3 && EffectSlot_HasPendingTarget(slot) == 0)
    BattleFx_ClearOwnedSlot(slot);
}

/// A heavy impact: places object 0x11b 32 pixels below the source and scatters
/// 12 shards from it.
s32 SpawnHeavyImpactEffect(struct ScriptInterpreter *interpreter) {
  struct MotionObject *source = (struct MotionObject *)interpreter;
  struct EffectVector pos;
  struct ObjectRuntime *impact;
  struct ObjectRuntime *fragment;
  s32 fragmentIndex;

  AudioCommand_PlayFar(SE_HEAVY_IMPACT);
  pos.x = source->x;
  pos.y = source->y;
  pos.z = source->z;
  impact = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
  if (impact != NULL) {
    impact->flags = 0;
    impact->unk5e = 20;
    ObjectDispatch_InitializeFar((struct DispatchObject *)impact,
                                 gBattleFx_CommonParticleScript);
  }

  // Shards: 1 to 2 pixels a frame, 8 to 32 pixels in a random direction.
  for (fragmentIndex = 0; fragmentIndex < 12; fragmentIndex++) {
    fragment = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
    if (fragment != NULL) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)fragment,
                                   gBattleFx_FragmentScript);
      fragment->speedLimit = Random16() + (1 << 16);
      fragment->acceleration = 1 << 16;
      fragment->flags = 0;
      Motion_SetTargetPositionFromMagnitudeAngle(
          fragment, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}
