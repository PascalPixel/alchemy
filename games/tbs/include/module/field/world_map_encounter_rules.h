#ifndef ALCHEMY_MODULE_FIELD_WORLD_MAP_ENCOUNTER_RULES_H
#define ALCHEMY_MODULE_FIELD_WORLD_MAP_ENCOUNTER_RULES_H

#include "types.h"

/// The World Map's encounter zone for an encounter region and battle
/// environment. Read by Encounter_ResolveWorldMapZoneAndBackdrop, first match
/// first.
struct WorldMapEncounterRule {
  /// The World Map cell's encounter region (enum WorldMapEncounterRegion); -1
  /// ends the table.
  s16 region;
  /// The battle environment (enum BattleEnvironment), which also picks the
  /// battle background (BattleBackground_SelectForEnvironment), or -1 for any.
  s16 environment;
  s16 flag; ///< -1, or a flag that turns the rule off once set
  s16 zone; ///< a row of encounter_zones.c
};

/// A Djinni battle: the formation in slot `slot` of encounter zone `zone`.
struct DjinniBattle {
  u16 zone;
  u16 slot;
};

extern const struct WorldMapEncounterRule gWorldMap_EncounterRules[];
extern const struct DjinniBattle gEncounter_DjinnBattles[];

#endif
