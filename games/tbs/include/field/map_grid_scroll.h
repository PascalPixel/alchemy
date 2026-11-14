#ifndef ALCHEMY_MAP_GRID_SCROLL_H
#define ALCHEMY_MAP_GRID_SCROLL_H

/// @file
/// The map layers' scroll work and the background scroll values.

#include "static_assert.h"
#include "types.h"
#include "field/map_grid.h"
#include "system/heap_slots.h"

/// One background layer of the field map: its scroll position, origin,
/// parallax against the view, drift and wrap, and its first map cell.
struct MapGridLayerScroll {
  s32 x, y;                  ///< 16.16 fixed-point pixels
  s32 offsetX, offsetY;      ///< 16.16 fixed-point pixels
  s32 scaleX, scaleY;        ///< 16.16 fixed point, Q16_ONE moves with the view
  s32 speedX, speedY;        ///< 16.16 fixed-point pixels per frame
  s32 phaseX, phaseY;        ///< 16.16 fixed-point pixels
  u16 maskX, maskY;          ///< a drifting layer's wrap mask, in 8-pixel tiles
  struct MapGridCell *cells; ///< the layer's first cell in the map cell buffer
};

/// The field map's work: the followed position, screen shake, tileset
/// animations, blend script, view and its bounds, and three scrolling layers.
struct MapGridScrollWork {
  /// The followed point, read as x, height and z in 16.16 fixed-point pixels;
  /// FieldCamera_FollowActor points it at the camera's position.
  s32 *origin;
  /// Screen shake amplitudes in 16.16 fixed-point pixels, and the 16.16
  /// factor that scales them each frame.
  s32 shakeX, shakeY, shakeDecay;
  u8 *markers;   ///< the layout's treasure spots, for Field_PlaceTreasureSpots
  u16 flags;     ///< REG_DISPCNT bits: sprites and the BG1 to BG3 shown
  u8 wideTiles;  ///< nonzero: tileset animations copy 64-byte tiles
  u8 footprints; ///< nonzero: the leader leaves footprints
  struct TilesetAnim animations[16];
  struct BlendScriptState blend;
  s32 view[2]; ///< the view's top left, x then y, 16.16 fixed-point pixels
  s32 minX, minY, maxX, maxY; ///< 16.16 fixed-point pixels
  /// Set while a BG1 animation runs; the tileset animations are then neither
  /// suspended nor resumed.
  s8 animationActive;
  u8 unused[3];
  /// Each layer's BG priority, written into its BGCNT; zero hides the layer.
  u8 enabled[3];
  /// The last BLDCNT the blend script set; with mode 1 in bits 7..6 its values
  /// go to BLDALPHA, else to BLDY.
  u8 blendControl;
  struct MapGridLayerScroll layers[3]; ///< BG3, BG2 and BG1, in that order
};

#include "field/perspective.h"

STATIC_ASSERT(sizeof(struct MapGridLayerScroll) == 0x30);
STATIC_ASSERT(offsetof(struct MapGridLayerScroll, cells) == 0x2c);
STATIC_ASSERT(sizeof(struct MapGridScrollWork) == 0x194);
STATIC_ASSERT(offsetof(struct MapGridScrollWork, animations) == 0x18);
STATIC_ASSERT(offsetof(struct MapGridScrollWork, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapGridScrollWork, view) == 0xe4);
STATIC_ASSERT(offsetof(struct MapGridScrollWork, animationActive) == 0xfc);
STATIC_ASSERT(offsetof(struct MapGridScrollWork, layers) == 0x104);

/// A background's hardware scroll offsets.
struct BgScroll {
  u16 x, y; ///< pixels
};

/// BG0 to BG3's scroll offsets, which the VBlank handler copies to
/// REG_BG0HOFS to REG_BG3VOFS in the VBlank after wait() sets
/// gFrameRenderPending.
extern struct BgScroll gBgScroll[];

#endif
