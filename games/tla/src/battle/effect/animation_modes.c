//--------------------------------------------------------------------------
// animation_modes.c
// Effects: burst effect entry points.
//--------------------------------------------------------------------------

#include "effect_step.h"

void BattleFx_RunBurstScene(struct EffectStep *, s32);

//--------------------------------------------------------------------------
// Each entry passes a burst type to BattleFx_RunBurstScene.
// step: effect arguments.
//--------------------------------------------------------------------------
void BattleFx_RunGust(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleFx_RunBurstScene(step, 6);
    return;
  }
  if (step->variant == 1) {
    BattleFx_RunBurstScene(step, 7);
    return;
  }
  BattleFx_RunBurstScene(step, 8);
}

void BattleFx_RunBearClaw(struct EffectStep *step) {
  BattleFx_RunBurstScene(step, 0);
}

void BattleFx_RunRansack(struct EffectStep *step) {
  BattleFx_RunBurstScene(step, 0);
}

void BattleFx_RunBacteriaRush(struct EffectStep *step) {
  BattleFx_RunBurstScene(step, 1);
}

void BattleFx_RunFreebiteRush(struct EffectStep *step) {
  BattleFx_RunBurstScene(step, 2);
}

void BattleFx_RunFlyingAttack(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleFx_RunBurstScene(step, 3);
    return;
  }
  if (step->variant == 1) {
    BattleFx_RunBurstScene(step, 4);
    return;
  }
  BattleFx_RunBurstScene(step, 5);
}
