#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

/// @file
/// Map grids: their cells, layers, animations and blend scripts, and the map
/// grid state.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/field.h"
#include "system/heap.h"

/// A map cell's cell behavior (`MapGridCell.behavior`): what stepping
/// on, pushing against or searching the cell does.
enum CellBehavior {
  CELL_BEHAVIOR_NORMAL = 0, ///< open ground: nothing happens there
  /// 1 to 239: the scene's own events, step trigger or exit when stepped on
  /// (Scene_RunStepTriggerOrExit), examine trigger when the player presses A
  /// facing the cell.
  CELL_BEHAVIOR_EVENT_FIRST = 1,
  CELL_BEHAVIOR_EVENT_LAST = 239,
  /// Ground with encounters of its own: grounds 1 and 2 of the map's zones.
  CELL_BEHAVIOR_ENCOUNTER_GROUND_A = 240,
  CELL_BEHAVIOR_ENCOUNTER_GROUND_B = 241,
  /// 242 to 247: examine triggers with lines of their own, the same on every
  /// map (Field_ExamineCell, gField_ExamineTriggerMessageOffsets).
  CELL_BEHAVIOR_JAR = 242,
  CELL_BEHAVIOR_BARREL = 243,
  CELL_BEHAVIOR_WOODEN_BOX = 244,
  CELL_BEHAVIOR_GRAVE = 245,
  CELL_BEHAVIOR_BOOKS = 246,
  CELL_BEHAVIOR_WELL = 247,
  /// Jumps forward like CELL_BEHAVIOR_JUMP_FORWARD, but Field_UpdateWalking
  /// never passes it to Field_RunPushCell.
  CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED = 249,
  /// Ground that hurts the party over time: the Lamakan Desert's sand.
  CELL_BEHAVIOR_DESERT_SAND = 250,
  /// Ground Cloak keeps the leader hidden on; stepping off it ends Cloak.
  CELL_BEHAVIOR_CLOAK = 251,
  /// 252 to 254: cells the leader pushes against (Field_RunPushCell).
  CELL_BEHAVIOR_LEDGE = 252,        ///< jumped down with Down held
  CELL_BEHAVIOR_CLIMB = 253,        ///< climbed up or down with Up or Down held
  CELL_BEHAVIOR_JUMP_FORWARD = 254, ///< jumped across
  CELL_BEHAVIOR_WALL = 255          ///< blocks whatever the heights
};

/// One cell of a map layer: its cell picture and flags, its cell behavior and
/// its shape, an index into the shape table.
struct MapGridCell {
  /// bits: 15..14 the priority an actor standing on it takes, if not 0
  /// (also read by MapGrid_GetCellHighFlags) | 13..12 the layer such an
  /// actor takes, + 1, if not 0 | 11..0 cell picture (struct ActorCameraTile)
  u16 cellPictureAndFlags;
  u8 behavior; ///< an enum CellBehavior
  /// Indexes the 4-byte shape table (gMapGridShapes): the shape's kind in
  /// the first byte's low four bits, then its height samples.
  u8 shape;
};

/// The cell picture number in a map cell's low 12 bits.
#define MAPGRID_CELL_PICTURE_ID_MASK 0x00000fff

/// The map's running blend script, which sets the blend registers over time
/// (BlendScript_Run, each frame from BlendScript_Start).
struct BlendScriptState {
  u16 *script; ///< where 0xffff loops back to; null for none
  u16 *cursor; ///< the next command
  u16 delay;   ///< frames left before the next command
  u16 paused;  ///< nonzero holds the script where it is
};

/// One of the 16 tileset animations, which copy tiles into VRAM by
/// script (TilesetAnim_Update): groups of four halfwords, the source tile,
/// the count, the destination tile and the frames to wait.
struct TilesetAnim {
  const u16 *start;  ///< where 0xffff loops back to; null for none
  const u16 *cursor; ///< the next group
  u16 timer;         ///< frames left of the current wait
  u16 paused;        ///< nonzero, set by a scene, holds the animation
};

/// A layout's three layers, by what they hold. Most layouts give them
/// priorities 3, 2 and 1.
enum MapGridLayerIndex {
  MAPGRID_LAYER_GROUND = 0,   ///< `BG3`: the ground, at the back
  MAPGRID_LAYER_MIDDLE = 1,   ///< `BG2`: walls and edges, under the actors
  MAPGRID_LAYER_OVERHEAD = 2, ///< `BG1`: roofs and tree tops, over the actors
  MAPGRID_LAYER_COUNT = 3
};

STATIC_ASSERT(sizeof(struct TilesetAnim) == 12);

/// A position on the world map, as its walkability check reads it.
struct WorldPosition {
  FIXED x;      ///< 16.16 fixed-point pixels
  FIXED height; ///< 16.16 fixed-point pixels
  FIXED y;      ///< 16.16 fixed-point pixels
};

/// The layout's cell pictures, 32 KB of EWRAM at 0x02020000: each cell picture
/// 8 bytes, its four tile entries. MapGrid_LoadMap decodes them in.
extern u32 gCellPictures[];
/// The World Map's 32-pixel blocks, 32 to a row of gCellPictures, wrapping
/// round: a 16.16 coordinate shifted down this far is a block.
#define WORLD_MAP_BLOCK_SHIFT 21
#define WORLD_MAP_BLOCK_PIXELS 32
#define WORLD_MAP_BLOCKS_ACROSS 32

/// The tileset animations' tiles, 16 KB of EWRAM at 0x02028000.
extern u8 gMapGridAnimTiles[];

/// The map's shape table, 16 KB of EWRAM at 0x0202c000: 4-byte entries
/// that MapGridCell.shape indexes.
extern u8 gMapGridShapes[];

/// 32 KB of EWRAM at 0x02038000 holding background tiles, such as the world
/// map's char blocks 2 and 3, which WorldMap_ShowBg1FromBuffer copies back
/// after the still image.
extern u8 gBgTileBuffer[];

/// The map grid: 128 by 128 cells, each 16 pixels square, wrapping at its
/// edges.
#define MAPGRID_SIZE 128
#define MAPGRID_CELL_PIXELS 16
/// The map grid's cells, row by row, in Ram_MapGridCellBuffer
/// (system/ram_buffer.h): the cell at column x, row y is
/// MAPGRID_CELLS[y * MAPGRID_SIZE + x].
#define MAPGRID_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)
/// A whole pixel coordinate's map cell, in pixels.
#define MAPGRID_CELL_PIXEL_MASK 0xfff0
/// The low four bits of a shape's first byte: its kind, which height
/// function reads its samples (gMapGrid_TerrainHeightFunctions).
#define MAPGRID_SHAPE_KIND_MASK 0xf

/// HEAP_SLOT_WORLD_MAP_AFFINE under its own name, which no code reads; the
/// world map keeps its struct WorldMapAffineWork in the slot.
#define gWorldMapAffineWork (*(u8 **)&gHeapSlots[HEAP_SLOT_WORLD_MAP_AFFINE])

/// A row of gMaps: a layout's resources, each counted from ResourceId_EmptyMap.
struct MapResources {
  u16 header;
  u16 palette;
  u16 tiles[3];       ///< For char blocks 1, 2 and 3.
  u16 animationTiles; ///< Into gMapGridAnimTiles.
};
/// Every layout's resources, by map number (src/map/resources.c).
extern const struct MapResources gMaps[];

#endif
