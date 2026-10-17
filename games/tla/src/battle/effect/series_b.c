#include "types.h"

struct BattleEffectArgument;
void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Demon Night (kind 50).
void BattleFx_RunDemonNight(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 0);
}

// Abilities: Rumble (kind 308).
void BattleFx_RunRumble(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 1);
}

// Abilities: Haunting (kind 351).
void BattleFx_RunHaunting(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 2);
}

// Abilities: Thorny Grave (kind 127).
void BattleFx_RunThornyGrave(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 3);
}
