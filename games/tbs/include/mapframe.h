#ifndef ALCHEMY_MAPFRAME_H
#define ALCHEMY_MAPFRAME_H

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

struct MapScanline {
  s32 scale, depth;
  s32 offsetX, offsetY;
  s16 pa, pc;
};

struct MapAffineLine {
  s16 pa, pb, pc, pd;
  s32 x, y;
};

struct MapAffinePair {
  struct MapAffineLine bg2, bg3;
};

struct MapFrameWork {
  struct MapScanline scanlines[160];
  struct MapAffinePair pages[2][160];
  u8 unk3480[4];
};

STATIC_ASSERT(sizeof(struct MapScanline) == 20);
STATIC_ASSERT(offsetof(struct MapScanline, pa) == 16);
STATIC_ASSERT(sizeof(struct MapAffineLine) == 16);
STATIC_ASSERT(sizeof(struct MapAffinePair) == 32);
STATIC_ASSERT(offsetof(struct MapFrameWork, pages) == 0xc80);
STATIC_ASSERT(offsetof(struct MapFrameWork, unk3480) == 0x3480);
STATIC_ASSERT(sizeof(struct MapFrameWork) == 0x3484);

#endif
