#include "types.h"
#include "battle_effect_work.h"

void Func_0818d5a8(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Planet Diver (kind 60).
void BattleFx_RunPlanetDiver(struct BattleEffectArgument *effect) {
  Func_0818d5a8(effect, 0);
}

// Abilities: Planetary (kind 128).
void BattleFx_RunPlanetary(struct BattleEffectArgument *effect) {
  Func_0818d5a8(effect, 1);
}
