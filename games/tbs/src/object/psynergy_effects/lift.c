/// @file
/// Lift.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// A hand appears 32 pixels to either side, each moves 14 pixels in, and
/// they lift the target 32 pixels with them.
///
/// Abilities: Lift (FIELD_KIND_LIFT).
///
/// This is Lift cast on a target, which FieldEffect_DispatchRequestKind calls.
/// It leaves the target raised and floating (motion flag bit 2), with its
/// update set to ObjectGroup_ApplyRandomChildValues.
void FieldEffect_RunLiftOnTarget(s32 unused) {
  struct FieldEffectWork *work;
  struct DispatchObject *caster;
  struct ObjectEvent *target;
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

  FieldEffect_FadeToEffectColor();
  // The caster's script starts 3 words into the common particle script, with
  // the target as its argument.
  caster->argument = (s32)target;
  ObjectDispatch_InitializeFar(caster, gFieldEffect_CommonParticleScript + 3);

  x = work->x;
  pos[0] = x;
  y = work->y + 0x100000; // 16 pixels up
  pos[1] = y;
  z = work->z;
  pos[2] = z;
  pair[0] = FieldEffect_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
  pair[1] =
      FieldEffect_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

  // Each hand faces the target (angle 0x8000 on the right, 0 on the left) and
  // moves 14 pixels that way.
  WaitFrames(15);
  for (handIndex = 0; handIndex < 2; handIndex++) {
    struct MotionObject *hand = pair[handIndex];
    if (hand != NULL)
      Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)hand,
                                                 0xe0000, hand->angle);
  }

  FieldObject_WaitForMoveFar(pair[0]);
  target->update = ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(SE_130);
  target->motionFlags = OBJ_EVENT_FLOATS;

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

  // The hands burst into particles; the target stays up.
  UpdateRisingParticleBurst(pair[0]);
  UpdateRisingParticleBurst(pair[1]);
  FieldEffect_FadeFromEffectColor();
}

/// Two hands appear 16 pixels above the spot in front of the caster, 32
/// pixels to either side, then close in 24 pixels and scatter into shards.
///
/// Abilities: Lift (FIELD_KIND_LIFT).
///
/// This is Lift cast with no target, which FieldEffect_Run calls.
DEFINE_ITEM_BREAK_EFFECT(FieldEffect_RunLift, FieldEffect_SpawnItemBreakMode3)

/// Spawns a hand (object 215) and grows it, spinning, from a quarter to full
/// size over 24 frames; angle is the way it faces once grown. Makes Lift's
/// two hands, for FieldEffect_RunLiftOnTarget and FieldEffect_RunLift; returns
/// NULL when no object could be spawned. Unlike Carry's
/// (FieldEffect_SpawnItemBreakMode1) it takes animation 3, not 1, and keeps
/// autoFaceMotion as created; both are at sprite priority 0 (Object_Spawn).
struct MotionObject *FieldEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                     s32 angle) {
  struct MotionObject *fragment;
  s32 scale;

  AudioCommand_PlayFar(SE_ITEM_BREAK);
  fragment = (struct MotionObject *)Object_Spawn(215, x, y, z);
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
/// Set on Force's 12 flashes by FieldEffect_RunForce in force.c.
void FieldEffect_UpdateRadialBurst(struct EffectSlot *effect) {
  // A for/continue retry keeps the same states but grows the native English
  // callback from 0xf8 to 0xfc bytes. OUT_AND_BACK_FLYING_OUT must enter
  // OUT_AND_BACK_AIM_BACK immediately.
  struct FieldEffectWork *effectWork = gEffectWork;
  struct EffectVector pos;
  s32 state;

again:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    // 25 pixels from the origin, in a random direction.
    pos.x = effect->originX;
    pos.z = effect->originZ;
    Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 3 << 16;
    effect->speedMax = 3 << 16;
    effect->turnStepLimited = 0;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_OUT) {
    // On arrival, go straight on.
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto again;
    }
    return;
  }

  if (state == OUT_AND_BACK_AIM_BACK) {
    // 8 pixels toward scene->angle from 16 pixels above the main object, on
    // screen, give or take 4 pixels.
    struct MotionObject *caster = effectWork->mainObject;

    pos.x = caster->x;
    pos.y = caster->y + (16 << 16);
    pos.z = caster->z;
    Vector_AddPolarOffset(8 << 16, effectWork->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->turnStepMax = 0x800;
    effect->turnStepLimited = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      EffectSlot_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// A heavy impact: places object 0x11b 32 pixels below the source for 20
/// frames and scatters 12 shards from it. Reached through SC_CALL in
/// gFieldEffect_HeavyImpactScript (fxdata.c), where the interpreter is the
/// object running the script, which Force's thrown object runs (force.c).
/// Returns 0, so the script moves on and frees the object.
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
    impact->wait = 20;
    ObjectDispatch_InitializeFar((struct DispatchObject *)impact,
                                 gFieldEffect_CommonParticleScript);
  }

  // Shards: 1 to 2 pixels a frame, 8 to 32 pixels in a random direction.
  for (fragmentIndex = 0; fragmentIndex < 12; fragmentIndex++) {
    fragment = Object_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (fragment != NULL) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)fragment,
                                   gFieldEffect_FragmentScript);
      fragment->speedLimit = Random16() + (1 << 16);
      fragment->acceleration = 1 << 16;
      fragment->flags = 0;
      Motion_SetTargetPositionFromMagnitudeAngle(
          fragment, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}
