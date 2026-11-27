#ifndef ALCHEMY_FIELD_MAP_H
#define ALCHEMY_FIELD_MAP_H

// The map's buffers in EWRAM and its shape table, the world map's blocks, and the map functions the
// scenes call.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/map_grid.h"
#include "system.h"

struct ACTOR;

// One block of the world map, an entry of WORLD_MAP_BLOCKS: the place it belongs to and its
// terrain.
typedef struct WORLD_MAP_BLOCK
{
	Uint8 unused[2]; // no code names it
	Uint8 place;     // METATILE_BEHAVIOR_WALL where the map has no block
	// the terrain kind and flags (fdWorldMapGetBattleEnvironment)
	Uint8 unusedAfterPlace;
} WORLD_MAP_BLOCK;

// The world map's blocks, a row of 32 for each z, in the metatile tiles buffer (MetatileTiles)
// while the world map is shown.
typedef struct WORLD_MAP_BLOCKS
{
	WORLD_MAP_BLOCK blocks[32 * 32];
} WORLD_MAP_BLOCKS;

// The world map area's four tile sets, decoded one after another into BgTileBuffer and copied to
// the two halves of BG char blocks 2 and 3.
#define WORLD_MAP_TILE_SET_BYTES 8192
#define WORLD_MAP_TILE_SET(n)    (RamBgTileBuffer + (n) * WORLD_MAP_TILE_SET_BYTES)
// The shape a scene gives the metatiles it marks, rewriting the shape's entry with their height in
// its second byte: Apojii's metatile markers and Aqua Rock's spouts.
#define MAP_SHAPE_MARKED         120
// The shape a scene gives the one metatile under a pillar it raises alone, its entry rewritten as
// MAP_SHAPE_MARKED's: Aqua Rock's water pillar.
#define MAP_SHAPE_RAISED_ALONE   121
// A map shape entry as MapGridShapes holds it: the shape's kind and its three heights, a byte each
// (the rows of a map's shapes.tsv).
#define MAP_SHAPE(shape, height0, height1, height2)                                                \
	((shape) | (height0) << 8 | (height1) << 16 | (height2) << 24)

// The map's animation each tick: its tiles, its blend scripts and the World Map's tints and tile
// blocks.
void               fdTilesetAnimationUpdate(void);
void               fdWorldMapUpdateRegionTint(Sint32 region, Sint32 gradual);
void               fdWorldMapPreloadCurrentTileBlock(void);
Sint32             fdScreenTransitionIsScanlineActiveStub(void);

// The world map's row drawer, its Thumb entry and ARM code (draw_map.s), copied to RAM to run.
extern const Uint8 fdRenderDrawMapCode[];
// The size of fdRenderDrawMapCode (draw_map.s).
extern Uint8       fdRenderDrawMapCodeSize[];

Sint32             fdMapShapeFindOrAddStub(Sint32 layer, Sint32 unused, Sint32 value);
Sint32             fdActorLiftTargetBlockStub(Sint32 layer, Sint32 unused, Sint32 value);

#endif
