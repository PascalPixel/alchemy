#include "types.h"
#include "battle_effect_argument.h"

void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Demon Night (kind 50).
DEFINE_EFFECT_KIND(BattleFx_RunDemonNight, Func_08144628, 0)

// Abilities: Rumble (kind 308).
DEFINE_EFFECT_KIND(BattleFx_RunRumble, Func_08144628, 1)

// Abilities: Haunting (kind 351).
DEFINE_EFFECT_KIND(BattleFx_RunHaunting, Func_08144628, 2)

// Abilities: Thorny Grave (kind 127).
DEFINE_EFFECT_KIND(BattleFx_RunThornyGrave, Func_08144628, 3)
