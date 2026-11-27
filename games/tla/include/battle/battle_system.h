#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

/* @file */
/* The units, abilities and random numbers the battle reads from the other */
/* modules. */

#include "battle_event.h"
#include "battle/battle_type.h"
#include "edition.h"
#include "system.h"

/* Returns the definition of an ability by its low 14 bits, the first for an id
 */
/* past the table (gmAbilityGetDirect); every module reads abilities so. */
ABILITY_DEFINITION *btAbilityGet(s32 abilityId);
/* Recomputes the unit's HP and PP gauges from its current HP and PP. */
void gmUnitRecalculateGaugesStub(s32 unitId);
/* Returns the battle's next random number, bits 8 to 23 of its new seed. */
s32 btRandomStub(void);
/* Returns 1 for the revive effects, which only work on a downed unit. */
s32 btIsReviveEffectStub(s32 effect);
u32 btAbilityIsPsynergy(s32 actionId);
s32 mnBattleClassifyEntryKind(const ABILITY_DEFINITION *entry);

#endif
