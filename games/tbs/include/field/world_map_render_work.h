#ifndef ALCHEMY_WORLD_MAP_RENDER_WORK_H
#define ALCHEMY_WORLD_MAP_RENDER_WORK_H

/// @file
/// WorldMapRenderWork, the World Map's view as the World Map overlay reads it.

#include "system/heap_slots.h"

#include "types.h"
#include "static_assert.h"

/// The world map's perspective work (struct WorldMapView) as the world
/// map overlay sees it, for the map's rotation.
struct WorldMapRenderWork {
  u8 beforeAnimationActive[252]; ///< WorldMapView's fields before it
  s8 animationActive;            ///< WorldMapView's; not read by name
  u8 unused[3];                  ///< WorldMapView's
  s16 windowTop;                 ///< WorldMapView's; not read by name
  s16 windowBottom;              ///< WorldMapView's; not read by name
  u8 windowToPitch[22]; ///< WorldMapView's shown window, tiles and pitch
  u16 rotation;         ///< WorldMapView's yaw, 1/65536 turns
};

STATIC_ASSERT(offsetof(struct WorldMapRenderWork, animationActive) == 252);
STATIC_ASSERT(offsetof(struct WorldMapRenderWork, windowTop) == 256);
STATIC_ASSERT(offsetof(struct WorldMapRenderWork, rotation) == 282);

#endif
