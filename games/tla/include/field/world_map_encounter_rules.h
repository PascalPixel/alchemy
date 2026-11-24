#ifndef ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H
#define ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H

/// @file
/// Which encounter zone each part of the World Map takes.

#include "types.h"

/// The World Map's encounter zone for an encounter region and battle
/// environment. Read by Encounter_ResolveWorldMapZoneAndBackdrop, first match
/// first.
struct WorldMapEncounterRule {
  /// The World Map cell's encounter region (enum WorldMapEncounterRegion); -1
  /// ends the table.
  s16 region;
  /// The battle environment (enum BattleEnvironment), which also picks the
  /// battle background, or -1 for any.
  s16 environment;
  s16 flag; ///< -1, or a flag the rule needs set
  s16 zone; ///< a row of gEncounterZones
};

extern const struct WorldMapEncounterRule gWorldMap_EncounterRules[];
/// The same aboard the ship (LEADER_MOVEMENT_SHIP).
extern const struct WorldMapEncounterRule gWorldMap_ShipEncounterRules[];

#endif
