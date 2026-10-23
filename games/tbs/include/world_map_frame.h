#ifndef ALCHEMY_WORLD_MAP_FRAME_H
#define ALCHEMY_WORLD_MAP_FRAME_H

/// @file
/// The map frame's scanlines and affine lines.

#include "unit/field/field.h"
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

/// The world map's frame work in HEAP_SLOT_WORLD_MAP_FRAME: the scanline terms
/// and two pages of per-line affine parameters, one built while the other is
/// shown. The world map's setup takes the slot; the next scene change frees it.
struct WorldMapFrameWork {
  struct WorldMapScanline scanlines[160];
  struct WorldMapAffinePair pages[2][160];
  u8 unk3480[4];
};

STATIC_ASSERT(sizeof(struct WorldMapScanline) == 20);
STATIC_ASSERT(offsetof(struct WorldMapScanline, pa) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffineLine) == 16);
STATIC_ASSERT(sizeof(struct WorldMapAffinePair) == 32);
STATIC_ASSERT(offsetof(struct WorldMapFrameWork, pages) == 0xc80);
STATIC_ASSERT(offsetof(struct WorldMapFrameWork, unk3480) == 0x3480);
STATIC_ASSERT(sizeof(struct WorldMapFrameWork) == 0x3484);

#endif
