#include "types.h"

struct BattleEffectArgument;
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb (kind
// 15).
void BattleFx_RunNova(struct BattleEffectArgument *effect) {
  BattleFx_RunSparkGroups(effect, 0);
}

// Abilities: Dynamite, Angry Mine (kind 338).
void BattleFx_RunDynamite(struct BattleEffectArgument *effect) {
  BattleFx_RunSparkGroups(effect, 1);
}
