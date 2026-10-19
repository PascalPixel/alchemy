#ifndef ALCHEMY_MAPFRAME_H
#define ALCHEMY_MAPFRAME_H

/// @file
/// The map frame's scanlines and affine lines.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

/// One screen line's ground-plane terms on the world map: its width scale and
/// distance, from which its affine parameters are built.
struct MapScanline {
  s32 scale, depth;
  s32 offsetX, offsetY;
  s16 pa, pc;
};

/// One screen line's affine parameters for a background, as the HBlank DMA
/// copies them into the BG2 or BG3 registers.
struct MapAffineLine {
  s16 pa, pb, pc, pd;
  s32 x, y;
};

/// One screen line's affine parameters for BG2 and BG3.
struct MapAffinePair {
  struct MapAffineLine bg2, bg3;
};

/// The world map's frame work in heap slot 7: the scanline terms and two pages
/// of per-line affine parameters, one built while the other is shown.
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
