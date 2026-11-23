#ifndef ALCHEMY_FIELD_MAP_H
#define ALCHEMY_FIELD_MAP_H

/// @file
/// The map's buffers in EWRAM and its shape table, the world map's blocks, and
/// the map routines the scenes call.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/map_grid.h"

struct Actor;

/// One block of the world map, an entry of WorldMapBlocks: the place it belongs
/// to and its terrain.
struct WorldMapBlock {
  u8 unused[2];        ///< no code names it
  u8 place;            ///< CELL_BEHAVIOR_WALL where the map has no block
  u8 unusedAfterPlace; ///< the terrain kind and flags
                       ///< (WorldMap_GetBattleEnvironment)
};

/// The world map's blocks, a row of 32 for each z, in the cell picture buffer
/// (gCellPictures) while the world map is shown.
struct WorldMapBlocks {
  struct WorldMapBlock blocks[32 * 32];
};

struct Actor *Actor_FindAtCell(s32 *position, s32 check_y);

/// 32 KB of EWRAM holding background tiles, such as the world map's char blocks
/// 2 and 3.
extern u8 gBgTileBuffer[];
/// The world map area's four tile sets, decoded one after another into
/// gBgTileBuffer and copied to the two halves of BG char blocks 2 and 3.
#define WORLD_MAP_TILE_SET_BYTES 8192
#define WORLD_MAP_TILE_SET(n)                                                  \
  (Ram_BgTileBuffer + (n) * WORLD_MAP_TILE_SET_BYTES)
/// The map's shape table, 16 KB of EWRAM: 4-byte entries that a map cell's
/// shape indexes.
extern u8 gMapGridShapes[];
/// The shape a scene gives the cells it marks, rewriting the shape's entry with
/// their height in its second byte: Apojii's cell markers and Aqua Rock's
/// spouts.
#define MAP_SHAPE_MARKED 120
/// The shape a scene gives the one cell under a pillar it raises alone, its
/// entry rewritten as MAP_SHAPE_MARKED's: Aqua Rock's water pillar.
#define MAP_SHAPE_RAISED_ALONE 121
/// A map shape entry as gMapGridShapes holds it: the shape's kind and its
/// three heights, a byte each (the rows of a map's shapes.tsv).
#define MAP_SHAPE(shape, height0, height1, height2)                            \
  ((shape) | (height0) << 8 | (height1) << 16 | (height2) << 24)

void WorldMap_UpdateRegionTint(s32 region, s32 gradual);

s32 ScreenTransition_IsScanlineActiveFar(void);

/// 16 KB of EWRAM that pictures and tiles are decoded into before they are
/// copied on.
extern u8 gDecodeBuffer[];

void TilesetAnim_Update(void);

s32 MapCollision_FindOrAddTagFar(s32 layer, s32 unused, s32 value);

s32 Actor_LiftTargetBlockFar(s32 layer, s32 unused, s32 value);

/// The world map's row drawer, its Thumb entry and ARM code (draw_map.s),
/// copied to RAM to run.
extern const u8 Render_DrawMapCode[];
/// The size of Render_DrawMapCode (draw_map.s).
extern u8 Render_DrawMapCodeSize[];

s32 SpriteViewer_FindNextSpriteWithPictures(s32 spriteId, s32 direction);
void SpriteViewer_PlaceRing(void);

void BlendScript_Start(u16 *script);
s32 MapGrid_CheckCollisionAt(FIXED px, FIXED pz, s32 layer, s32 height);
void WorldMap_PreloadCurrentTileBlock(void);
void BlendScript_Resume(void);
void BlendScript_Suspend(void);

#endif
