/// @file
/// WorldMapRenderWork, the map renderer's work.

#include "global_cells.h"
#ifndef ALCHEMY_WORLD_MAP_RENDER_WORK_H
#define ALCHEMY_WORLD_MAP_RENDER_WORK_H

#include "types.h"
#include "static_assert.h"

/// The world map's perspective work (struct PerspectiveWork) as the world
/// map overlay sees it, for the map's rotation.
struct WorldMapRenderWork {
  u8 beforeActive[0xfc]; ///< PerspectiveWork's fields before animationActive
  s8 active;    ///< PerspectiveWork's animationActive; not read by name
  u8 unused[3]; ///< PerspectiveWork's; no code names it
  s16 first;    ///< PerspectiveWork's windowTop; not read by name
  s16 second;   ///< PerspectiveWork's windowBottom; not read by name
  u8 windowToPitch[0x16]; ///< PerspectiveWork's shown window, tiles and pitch
  u16 rotation;           ///< PerspectiveWork's yaw, 1/65536 turns
};

STATIC_ASSERT(offsetof(struct WorldMapRenderWork, active) == 0xfc);
STATIC_ASSERT(offsetof(struct WorldMapRenderWork, first) == 0x100);
STATIC_ASSERT(offsetof(struct WorldMapRenderWork, rotation) == 0x11a);

#endif
