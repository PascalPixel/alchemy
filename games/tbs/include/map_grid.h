#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

/// @file
/// Map grids: their cells, layers, animations and blend scripts, and the map
/// grid state.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

/// A map cell's metatile behavior (`MapGridCell.behavior`): what stepping
/// on, pushing against or searching the cell does.
enum MetatileBehavior {
  MB_NORMAL = 0, ///< open ground: nothing happens there
  /// 1 to 239: the map's own events, coord event or warp when stepped on
  /// (Map_RunCoordEventOrWarp), bg event when the player presses A facing the
  /// cell.
  MB_EVENT_FIRST = 1,
  MB_EVENT_LAST = 239,
  /// Ground with encounters of its own: grounds 1 and 2 of the map's zones.
  MB_ENCOUNTER_GROUND_1 = 240,
  MB_ENCOUNTER_GROUND_2 = 241,
  /// 242 to 247: bg events with lines of their own, the same on every map
  /// (Field_SearchCell, gField_BgEventMessageOffsets).
  MB_JAR = 242,
  MB_BARREL = 243,
  MB_WOODEN_BOX = 244,
  MB_GRAVE = 245,
  MB_BOOKS = 246,
  MB_WELL = 247,
  /// Jumps forward like MB_JUMP_FORWARD, but Field_ProcessStep never
  /// passes it to Field_RunPushCellEvent.
  MB_JUMP_FORWARD_UNREACHED = 249,
  /// Ground that hurts the party over time: the Lamakan Desert's sand.
  MB_DESERT_SAND = 250,
  /// 252 to 254: cells the leader pushes against (Field_RunPushCellEvent).
  MB_LEDGE = 252,        ///< jumped down with Down held
  MB_CLIMB = 253,        ///< climbed up or down with Up or Down held
  MB_JUMP_FORWARD = 254, ///< jumped across
  MB_WALL = 255          ///< blocks whatever the heights
};

/// One cell of a map layer: its metatile and flags, its metatile behavior and
/// its shape, an index into the shape table.
struct MapGridCell {
  // XXX (no evidence): what the four flag bits mark; the grids set them, but
  // no code reads them, so only Camelot's map tools could say.
  /// bits: 15..12 flags (15..14 read by MapGrid_GetCellHighFlags) | 11..0
  /// metatile
  u16 metatileAndFlags;
  u8 behavior; ///< an enum MetatileBehavior
  /// Indexes the 4-byte shape table (gMapGridShapes): the shape's kind in
  /// the first byte's low four bits, then its height samples.
  u8 shape;
};

/// A map layer's scroll state, seen from its cells.
struct MapGridLayer {
  struct MapGridCell
      *cells; ///< the layer's first cell (MapGridLayerScroll's cells)
  /// The next layer's scroll values (MapGridLayerScroll up to its cells); not
  /// named through this view.
  u8 nextScroll[0x2c];
};

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
  u16 paused;        ///< nonzero, set by a map, holds the animation
};

/// A layout's three layers, by what they hold. Most layouts give them
/// priorities 3, 2 and 1. An object event takes its sprite's priority from the
/// cell under it (OBJ_EVENT_PRIORITY_AUTOMATIC, ObjectSystem_PlaceSprites):
/// 2 on open ground, which puts it over the middle layer and under the overhead
/// one, and 3 where the cells put it behind a building. Each layer shows on its
/// own background: `BG3`, `BG2` and `BG1`.
enum MapGridLayerIndex {
  MAPGRID_LAYER_GROUND = 0, ///< `BG3`: the ground, at the back
  MAPGRID_LAYER_MIDDLE = 1, ///< `BG2`: walls and edges, under the object events
  MAPGRID_LAYER_OVERHEAD =
      2, ///< `BG1`: roofs and tree tops, over the object events
  MAPGRID_LAYER_COUNT = 3
};

