#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

/// @file
/// The world map's perspective view state.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/// The world map's perspective view, in the map's heap slot: the followed
/// position, screen shake, view position, window, pitch and yaw, a 32 by 32
/// grid of tile ids and zoom, then the still image's state and the region
/// tint.
struct WorldMapView {
  s32 *origin;                    ///< the followed point, as in MapState
  s32 shakeX, shakeY, shakeDecay; ///< the screen shake, as in MapState
  u8 *script;                     ///< NULL at load
  u16 fade; ///< MapState's flags: REG_DISPCNT bits, BG0 to BG3 and the sprites
            ///< at load
  u8 fadeStep; ///< MapState's wideTiles, 0x80 at load
  /// As MapState: footprints, tileset animations and blend script.
  u8 animationsAndBlend[205];
  FIXED viewX;             ///< the view's left, 16.16 fixed-point pixels
  FIXED viewY;             ///< the view's top, 16.16 fixed-point pixels
  FIXED scaleX;            ///< 32 in 16.16 at load
  FIXED scaleY;            ///< 64 in 16.16 at load
  FIXED limitX;            ///< 16352 in 16.16 at load
  FIXED limitY;            ///< 16352 in 16.16 at load
  u8 unusedAfterLimitY[4]; ///< no code names it
  /// Set by WorldMap_StartStillImage while BG1 shows the drawn map; the
  /// tileset animations are then neither suspended nor resumed.
  s8 animationActive;
  u8 unused[3];
  /// Screen rows, 0 and 159 at load and 200 and 255 from
  /// WorldMap_StartStillImage; each frame copies them to shownTop and
  /// shownBottom and picks BG mode 2 or 0 from them.
  ///@{
  u16 windowTop;
  u16 windowBottom;
  ///@}
  u16 shownTop;      ///< windowTop as the frame update last applied it
  u16 shownBottom;   ///< windowBottom as the frame update last applied it
  u16 windowPending; ///< cleared by the frame update; not set by name
  u8 unusedAfterWindow[6];
  /// The world map's tile offsets (ResourceId_PerspectiveDataA), each tile
  /// LZ-compressed.
  void *tiles;
  u8 unusedAfterTiles[4];
  u16 pitch; ///< 1/65536 turns
  u16 yaw;   ///< 1/65536 turns
  /// A row of gWorldMap_GraphicsResources, by the region the party is in.
  const u32 *resources;
  u8 unusedAfterResources[36];
  u16 tileIds[32 * 32]; ///< the world map tile in each of 32 by 32 cells
  /// The tile each quarter of each layer's block buffer shows, so an unchanged
  /// quarter is not decoded again.
  u16 quarterTiles[8];
  /// Steps left of reloading the area's graphics after the region changed,
  /// one a frame from 12 (WorldMap_PreloadCurrentTileBlock).
  s32 reloadSteps;
  s32 farPlane;             ///< 510 pixels at load
  s32 distance;             ///< farPlane scaled by the zoom
  s32 previousZoom;         ///< the zoom a transition starts from
  FIXED zoom;               ///< 16.16 fixed point, FIXED_ONE full size
  u16 zoomTransitionFrames; ///< a zoom transition's length; 0 when none
  s16 zoomTransitionStep;   ///< frames of the transition done
  /// Set when a field Psynergy cast on the World Map has used the second half
  /// of the BG tile buffer; the still image then decodes it again.
  s8 tileBufferUsed;
  /// Set while BG1 shows the still image: the view then only shakes BG1.
  s8 stillImageShown;
  /// Set while the view is held for the still image or the map screen:
  /// no tile is preloaded, and without a shake the frame does nothing.
  s8 viewHeld;
  s8 tintRegion; ///< the encounter region the tint was chosen for
  s8 tint;       ///< brightness steps, darker when negative
  s8 tintTarget; ///< the tint the region asks for
  s8 fixedEye;   ///< set: the frame update leaves the eye where it is
};

STATIC_ASSERT(offsetof(struct WorldMapView, viewX) == 228);
STATIC_ASSERT(offsetof(struct WorldMapView, animationActive) == 256);
STATIC_ASSERT(offsetof(struct WorldMapView, windowTop) == 260);
STATIC_ASSERT(offsetof(struct WorldMapView, tiles) == 276);
STATIC_ASSERT(offsetof(struct WorldMapView, pitch) == 284);
STATIC_ASSERT(offsetof(struct WorldMapView, resources) == 288);
STATIC_ASSERT(offsetof(struct WorldMapView, tileIds) == 328);
STATIC_ASSERT(offsetof(struct WorldMapView, reloadSteps) == 2392);
STATIC_ASSERT(offsetof(struct WorldMapView, farPlane) == 2396);
STATIC_ASSERT(offsetof(struct WorldMapView, zoom) == 2408);
STATIC_ASSERT(offsetof(struct WorldMapView, tileBufferUsed) == 2416);
STATIC_ASSERT(offsetof(struct WorldMapView, tintRegion) == 2419);
STATIC_ASSERT(offsetof(struct WorldMapView, fixedEye) == 2422);
STATIC_ASSERT(sizeof(struct WorldMapView) == 2424);

#endif
