#include "types.h"

struct BattleEffectArgument;
void Func_08151ff0(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Smoke Bomb, Sleep Bomb (kind 380).
void BattleFx_RunSmokeBomb(struct BattleEffectArgument *effect) {
  Func_08151ff0(effect, 1);
}

// Abilities: Blast, Mad Blast, Fiery Blast (kind 14).
void BattleFx_RunBlast(struct BattleEffectArgument *effect) {
  Func_08151ff0(effect, 0);
}
