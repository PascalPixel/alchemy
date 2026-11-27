#ifndef ALCHEMY_WORLD_MAP_AFFINE_H
#define ALCHEMY_WORLD_MAP_AFFINE_H

// The world map's scanlines and affine lines.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "system/globals.h"

// One screen line's ground-plane terms on the world map: its width scale and distance, from which
// its affine parameters are built (fdWorldMapBuildScanlineTable, when the pitch changes).
typedef struct WORLD_MAP_SCANLINE
{
    // The line's width scale and its signed ground distance, halved, both 16.16 fixed point; both 0
    // for a line above the horizon.
    FIXED  scale, depth;
    Sint32 offsetX, offsetY; // 0 from fdWorldMapBuildScanlineTable
    Sint16 pa, pc;
} WORLD_MAP_SCANLINE;

// One screen line's affine parameters for a background, as the HBlank DMA copies them into the BG2
// or BG3 registers.
typedef struct WORLD_MAP_AFFINE_LINE
{
    // the matrix, 8.8 fixed point as BG2PA to BG2PD take it
    Sint16 pa, pb, pc, pd;
    // the reference point, 20.8 fixed point as BG2X and BG2Y take it
    Sint32 x, y;
} WORLD_MAP_AFFINE_LINE;

// One screen line's affine parameters for BG2 and BG3.
typedef struct WORLD_MAP_AFFINE_PAIR
{
    WORLD_MAP_AFFINE_LINE bg2, bg3;
} WORLD_MAP_AFFINE_PAIR;

// The world map's affine state (heap slot worldMapAffine): the scanline terms and two pages of
// per-line affine parameters, one built while the other is shown.
typedef struct WORLD_MAP_AFFINE_STATE
{
    WORLD_MAP_SCANLINE    scanlines[160];
    WORLD_MAP_AFFINE_PAIR pages[2][160];
    Uint8                 unused[4]; // allocated with the rest; no code reads it
} WORLD_MAP_AFFINE_STATE;

STATIC_ASSERT(sizeof(WORLD_MAP_SCANLINE) == 20);

STATIC_ASSERT(offsetof(WORLD_MAP_SCANLINE, pa) == 16);
STATIC_ASSERT(sizeof(WORLD_MAP_AFFINE_LINE) == 16);
STATIC_ASSERT(sizeof(WORLD_MAP_AFFINE_PAIR) == 32);
STATIC_ASSERT(offsetof(WORLD_MAP_AFFINE_STATE, pages) == 3200);

#endif
