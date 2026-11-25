/// @file
/// Lift: two hands grip the target and raise it, and Force's flashes and
/// heavy impact, linked beside it.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// A hand appears 32 pixels to either side, each moves 14 pixels in, and they
/// lift the target 32 pixels with them.
void FieldPsynergy_LiftOnTarget(s32 unused) {
  struct FieldEffectState *effectState;
  struct Actor *caster;
  struct Actor *target;
  void *hands[2]; // right then left
  FIXED pos[3];
  s32 x;
  FIXED y;
  s32 z;
  s32 handIndex;
  FIXED startY;

  effectState = gFieldEffectState;
  target = effectState->target;
  caster = effectState->caster;
  if (target == NULL)
    return;

  FieldPsynergy_FadeToCastColor();
  // The caster faces its target while Lift holds it.
  caster->watchedActor = (union ActorOrEffect *)target;
  Actor_StartScriptFar(caster, gFieldEffect_SourceHoldScript);

  x = effectState->x;
  pos[0] = x;
  y = effectState->y + toFIXED(16);
  pos[1] = y;
  z = effectState->z;
  pos[2] = z;
  hands[0] = FieldEffect_SpawnLiftHand(x + toFIXED(32), y, z, FACING_WEST);
  hands[1] = FieldEffect_SpawnLiftHand(pos[0] - toFIXED(32), pos[1], pos[2],
                                       FACING_EAST);

  // Each hand faces the target (FACING_WEST on the right, FACING_EAST on the
  // left) and moves 14 pixels that way.
  wait(15);
  for (handIndex = 0; handIndex < 2; handIndex++) {
    struct Actor *hand = hands[handIndex];
    if (hand != NULL)
      Actor_SetTargetPositionFromMagnitudeAngle(hand, toFIXED(14),
                                                hand->facing);
  }

  Actor_WaitForMoveFar(hands[0]);
  target->update = Actor_CycleSpriteColors;
  Sound_PlayFar(SE_PSYNERGY_START);
  target->motionFlags = ACTOR_FLOATS;

  // Lift 32 pixels, a quarter pixel at a time.
  startY = target->y.fixed;
  if (hands[0] != NULL && hands[1] != NULL) {
    while (target->y.fixed <= startY + toFIXED(32)) {
      ((struct Actor *)hands[0])->y.fixed += toFIXED(1) / 4;
      ((struct Actor *)hands[1])->y.fixed += toFIXED(1) / 4;
      target->y.fixed += toFIXED(1) / 4;
      wait(1);
    }
  }

  // The hands burst into particles; the target stays up.
  FieldEffect_BurstHand(hands[0]);
  FieldEffect_BurstHand(hands[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Two hands appear 16 pixels above the spot in front of the caster, 32 pixels
/// to either side, then close in 24 pixels and clap, and sparkles scatter.
DEFINE_CLAPPING_HANDS_CAST(FieldPsynergy_Lift, FieldEffect_SpawnLiftHand)

/// Spawns a hand (SPRITE_MOVE_HAND) and grows it, spinning, from a quarter to
/// full size over 24 frames; angle is the way it faces once grown.
struct Actor *FieldEffect_SpawnLiftHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *hand;
  FIXED scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  hand = Actor_Spawn(SPRITE_MOVE_HAND, x, y, z);
  if (hand != NULL) {
    hand->scaleX = hand->scaleY = FIXED_ONE / 4;
    hand->acceleration = hand->speedLimit = toFIXED(3);
    ((struct Sprite *)hand->sprite)->image.priority = 0;
    Actor_SelectAnimationFar(hand, MOVE_HAND_ANIMATION_GRIP);
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

/// Field effect sprite callback of a particle that scatters 25 pixels from its
/// origin in a random direction, then gathers in front of the caster and is
/// freed: each of Force's FORCE_FLASHES flashes (event_commands_force.c).
void FieldEffect_UpdateRadialBurst(struct FieldParticle *effect) {
  // OUT_AND_BACK_FLYING_OUT must enter OUT_AND_BACK_AIM_BACK immediately.
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Vector3 pos;
  s32 state;

again:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    // 25 pixels from the origin, in a random direction.
    pos.x = effect->originX;
    pos.z = effect->originZ;
    Vector_AddPolarOffset(toFIXED(25), (u16)Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = toFIXED(3);
    effect->speedMax = toFIXED(3);
    effect->turnStepLimited = 0;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_OUT) {
    // On arrival, go straight on.
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto again;
    }
    return;
  }

  if (state == OUT_AND_BACK_AIM_BACK) {
    // 8 pixels toward effectState->angle from 16 pixels above the caster,
    // on screen, give or take 4 pixels.
    struct Actor *caster = effectState->caster;

    pos.x = caster->x.fixed;
    pos.y = caster->y.fixed + toFIXED(16);
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(toFIXED(8), effectState->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(toFIXED(4), Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->turnStepMax = TRIG_TURN / 32;
    effect->turnStepLimited = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

#define HEAVY_IMPACT_SPARKLES 12

/// A heavy impact: places a star burst (SPRITE_STAR_BURST) 32 pixels below the
/// source for 20 frames and scatters HEAVY_IMPACT_SPARKLES sparkles
/// (SPRITE_SPARKLE) from it.
s32 FieldEffect_SpawnHeavyImpact(struct Actor *source) {
  struct Vector3 pos;
  struct Actor *impact;
  struct Actor *sparkle;
  s32 sparkleIndex;

  Sound_PlayFar(SE_HEAVY_IMPACT);
  pos.x = source->x.fixed;
  pos.y = source->y.fixed;
  pos.z = source->z.fixed;
  impact = Actor_Spawn(SPRITE_STAR_BURST, pos.x, pos.y - toFIXED(32), pos.z);
  if (impact != NULL) {
    impact->motionFlags = 0;
    impact->wait = 20;
    Actor_StartScriptFar(impact, gFieldEffect_SharedParticleScript);
  }

  // Sparkles: 1 to 2 pixels a frame, 8 to 32 pixels in a random direction.
  for (sparkleIndex = 0; sparkleIndex < HEAVY_IMPACT_SPARKLES; sparkleIndex++) {
    sparkle = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (sparkle != NULL) {
      Actor_StartScriptFar(sparkle, gFieldEffect_FragmentScript);
      sparkle->speedLimit = Random16() + toFIXED(1);
      sparkle->acceleration = toFIXED(1);
      sparkle->motionFlags = 0;
      Actor_SetTargetPositionFromMagnitudeAngle(
          sparkle, Random16() * 24 + toFIXED(8), Random16());
    }
  }
  return 0;
}

#undef HEAVY_IMPACT_SPARKLES
