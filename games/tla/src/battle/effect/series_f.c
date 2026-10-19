#include "types.h"
#include "battle_effect_argument.h"

void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb (kind
/// 15).
DEFINE_EFFECT_KIND(BattleFx_RunNova, BattleFx_RunSparkGroups, 0)

/// Abilities: Dynamite, Angry Mine (kind 338).
DEFINE_EFFECT_KIND(BattleFx_RunDynamite, BattleFx_RunSparkGroups, 1)
