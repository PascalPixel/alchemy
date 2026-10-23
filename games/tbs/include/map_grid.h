#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

/// @file
/// Maps: their cells, layers, animations and blend scripts, and the map state.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

/// A map cell's collision code (`MapGridCell.collisionCode`): what stepping on,
/// pushing against or searching the cell does. 0 is open ground.
enum MetatileBehavior {
  /// 1 to 239: the scene's own events, touch or exit when stepped on
  /// (Map_RunCoordEventOrWarp), search when the player presses A facing the
  /// cell.
  MB_EVENT_FIRST = 1,
  MB_EVENT_LAST = 239,
  /// Ground with encounters of its own: grounds 1 and 2 of the scene's zones.
  MB_ENCOUNTER_GROUND_1 = 240,
  MB_ENCOUNTER_GROUND_2 = 241,
  /// 242 to 247: search spots with lines of their own, the same on every map
  /// (Field_SearchTile, gField_TileMessageOffsets).
  MB_JAR = 242,
  MB_BARREL = 243,
  MB_WOODEN_BOX = 244,
  MB_GRAVE = 245,
  MB_BOOKS = 246,
  MB_WELL = 247,
  /// Jumps forward like MB_JUMP_FORWARD, but Field_ProcessStep never
  /// passes it to Field_RunPushTileEvent.
  MB_JUMP_FORWARD_UNREACHED = 249,
  /// Ground that hurts the party over time: the Lamakan Desert's sand.
  MB_DESERT_SAND = 250,
  /// 252 to 254: cells the leader pushes against (Field_RunPushTileEvent).
  MB_LEDGE = 252,        ///< jumped down with Down held
  MB_CLIMB = 253,        ///< climbed up or down with Up or Down held
  MB_JUMP_FORWARD = 254, ///< jumped across
  MB_WALL = 255          ///< blocks whatever the heights
};

/// One cell of a map layer: its metatile and flags, its collision code and an
/// attribute indexing the collision table.
struct MapGridCell {
  /// bits: 15..12 flags (15..14 read by MapGrid_GetCellHighFlags) | 11..0
  /// metatile
  u16 metatileAndFlags;
  u8 collisionCode; ///< an enum MetatileBehavior
  /// Indexes the 4-byte collision entries (gMapGridCollision): terrain kind in
  /// the first byte's low four bits, then its height samples.
  u8 attributeB;
};

/// A map layer's scroll record, seen from its cells.
struct MapGridLayer {
  struct MapGridCell
      *cells;     ///< the layer's first cell (MapGridLayerScroll's cells)
  u8 unk04[0x2c]; ///< the next layer's scroll values; not named by TBS code
};

/// The map's running blend script, which sets the blend registers over time
/// (DisplayBlend_RunScript, each frame from DisplayBlend_StartScript).
struct BlendScriptState {
  u16 *script; ///< where 0xffff loops back to; null for none
  u16 *cursor; ///< the next command
  u16 delay;   ///< frames left before the next command
  u16 paused;  ///< nonzero holds the script where it is
};

/// One of the 16 tile animation channels, which copy tiles into VRAM by
/// script (TileAnimation_Update): groups of four halfwords, the source tile,
/// the count, the destination tile and the frames to wait.
struct TileAnimation {
  const u16 *start;  ///< where 0xffff loops back to; null for none
  const u16 *cursor; ///< the next group
  u16 timer;         ///< frames left of the current wait
  u16 paused;        ///< nonzero, set by a scene, holds the channel
};

/// A layered map's three layers, by what they hold. Most maps give them
/// priorities 3, 2 and 1, and the field draws sprites at priority 2, so the
/// ground is at the back, the middle layer under the actors and the overhead
/// layer over them. Each shows on its own background: `BG3`, `BG2` and `BG1`.
enum MapGridLayerIndex {
  MAPGRID_LAYER_GROUND = 0,   ///< `BG3`: the ground, at the back
  MAPGRID_LAYER_MIDDLE = 1,   ///< `BG2`: walls and edges, under the actors
  MAPGRID_LAYER_OVERHEAD = 2, ///< `BG1`: roofs and tree tops, over the actors
  MAPGRID_LAYER_COUNT = 3
};

/// The map work in HEAP_SLOT_MAP_GRID (struct MapGridScrollWork), seen as its
/// tile animation channels, blend script and layers' cells.
struct MapGridState {
  u8 unk000[0x14]; ///< MapGridScrollWork's origin, shake and script
  u16 flags;       ///< REG_DISPCNT bits; MapGrid_SetShownLayers sets BG1 to BG3
  u8 wideTiles;    ///< nonzero: tile animations copy 64-byte tiles
  u8 unk17;        ///< MapGridScrollWork's footprints; Bilibin Cave sets it
  struct TileAnimation animations[16];
  struct BlendScriptState blend;
  u8 unke4[0x1f]; ///< MapGridScrollWork's view, bounds and shown layers
  /// The last BLDCNT the blend script wrote; with mode 1 in bits 7..6 its
  /// values go to BLDALPHA, else to BLDY.
  u8 blendControl;
  u8 unk104[0x2c]; ///< the first layer's scroll values; not named by TBS code
  /// Each layer's cells, at MapGridScrollWork's layers[i].cells; readers mask
  /// the layer with 3, and the fourth entry lies past MapGridScrollWork's end.
  struct MapGridLayer layers[4];
};

STATIC_ASSERT(sizeof(struct TileAnimation) == 12);
STATIC_ASSERT(offsetof(struct MapGridState, flags) == 0x14);
STATIC_ASSERT(offsetof(struct MapGridState, wideTiles) == 0x16);
STATIC_ASSERT(offsetof(struct MapGridState, animations) == 0x18);
STATIC_ASSERT(offsetof(struct MapGridState, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapGridState, layers) == 0x130);

/// A position on the world map, as its walkability check reads it.
struct WorldPosition {
  s32 x;      ///< 16.16 fixed-point pixels
  s32 height; ///< 16.16 fixed-point pixels
  s32 y;      ///< 16.16 fixed-point pixels
};

/// The map's blocks, 32 KB of EWRAM at 0x02020000: each block 8 bytes, its
/// four tile entries. MapGrid_LoadLayout decodes them in.
extern u32 gMetatiles[];

/// The map's layer data, 16 KB of EWRAM at 0x02028000. A tile animation's
/// source tiles numbered 0x600 and up (0x200 and up for 64-byte tiles) are
/// read from here, tile 0x600 (0x200) at its first byte.
extern u8 gMapGridLayerData[];

/// The map's collision shapes, 16 KB of EWRAM at 0x0202c000: 4-byte entries
/// that MapGridCell.attributeB indexes.
extern u8 gMapGridCollision[];

/// 32 KB of EWRAM at 0x02038000 holding background tiles, such as the world
/// map's char blocks 2 and 3, which WorldMap_ShowBg1FromBuffer copies back
/// after the still view. The EWRAM pool's limit, 0x02040000, lies past it, so
/// the pool's blocks can reach these bytes.
extern u8 gBgTileBuffer[];

/// HEAP_SLOT_WORLD_MAP_FRAME under its own name, which no code reads; the world
/// map keeps its struct WorldMapFrameWork in the slot.
extern u8 *gTileAnimationPages;

#endif
