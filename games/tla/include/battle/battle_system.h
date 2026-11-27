#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

/* The units, abilities and random numbers the battle reads from the other
   modules. */

#include "battle_event.h"
#include "battle/battle_type.h"
#include "edition.h"
#include "system.h"

/* Returns the definition of an ability by its low 14 bits, the first for an id
   past the table (gmAbilityGetDirect); every module reads abilities so. */
ABILITY_DEFINITION *gmAbilityGetDirectStub(Sint32 abilityId);
/* Recomputes the unit's HP and PP gauges from its current HP and PP. */
void gmUnitRecalculateGaugesStub(Sint32 unitId);
/* Returns the battle's next random number, bits 8 to 23 of its new seed. */
Sint32 gmBattleRandomStub(void);
/* Returns 1 for the revive effects, which only work on a downed unit. */
Sint32 gmBattleIsReviveEffectStub(Sint32 effect);
Uint32 btAbilityIsPsynergy(Sint32 actionId);
Sint32 mnBattleClassifyEntryKind(const ABILITY_DEFINITION *entry);

#endif
