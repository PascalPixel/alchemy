//--------------------------------------------------------------------------
// render_mode.c
// Battle effects: particle effect entry points.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "battle_effect_work.h"

//--------------------------------------------------------------------------
// Each entry passes a particle type (color and count) to
// BattleFx_RunTargetMotes. effect: effect arguments.
//--------------------------------------------------------------------------
void BattleFx_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunTargetMotes(effect, 0);
    return;
  }
  BattleFx_RunTargetMotes(effect, 1);
}

void BattleFx_RunTargetMotesMode1(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 1);
}

void BattleFx_RunDelude(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 2);
}

void BattleFx_RunParalyze(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 3);
}

void BattleFx_RunHeartrender(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 4);
}

void BattleFx_RunMysticFlame(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 5);
}

void BattleFx_RunDemonFire(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 7);
}

void BattleFx_RunDrown(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 6);
}
