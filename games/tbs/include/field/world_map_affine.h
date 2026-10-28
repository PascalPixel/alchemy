#ifndef ALCHEMY_WORLD_MAP_AFFINE_H
#define ALCHEMY_WORLD_MAP_AFFINE_H

/// @file
/// The world map's scanlines and affine lines.

#include "module/field/field.h"
#include "types.h"
#include "static_assert.h"

/// One screen line's ground-plane terms on the world map: its width scale and
/// distance, from which its affine parameters are built
/// (WorldMap_BuildScanlineTable, when the pitch changes).
struct WorldMapScanline {
  /// The line's width scale and its signed ground distance, halved, both 16.16
  /// fixed point; both 0 for a line above the horizon.
  s32 scale, depth;
  s32 offsetX, offsetY; ///< 0 from WorldMap_BuildScanlineTable
  s16 pa, pc;
};

/// One screen line's affine parameters for a background, as the HBlank DMA
/// copies them into the BG2 or BG3 registers.
struct WorldMapAffineLine {
  s16 pa, pb, pc, pd; ///< the matrix, 8.8 fixed point as BG2PA to BG2PD take it
  s32 x, y; ///< the reference point, 20.8 fixed point as BG2X and BG2Y take it
};

/// One screen line's affine parameters for BG2 and BG3.
struct WorldMapAffinePair {
  struct WorldMapAffineLine bg2, bg3;
};

/// The world map's affine work in HEAP_SLOT_WORLD_MAP_AFFINE: the scanline
/// terms and two pages of per-line affine parameters, one built while the other
/// is shown. The world map's setup takes the slot; the next scene change frees
/// it.
struct WorldMapAffineWork {
  struct WorldMapScanline scanlines[160];
  struct WorldMapAffinePair pages[2][160];
  u8 unused[4]; ///< no code names it
};

STATIC_ASSERT(sizeof(struct WorldMapScanline) == 20);
STATIC_ASSERT(offsetof(struct WorldMapScanline, pa) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffineLine) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffinePair) == 32);
STATIC_ASSERT(offsetof(struct WorldMapAffineWork, pages) == 0xc80);
STATIC_ASSERT(offsetof(struct WorldMapAffineWork, unused) == 0x3480);
STATIC_ASSERT(sizeof(struct WorldMapAffineWork) == 0x3484);

#endif
