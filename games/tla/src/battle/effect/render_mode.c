/// @file
/// Battle effects: particle effect entry points.

#include "types.h"
#include "battle.h"
#include "battle_effect_work.h"

/// Each entry passes a particle type (color and count) to
/// BattleEffect_RunTargetMotes. effect: effect arguments.
void BattleEffect_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunTargetMotes(effect, 0);
    return;
  }
  BattleEffect_RunTargetMotes(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunTargetMotesMode1, BattleEffect_RunTargetMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunDelude, BattleEffect_RunTargetMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunParalyze, BattleEffect_RunTargetMotes, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunHeartrender, BattleEffect_RunTargetMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunMysticFlame, BattleEffect_RunTargetMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunDemonFire, BattleEffect_RunTargetMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunDrown, BattleEffect_RunTargetMotes, 6)
