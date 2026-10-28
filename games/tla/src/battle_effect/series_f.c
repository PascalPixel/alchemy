#include "types.h"
#include "battle_effect_argument.h"

void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb (kind
/// 15).
DEFINE_EFFECT_KIND(BattleEffect_RunNova, BattleEffect_RunSparkGroups, 0)

/// Abilities: Dynamite, Angry Mine (kind 338).
DEFINE_EFFECT_KIND(BattleEffect_RunDynamite, BattleEffect_RunSparkGroups, 1)
