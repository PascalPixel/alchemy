#ifndef ALCHEMY_SCENE_GROUND_EFFECTS_H
#define ALCHEMY_SCENE_GROUND_EFFECTS_H

// The map state's ground effects byte as the scenes write it: its footprints and the kind of
// wading. map_grid.h keeps the byte whole.

#include "field/map_grid.h"

// The map's kind of wading (GROUND_EFFECTS_WADING).
typedef enum MAP_WADING
{
	// A room with a second level: the leader wades only on the level the water is on.
	MAP_WADING_UPPER_LEVEL = 1,
	MAP_WADING_MUD         = 2 // Taopo Swamp: the leader sinks into the mud
} MAP_WADING;

// MAP_STATE as far as groundEffects, read through its bits.
typedef struct MAP_STATE_GROUND_EFFECTS
{
	Uint8 before[offsetof(MAP_STATE, groundEffects)];
	Uint8 footprints : 2;
	Uint8 wading     : 2;
	Uint8 unused     : 4;
} MAP_STATE_GROUND_EFFECTS;

// Sets the map's kind of wading to that of a room with a second level.
#define fdMapSetUpperLevelWading(map)                                                              \
	(((MAP_STATE_GROUND_EFFECTS *)(map))->wading = MAP_WADING_UPPER_LEVEL)

#endif
