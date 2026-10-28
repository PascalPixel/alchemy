/// @file
/// Effects: burst effect entry points.

#include "effect_step.h"

void BattleEffect_RunBurstScene(struct EffectStep *, s32);

/// Each entry passes a burst type to BattleEffect_RunBurstScene.
/// step: effect arguments.
void BattleEffect_RunGust(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 6);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 7);
    return;
  }
  BattleEffect_RunBurstScene(step, 8);
}

void BattleEffect_RunBearClaw(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 0);
}

void BattleEffect_RunRansack(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 0);
}

void BattleEffect_RunBacteriaRush(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 1);
}

void BattleEffect_RunFreebiteRush(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 2);
}

void BattleEffect_RunFlyingAttack(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 3);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 4);
    return;
  }
  BattleEffect_RunBurstScene(step, 5);
}
