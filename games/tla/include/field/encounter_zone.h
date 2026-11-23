#ifndef ALCHEMY_ENCOUNTER_ZONE_H
#define ALCHEMY_ENCOUNTER_ZONE_H

/// @file
/// A row of the encounter zone table, gEncounterZones (encounter_zones.c).

#include "types.h"

/// An encounter zone, a row of gEncounterZones: its rate, the level
/// it is made for, and the formations that can appear with their chances.
struct EncounterZone {
  /// The base encounter rate; 0 for a zone with no random encounters.
  u16 rate;
  u16 level;         ///< the party level the zone is made for
  s16 formations[8]; ///< the formations that can appear, by formation id
  u8 weights[8];     ///< each formation's relative chance
};

#endif
