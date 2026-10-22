#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

/// @file
/// The world map's perspective view work.

#include "types.h"
#include "static_assert.h"

/// The world map's perspective view: the followed position, screen shake,
/// view position, window, pitch and yaw, tile ids and zoom. Heap slot 8.
struct PerspectiveWork {
  s32 *origin;                    ///< the followed point, as in MapScrollWork
  s32 shakeX, shakeY, shakeDecay; ///< the screen shake, as in MapScrollWork
  u8 *script;                     ///< NULL at load
  u16 fade;    ///< MapScrollWork's flags: REG_DISPCNT bits, 0x1f00 at load
  u8 fadeStep; ///< MapScrollWork's wideTiles, 0x80 at load
  u8 unk017[0xe4 - 0x17]; ///< as MapScrollWork: tile animations, blend script
  s32 viewX;              ///< 16.16 fixed-point pixels
  s32 viewY;              ///< 16.16 fixed-point pixels
  s32 scaleX;             ///< 0x200000 at load
  s32 scaleY;             ///< 0x400000 at load
  s32 limitX;             ///< 0x1fe00000 at load
  s32 limitY;             ///< 0x1fe00000 at load
  /// Set by WorldMap_StartStillView while BG1 shows the drawn map; the tile
  /// animations are then neither suspended nor resumed.
  s8 animationActive;
  u8 unk0fd[3]; ///< not named by TBS code
  /// Screen rows, 0 and 159 at load and 200 and 255 from
  /// WorldMap_StartStillView; each frame copies them to shownTop and
  /// shownBottom and picks BG mode 2 or 0 from them.
  ///@{
  u16 windowTop;
  u16 windowBottom;
  ///@}
  u16 shownTop;      ///< windowTop as the frame update last applied it
  u16 shownBottom;   ///< windowBottom as the frame update last applied it
  u16 windowPending; ///< cleared by the frame update; not set by name
  u8 unk10a[6];      ///< not named by TBS code
  /// The world map's tile offsets (ResourceId_PerspectiveDataA), each tile
  /// LZ-compressed.
  void *tiles;
  u8 unk114[4];     ///< not named by TBS code
  u16 pitch;        ///< 1/65536 turns
  u16 yaw;          ///< 1/65536 turns
  u32 *resources;   ///< a row of gWorldMap_GraphicsResources, by terrain
  u8 unk120[0x18];  ///< not named by TBS code
  u16 tileIds[256]; ///< the world map tile in each of 16 by 16 cells
  /// The tile each quarter of each layer's block buffer shows, so an unchanged
  /// quarter is not decoded again.
  u16 quarterTiles[8];
  s32 farPlane;             ///< 0x1fe0000 at load
  s32 distance;             ///< farPlane scaled by the zoom
  s32 previousZoom;         ///< the zoom a transition starts from
  s32 zoom;                 ///< 16.16 fixed point, 0x10000 full size
  u16 zoomTransitionFrames; ///< a zoom transition's length; 0 when none
  s16 zoomTransitionStep;   ///< frames of the transition done
};

STATIC_ASSERT(sizeof(struct PerspectiveWork) == 0x35c);
STATIC_ASSERT(offsetof(struct PerspectiveWork, viewX) == 0xe4);
STATIC_ASSERT(offsetof(struct PerspectiveWork, pitch) == 0x118);
STATIC_ASSERT(offsetof(struct PerspectiveWork, animationActive) == 0xfc);
STATIC_ASSERT(offsetof(struct PerspectiveWork, resources) == 0x11c);
STATIC_ASSERT(offsetof(struct PerspectiveWork, tileIds) == 0x138);
STATIC_ASSERT(offsetof(struct PerspectiveWork, quarterTiles) == 0x338);
STATIC_ASSERT(offsetof(struct PerspectiveWork, farPlane) == 0x348);
STATIC_ASSERT(offsetof(struct PerspectiveWork, zoom) == 0x354);
STATIC_ASSERT(offsetof(struct PerspectiveWork, previousZoom) == 0x350);
STATIC_ASSERT(offsetof(struct PerspectiveWork, zoomTransitionFrames) == 0x358);
STATIC_ASSERT(offsetof(struct PerspectiveWork, zoomTransitionStep) == 0x35a);

#endif
