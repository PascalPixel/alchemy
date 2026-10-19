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
  u16 metatileAndFlags;
  u8 collisionCode;
  u8 attributeB;
};

/// A map layer's scroll record, seen from its cells.
struct MapLayer {
  struct MapCell *cells;
  u8 unk04[0x2c];
};

/// The map's running blend script, which sets the blend registers over time.
struct BlendScriptState {
  u16 *script;
  u16 *cursor;
  u16 delay; ///< frames
  u16 paused;
};

/// One of the 16 tile animation channels, which copy tiles into VRAM by
/// script.
struct MapAnimation {
  const u16 *start;
  const u16 *cursor;
  u16 timer; ///< frames
  u16 paused;
};

/// The map work in heap slot 8 (struct MapScrollWork), seen as its tile
/// animation channels, blend script and layers' cells.
struct MapState {
  u8 unk000[0x14];
  u16 flags;
  u8 wideTiles;
  u8 unk17;
  struct MapAnimation anim[16];
  struct BlendScriptState blend;
  u8 unke4[0x1f];
  u8 blendControl;
  u8 unk104[0x2c];
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

extern u32 gMapBlocks[];

extern u8 gMapLayerData[];

extern u8 gMapCollision[];

extern u8 gBgTileBuffer[];

extern u8 *gMapAnimationPages;

#endif
