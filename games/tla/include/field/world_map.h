#ifndef ALCHEMY_WORLD_MAP_H
#define ALCHEMY_WORLD_MAP_H

// The destinations the map screen marks. Its state, WorldMapStateWork, is the system library's
// (system/definitions.h), as the variable is.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"

// The game flag that marks a World Map place visited, from its WORLD_MAP_PLACE.flags. The first of
// those flags, a place's WORLD_MAP_PLACE.flags being added to it.
#define FLAG_PLACE_VISITED_FIRST  0x180
#define FLAG_PLACE_VISITED(place) ((place) + FLAG_PLACE_VISITED_FIRST)

// WORLD_MAP_PLACE.value's markers, besides the marker pictures 1 to 14.
typedef enum WORLD_MAP_MARKER_KIND
{
	WORLD_MAP_MARKER_NONE = 0, // the place is not drawn
	// The ship's marker, drawn at the ship once it sails and the party is ashore.
	WORLD_MAP_MARKER_SHIP = 15
} WORLD_MAP_MARKER_KIND;

// A world map destination, keyed by its flag (fdWorldMapMarkPlaceVisited sets
// FLAG_PLACE_VISITED(flag)); fdWorldMapPlacesGetTable returns them, ended by a zero flag.
typedef struct WORLD_MAP_PLACE
{
	Uint8  flags;
	Uint8  value; // its markers: low nibble, high nibble on the map screen
	// The actor the World Map places there (scWorldMapUpdatePlacedActors): its sprite, place in
	// pixels and facing, and the flag it is shown by (-1 always; bit 12 set: shown while the rest
	// is set, else while clear).
	Sint16 sprite;
	Sint16 x;
	Sint16 z;
	Uint16 facing;
	Sint16 shownFlag;
	// The scene and entrance whose arrival marks it visited (fdWorldMapMarkPlaceVisited), and whose
	// area name the map screen shows for it; an entrance of -1 matches any.
	Sint16 visitScene;
	Sint16 visitEntrance;
	Sint16 scene;    // where choosing it leads
	Sint16 entrance; // that scene's entrance
} WORLD_MAP_PLACE;

STATIC_ASSERT(offsetof(WORLD_MAP_PLACE, visitScene) == 12);
STATIC_ASSERT(offsetof(WORLD_MAP_PLACE, scene) == 16);
STATIC_ASSERT(sizeof(WORLD_MAP_PLACE) == 20);

// The most places fdWorldMapPlacesGetTable lists.
#define MAP_SCREEN_PLACE_COUNT  128
// The step cue the actor of WorldMapPlacedActors' first place raises when touched; each next
// place's actor raises the next.
#define STEP_CUE_AREA_BASE      100
// BG1's map screen image, of 256 colours: 32 by 24 tiles from char block 1 on, which it scrolls
// through, wrapping, its map numbering them from the fifth row, where the still image starts.
#define MAP_SCREEN_FIRST_TILE   (BG_CHAR_SIZE / TILE_SIZE_8BPP)
#define MAP_SCREEN_TILE_ROWS    24
#define MAP_SCREEN_MAP_START    (4 * BG_SCREEN_WIDTH)

// A World Map position, in whole pixels, as a point on the map screen.
#define MAP_SCREEN_X(x)         ((x) * 853 / 16384 - 269)
#define MAP_SCREEN_Y(z)         ((z) * 640 / 16384 - 112)

// The depth the map screen's markers and cursor are drawn at.
#define MAP_SCREEN_MARKER_DEPTH 246
Uint32             fdWorldMapPlaceGetMarker(const WORLD_MAP_PLACE *place);

// fdWorldMapUpdateAffineLines' Thumb entry and ARM code (world_map_lines_entry.s), copied to RAM to
// run.
extern const Uint8 fdWorldMapUpdateAffineLinesCode[];

#endif
