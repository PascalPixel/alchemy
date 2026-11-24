#ifndef ALCHEMY_WORLD_MAP_AFFINE_H
#define ALCHEMY_WORLD_MAP_AFFINE_H

/// @file
/// The world map's scanlines and affine lines.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/// One screen line's ground-plane terms on the world map: its width scale and
/// distance, from which its affine parameters are built
/// (WorldMap_BuildScanlineTable, when the pitch changes).
struct WorldMapScanline {
  /// The line's width scale, 16.16 fixed point; 0 for a line above the
  /// horizon.
  FIXED scale;
  /// The line's signed ground distance, halved, 16.16 fixed point; 0 for a
  /// line above the horizon.
  FIXED depth;
  /// Where the line starts on the map, from the origin, 16.16; 0 from
  /// WorldMap_BuildScanlineTable.
  s32 offsetX;
  s32 offsetY; ///< the same, down the map
  s16 pa;      ///< BG2PA as last worked out, kept while the yaw holds
  s16 pc;      ///< BG2PC, the same
};

/// One screen line's affine parameters for a background, as the HBlank DMA
/// copies them into the BG2 or BG3 registers, BG2PA to BG2Y.
struct WorldMapAffineLine {
  /// The matrix, 8.8 fixed point as BG2PA to BG2PD take it;
  /// WorldMap_UpdateAffineLines writes pa and pc only.
  s16 pa, pb, pc, pd;
  s32 x, y; ///< the reference point, 20.8 fixed point as BG2X and BG2Y take it
};

/// One screen line's affine parameters for BG2 and BG3.
struct WorldMapAffinePair {
  struct WorldMapAffineLine bg2, bg3;
};

/// The world map's affine state in HEAP_SLOT_WORLD_MAP_AFFINE: the scanline
/// terms and two pages of per-line affine parameters, one built while the other
/// is shown.
struct WorldMapAffineState {
  struct WorldMapScanline scanlines[160];
  struct WorldMapAffinePair pages[2][160];
  u8 unused[4];
};

STATIC_ASSERT(sizeof(struct WorldMapScanline) == 20);
STATIC_ASSERT(offsetof(struct WorldMapScanline, pa) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffineLine) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffinePair) == 32);
STATIC_ASSERT(offsetof(struct WorldMapAffineState, pages) == 3200);
STATIC_ASSERT(offsetof(struct WorldMapAffineState, unused) == 13440);
STATIC_ASSERT(sizeof(struct WorldMapAffineState) == 13444);

#endif
