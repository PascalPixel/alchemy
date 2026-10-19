#include "types.h"
#include "battle_effect_work.h"

void Func_0818bc98(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Beam, Cycle Beam, Searing Beam (kind 73).
DEFINE_EFFECT_KIND(BattleFx_RunBeam, Func_0818bc98, 0)

/// Abilities: Rolling Flame (kind 434).
void BattleFx_RunRollingFlame(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  Func_0818bc98(effect, 1);
}

/// Abilities: Ghost Soldier, Pike Assault (kind 449).
void BattleFx_RunGhostSoldier(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  Func_0818bc98(effect, 2);
}

/// Abilities: Blast Breath (kind 414).
DEFINE_EFFECT_KIND(BattleFx_RunBlastBreath, Func_0818bc98, 3)
