#ifndef ALCHEMY_SCENE_GROUND_EFFECTS_H
#define ALCHEMY_SCENE_GROUND_EFFECTS_H

/// @file
/// The map work's ground effects byte as the scenes write it: its footprints
/// and the kind of wading. map_grid.h keeps the byte whole.

#include "field/map_grid.h"

/// The kind of wading a room with a second level sets, so that the leader
/// wades only on the level the water is on.
enum { MAP_WADING_UPPER_LEVEL = 1 };

/// MapWork as far as groundEffects, read through its bits.
struct MapWorkGroundEffects {
  u8 before[offsetof(struct MapWork, groundEffects)];
  u8 footprints : 2;
  u8 wading : 2;
  u8 unused : 4;
};

/// Sets the map's kind of wading to that of a room with a second level.
#define Map_SetUpperLevelWading(map)                                           \
  (((struct MapWorkGroundEffects *)(map))->wading = MAP_WADING_UPPER_LEVEL)

#endif
