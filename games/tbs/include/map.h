#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

/// @file
/// Maps: their cells, layers, animations and blend scripts, and the map state.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

/// One cell of a map layer: its metatile and flags, its collision code and an
/// attribute indexing the collision table.
struct MapCell {
  /// bits: 15..12 flags (15..14 read by Map_GetCellHighFlags) | 11..0 metatile
  u16 metatileAndFlags;
  /// The step code: 1 to 239 scene events, 240 and 241 encounter ground, 250
  /// hazard, 252 to 254 for Field_RunPushTileEvent, 0xff a wall.
  u8 collisionCode;
  /// Indexes the 4-byte collision entries (gMapCollision): terrain kind in the
  /// first byte's low four bits, then its height samples.
  u8 attributeB;
};

/// A map layer's scroll record, seen from its cells.
struct MapLayer {
  struct MapCell *cells; ///< the layer's first cell (MapLayerScroll's cells)
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
/// script (MapAnimation_Update): groups of four halfwords, the source tile,
/// the count, the destination tile and the frames to wait.
struct MapAnimation {
  const u16 *start;  ///< where 0xffff loops back to; null for none
  const u16 *cursor; ///< the next group
  u16 timer;         ///< frames left of the current wait
  u16 paused;        ///< nonzero, set by a scene, holds the channel
};

/// The map work in heap slot 8 (struct MapScrollWork), seen as its tile
/// animation channels, blend script and layers' cells.
struct MapState {
  u8 unk000[0x14]; ///< MapScrollWork's origin, shake and script
  u16 flags;       ///< REG_DISPCNT bits; Map_SetShownLayers sets BG1 to BG3
  u8 wideTiles;    ///< nonzero: tile animations copy 64-byte tiles
  u8 unk17;        ///< MapScrollWork's footprints; Bilibin Cave sets it
  struct MapAnimation anim[16];
  struct BlendScriptState blend;
  u8 unke4[0x1f]; ///< MapScrollWork's view, bounds and shown layers
  /// The last BLDCNT the blend script wrote; with mode 1 in bits 7..6 its
  /// values go to BLDALPHA, else to BLDY.
  u8 blendControl;
  u8 unk104[0x2c]; ///< the first layer's scroll values; not named by TBS code
  /// Each layer's cells, at MapScrollWork's layers[i].cells; readers mask the
  /// layer with 3, and the fourth entry lies past MapScrollWork's end.
  struct MapLayer layers[4];
};

STATIC_ASSERT(sizeof(struct MapAnimation) == 12);
STATIC_ASSERT(offsetof(struct MapState, flags) == 0x14);
STATIC_ASSERT(offsetof(struct MapState, wideTiles) == 0x16);
STATIC_ASSERT(offsetof(struct MapState, anim) == 0x18);
STATIC_ASSERT(offsetof(struct MapState, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapState, layers) == 0x130);

/// A position on the world map, as its walkability check reads it.
struct WorldPosition {
  s32 x;      ///< 16.16 fixed-point pixels
  s32 height; ///< 16.16 fixed-point pixels
  s32 y;      ///< 16.16 fixed-point pixels
};

/// The map's blocks, 32 KB of EWRAM at 0x02020000: each block 8 bytes, its
/// four tile entries. Map_LoadLayeredScene decodes them in.
extern u32 gMapBlocks[];

/// The map's layer data, 16 KB of EWRAM at 0x02028000. A tile animation's
/// source tiles numbered 0x600 and up (0x200 and up for 64-byte tiles) are
/// read from here, tile 0x600 (0x200) at its first byte.
extern u8 gMapLayerData[];

/// The map's collision shapes, 16 KB of EWRAM at 0x0202c000: 4-byte entries
/// that MapCell.attributeB indexes.
extern u8 gMapCollision[];

/// 32 KB of EWRAM at 0x02038000 holding background tiles, such as the world
/// map's char blocks 2 and 3, which Map_ShowBg1FromBuffer copies back after
/// the still view. The EWRAM pool's limit, 0x02040000, lies past it, so the
/// pool's blocks can reach these bytes.
extern u8 gBgTileBuffer[];

/// Heap slot 7 under its own name, which no code reads; the world map keeps
/// its struct MapFrameWork in the slot.
extern u8 *gMapAnimationPages;

#endif
