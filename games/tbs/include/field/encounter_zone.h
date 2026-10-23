#ifndef ALCHEMY_ENCOUNTER_ZONE_H
#define ALCHEMY_ENCOUNTER_ZONE_H

/// @file
/// The encounter zones.

#include "types.h"

/// The number of formations an encounter zone can hold.
enum { ENCOUNTER_GROUP_COUNT = 8 };

/// An encounter zone, a row of gEncounterZones: its rate, the level it is made
/// for, and the formations that can appear with their chances. 28 bytes, the
/// 14 halfwords Encounter_GetFormationAtSlot counts a zone as.
struct EncounterZone {
  /// The base encounter rate; 0 for zone 0 (no encounters) and for a zone only
  /// scripted battles, mimics and Djinn use.
  u16 rate;
  u16 level; ///< the party level the zone is made for
  /// The formations that can appear, rows of battle/formations.c.
  u16 enemies[ENCOUNTER_GROUP_COUNT];
  u8 weights[ENCOUNTER_GROUP_COUNT]; ///< each formation's relative chance
};

#endif
