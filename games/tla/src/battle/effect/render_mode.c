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

DEFINE_EFFECT_KIND(BattleFx_RunTargetMotesMode1, BattleFx_RunTargetMotes, 1)

DEFINE_EFFECT_KIND(BattleFx_RunDelude, BattleFx_RunTargetMotes, 2)

DEFINE_EFFECT_KIND(BattleFx_RunParalyze, BattleFx_RunTargetMotes, 3)

DEFINE_EFFECT_KIND(BattleFx_RunHeartrender, BattleFx_RunTargetMotes, 4)

DEFINE_EFFECT_KIND(BattleFx_RunMysticFlame, BattleFx_RunTargetMotes, 5)

DEFINE_EFFECT_KIND(BattleFx_RunDemonFire, BattleFx_RunTargetMotes, 7)

DEFINE_EFFECT_KIND(BattleFx_RunDrown, BattleFx_RunTargetMotes, 6)
