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
  /// The line's width scale and its signed ground distance, halved, both 16.16
  /// fixed point; both 0 for a line above the horizon.
  FIXED scale, depth;
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

/// The world map's affine state (heap slot worldMapAffine): the scanline terms
/// and two pages of per-line affine parameters, one built while the other is
/// shown.
struct WorldMapAffineState {
  struct WorldMapScanline scanlines[160];
  struct WorldMapAffinePair pages[2][160];
  u8 unused[4]; ///< allocated with the rest; no code reads it
};

STATIC_ASSERT(sizeof(struct WorldMapScanline) == 20);
/// The world map camera's yaw, in 1/65536 turns, the scanline parameters were
/// built for; the vertex update rebuilds them when it changes.
extern u32 gScanlineAngle;

STATIC_ASSERT(offsetof(struct WorldMapScanline, pa) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffineLine) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffinePair) == 32);
STATIC_ASSERT(offsetof(struct WorldMapAffineState, pages) == 3200);

#endif