/// The map work in HEAP_SLOT_MAP_GRID (struct MapGridScrollWork), seen as its
/// tileset animations, blend script and layers' cells.
struct MapGridScrollAnimView {
  /// MapGridScrollWork's origin, shake and script; not named through this view.
  ///@{
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  ///@}
  u16 flags;     ///< REG_DISPCNT bits; MapGrid_SetShownLayers sets BG1 to BG3
  u8 wideTiles;  ///< nonzero: tileset animations copy 64-byte tiles
  u8 footprints; ///< MapGridScrollWork's; Bilibin Cave sets it
  struct TilesetAnim animations[16];
  struct BlendScriptState blend;
  /// MapGridScrollWork's view, bounds and shown layers; not named through this
  /// view.
  ///@{
  s32 viewX, viewY;
  s32 minX, minY, maxX, maxY;
  s8 animationActive;
  u8 unused[3];
  u8 enabled[3];
  ///@}
  /// The last BLDCNT the blend script wrote; with mode 1 in bits 7..6 its
  /// values go to BLDALPHA, else to BLDY.
  u8 blendControl;
  /// The first layer's scroll values (MapGridLayerScroll up to its cells); not
  /// named through this view.
  u8 firstScroll[0x2c];
  /// Each layer's cells, at MapGridScrollWork's layers[i].cells; readers mask
  /// the layer with 3, and the fourth entry lies past MapGridScrollWork's end.
  struct MapGridLayer layers[4];
};

STATIC_ASSERT(sizeof(struct TilesetAnim) == 12);
STATIC_ASSERT(offsetof(struct MapGridScrollAnimView, flags) == 0x14);
STATIC_ASSERT(offsetof(struct MapGridScrollAnimView, wideTiles) == 0x16);
STATIC_ASSERT(offsetof(struct MapGridScrollAnimView, animations) == 0x18);
STATIC_ASSERT(offsetof(struct MapGridScrollAnimView, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapGridScrollAnimView, layers) == 0x130);

/// A position on the world map, as its walkability check reads it.
struct WorldPosition {
  s32 x;      ///< 16.16 fixed-point pixels
  s32 height; ///< 16.16 fixed-point pixels
  s32 y;      ///< 16.16 fixed-point pixels
};

/// The layout's metatiles, 32 KB of EWRAM at 0x02020000: each metatile 8 bytes,
/// its four tile entries. MapGrid_LoadLayout decodes them in.
extern u32 gMetatiles[];

/// The tileset animations' tiles, 16 KB of EWRAM at 0x02028000. A tileset
/// animation's source tiles numbered 0x600 and up (0x200 and up for 64-byte
/// tiles) are read from here, tile 0x600 (0x200) at its first byte.
extern u8 gMapGridAnimTiles[];

/// The map's shape table, 16 KB of EWRAM at 0x0202c000: 4-byte entries
/// that MapGridCell.shape indexes.
extern u8 gMapGridShapes[];

/// 32 KB of EWRAM at 0x02038000 holding background tiles, such as the world
/// map's char blocks 2 and 3, which WorldMap_ShowBg1FromBuffer copies back
/// after the still view. The EWRAM pool's limit, 0x02040000, lies past it, so
/// the pool's blocks can reach these bytes.
extern u8 gBgTileBuffer[];

/// HEAP_SLOT_WORLD_MAP_FRAME under its own name, which no code reads; the world
/// map keeps its struct WorldMapFrameWork in the slot.
extern u8 *gWorldMapFrameWork;

/// A row of gLayouts: a layout's resources, counted from
/// ResourceId_Layout001: its header, palette, three tilesets and the tileset
/// animations' tiles.
struct LayoutResources {
  /// Offsets from ResourceId_Layout001: the header, the palette, the
  /// tilesets for char blocks 1, 2 and 3, and the tileset animations' tiles
  /// (gMapGridAnimTiles).
  u16 resources[6];
};

#endif
