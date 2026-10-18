#include "types.h"
#include "battle_effect_argument.h"

void Func_08166b10(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Titan Blade (kind 61).
DEFINE_EFFECT_KIND(BattleFx_RunTitanBlade, Func_08166b10, 0)

// Abilities: Acheron's Grief (kind 104).
DEFINE_EFFECT_KIND(BattleFx_RunAcheronsGrief, Func_08166b10, 1)
