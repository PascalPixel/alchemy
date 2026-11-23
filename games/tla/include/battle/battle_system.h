#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

/// @file
/// The units, abilities and random numbers the battle reads from the other
/// banks.

#include "battle_event.h"
#include "battle/battle_type.h"
#include "edition.h"

struct AbilityDefinition *Ability_GetDirect(s32 abilityId);
/// Returns a unit's record, a party member's or an enemy's.
struct Unit *Unit_GetFar(s32 unitId);
/// Returns the definition of an ability by its low 14 bits, the first for an id
/// past the table (Ability_GetDirect); every bank reads abilities so.
struct AbilityDefinition *Ability_Get(s32 abilityId);
/// Returns whether a game flag is set.
s32 GameFlag_GetFar(s32 flag);
/// Recomputes the unit's HP and PP gauges from its current HP and PP.
void Unit_RecalculateGaugesFar(s32 unitId);
/// Returns the battle's next random number, bits 8 to 23 of its new seed.
s32 Battle_RandomFar(void);
/// Returns 1 for the revive effects, which only work on a downed unit.
s32 Battle_IsReviveEffectFar(s32 effect);
u32 Ability_IsPsynergy(s32 actionId);
s32 Battle_ClassifyEntryKind(const struct AbilityDefinition *entry);

#endif
