#ifndef ALCHEMY_ENCOUNTER_ZONE_H
#define ALCHEMY_ENCOUNTER_ZONE_H

/// @file
/// What games/tla/src/field/field_sequence/ declares for the files that use it,
/// written once.

#include "types.h"

struct EncounterZone {
  u16 rate;
  u16 level;
  s16 enemies[8];
  u8 weights[8];
};

#endif
