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
void BattleFx_RunTargetMotesMode0Or1(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunTargetMotes(effect, 0);
    return;
  }
  BattleFx_RunTargetMotes(effect, 1);
}

void BattleFx_RunTargetMotesMode1(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 1);
}

void BattleFx_RunTargetMotesMode2(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 2);
}

void BattleFx_RunTargetMotesMode3(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 3);
}

void BattleFx_RunTargetMotesMode4(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 4);
}

void BattleFx_RunTargetMotesMode5(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 5);
}

void BattleFx_RunTargetMotesMode7(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 7);
}

void BattleFx_RunTargetMotesMode6(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 6);
}
