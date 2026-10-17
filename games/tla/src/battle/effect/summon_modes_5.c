#include "types.h"
#include "battle_effect_work.h"

void Func_0818bc98(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Beam, Cycle Beam, Searing Beam (kind 73).
void BattleFx_RunBeam(struct BattleEffectArgument *effect) {
  Func_0818bc98(effect, 0);
}

// Abilities: Rolling Flame (kind 434).
void BattleFx_RunRollingFlame(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  Func_0818bc98(effect, 1);
}

// Abilities: Ghost Soldier, Pike Assault (kind 449).
void BattleFx_RunGhostSoldier(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  Func_0818bc98(effect, 2);
}

// Abilities: Blast Breath (kind 414).
void BattleFx_RunBlastBreath(struct BattleEffectArgument *effect) {
  Func_0818bc98(effect, 3);
}
