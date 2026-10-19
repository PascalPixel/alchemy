#ifndef ALCHEMY_MAP_SCROLL_H
#define ALCHEMY_MAP_SCROLL_H

/// @file
/// The map layers' scroll work and the background scroll values.

#include "types.h"
#include "global_cells.h"
#include "static_assert.h"
#include "map.h"

struct MapLayerScroll {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  struct MapCell *cells;
};

struct MapScrollWork {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 footprints;
  struct MapAnimation anim[16];
  struct BlendScriptState blend;
  s32 viewX, viewY;
  s32 minX, minY, maxX, maxY;
  s8 animationActive;
  u8 unk0fd[3];
  u8 enabled[3];
  u8 unk103;
  struct MapLayerScroll layers[3];
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

struct BgScroll {
  u16 x, y;
};

extern struct BgScroll gBgScroll[];

#endif
