#ifndef ALCHEMY_MAP_SCROLL_H
#define ALCHEMY_MAP_SCROLL_H

/// @file
/// The map layers' scroll work and the background scroll values.

#include "types.h"
#include "global_cells.h"
#include "static_assert.h"
#include "map.h"

/// One background layer of the field map: its scroll position, origin,
/// parallax against the view, drift and wrap, and its first map cell.
struct MapLayerScroll {
  s32 x, y;              ///< 16.16 fixed-point pixels
  s32 offsetX, offsetY;  ///< 16.16 fixed-point pixels
  s32 scaleX, scaleY;    ///< 16.16 fixed point, 0x10000 moves with the view
  s32 speedX, speedY;    ///< 16.16 fixed-point pixels per frame
  s32 phaseX, phaseY;    ///< 16.16 fixed-point pixels
  u16 maskX, maskY;      ///< a drifting layer's wrap mask, in 8-pixel tiles
  struct MapCell *cells; ///< the layer's first cell in the map cell buffer
};

/// The field map's work: the followed position, screen shake, tile
/// animations, blend script, view and its bounds, and three scrolling layers.
/// Heap slot 8 (HEAP_SLOT_MAP): Map_LoadLayeredScene takes and clears it as a
/// field scene with a layered map starts, and the next scene change frees it.
struct MapScrollWork {
  /// The followed point, read as x, height and z in 16.16 fixed-point pixels;
  /// Camera_FollowActor points it at the camera's position.
  s32 *origin;
  /// Screen shake amplitudes in 16.16 fixed-point pixels, and the 16.16
  /// factor that scales them each frame.
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;    ///< set from the layered scene's header; not read by name
  u16 flags;     ///< REG_DISPCNT bits: objects and the BG1 to BG3 shown
  u8 wideTiles;  ///< nonzero: tile animations copy 64-byte tiles
  u8 footprints; ///< nonzero: the leader leaves footprints
  struct MapAnimation anim[16];
  struct BlendScriptState blend;
  s32 viewX, viewY;           ///< the view's top left, 16.16 fixed-point pixels
  s32 minX, minY, maxX, maxY; ///< 16.16 fixed-point pixels
  /// Set while a BG1 animation runs; the tile animations are then neither
  /// suspended nor resumed.
  s8 animationActive;
  u8 unk0fd[3]; ///< not named by TBS code
  /// Each layer's BG priority, written into its BGCNT; zero hides the layer.
  u8 enabled[3];
  u8 unk103; ///< MapState's blendControl, the last BLDCNT the blend script set
  struct MapLayerScroll layers[3]; ///< BG3, BG2 and BG1, in that order
};

#include "perspective.h"

STATIC_ASSERT(sizeof(struct MapLayerScroll) == 0x30);
STATIC_ASSERT(offsetof(struct MapLayerScroll, cells) == 0x2c);
STATIC_ASSERT(sizeof(struct MapScrollWork) == 0x194);
STATIC_ASSERT(offsetof(struct MapScrollWork, anim) == 0x18);
STATIC_ASSERT(offsetof(struct MapScrollWork, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapScrollWork, viewX) == 0xe4);
STATIC_ASSERT(offsetof(struct MapScrollWork, animationActive) == 0xfc);
STATIC_ASSERT(offsetof(struct MapScrollWork, layers) == 0x104);

/// A background's hardware scroll offsets.
struct BgScroll {
  u16 x, y; ///< pixels
};

/// BG0 to BG3's scroll offsets, which the VBlank handler copies to
/// REG_BG0HOFS to REG_BG3VOFS in the VBlank after WaitFrames sets
/// gFrameRenderPending.
extern struct BgScroll gBgScroll[];

#endif
