#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "effect_step.h"

void EffectStep_RunAnimationMode9(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 9);
}

void EffectStep_RunAnimationMode12(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 12);
}

void EffectStep_RunAnimationMode10(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 10);
}

void EffectStep_RunAnimationMode11(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 11);
}

void EffectStep_RunAnimationMode6Or7Or8B(struct EffectStep *step) {
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
