#ifndef ALCHEMY_MAP_SCROLL_H
#define ALCHEMY_MAP_SCROLL_H

#include "types.h"
#include "global_cells.h"
#include "layout_guard.h"
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

LAYOUT_SIZE_GUARD(MapLayerScroll_Size, struct MapLayerScroll, 0x30);
LAYOUT_OFFSET_GUARD(MapLayerScroll_Cells, struct MapLayerScroll, cells, 0x2c);
LAYOUT_SIZE_GUARD(MapScrollWork_Size, struct MapScrollWork, 0x194);
LAYOUT_OFFSET_GUARD(MapScrollWork_Animation, struct MapScrollWork, anim, 0x18);
LAYOUT_OFFSET_GUARD(MapScrollWork_Blend, struct MapScrollWork, blend, 0xd8);
LAYOUT_OFFSET_GUARD(MapScrollWork_View, struct MapScrollWork, viewX, 0xe4);
LAYOUT_OFFSET_GUARD(MapScrollWork_Active, struct MapScrollWork, animationActive,
                    0xfc);
LAYOUT_OFFSET_GUARD(MapScrollWork_Layers, struct MapScrollWork, layers, 0x104);

struct BgScroll {
  u16 x, y;
};

extern struct BgScroll gBgScroll[];

#endif
