#include "types.h"

struct BattleEffectArgument;

void Func_0816a2d8(struct BattleEffectArgument *effect, s32 variant);

/// Abilities: Quick Strike (kind 52).
void BattleEffect_RunQuickStrike(s32 a0) { Func_0816a2d8(a0, 1); }

/// Abilities: Vorpal Slash (kind 65).
void BattleEffect_RunVorpalSlash(s32 a0) { Func_0816a2d8(a0, 2); }

/// Abilities: Lunar Slash (kind 118).
void BattleEffect_RunLunarSlash(s32 a0) { Func_0816a2d8(a0, 3); }

/// Abilities: Boost Hack (kind 109).
void BattleEffect_RunBoostHack(s32 a0) { Func_0816a2d8(a0, 4); }

/// Abilities: Twin Shear (kind 425).
void BattleEffect_RunTwinShear(s32 a0) { Func_0816a2d8(a0, 5); }
