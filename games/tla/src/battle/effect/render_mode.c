//--------------------------------------------------------------------------
// render_mode.c
// Battle effects: particle effect entry points.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "battle_effect_work.h"

//--------------------------------------------------------------------------
// Each entry passes a particle type (color and count) to BattleFx_RenderMode.
// effect: effect arguments.
//--------------------------------------------------------------------------
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RenderMode(effect, 0);
    return;
  }
  BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode1(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode2(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 2);
}

void BattleFx_RenderMode3(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 3);
}

void BattleFx_RenderMode4(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 4);
}

void BattleFx_RenderMode5(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 5);
}

void BattleFx_RenderMode7(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 7);
}

void BattleFx_RenderMode6(struct BattleEffectArgument *effect) {
  BattleFx_RenderMode(effect, 6);
}
