#ifndef ALCHEMY_WORLD_MAP_AFFINE_H
#define ALCHEMY_WORLD_MAP_AFFINE_H

/* The world map's scanlines and affine lines. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* One screen line's ground-plane terms on the world map: its width scale and
   distance, from which its affine parameters are built
   (fdWorldMapBuildScanlineTable, when the pitch changes). */
typedef struct WORLD_MAP_SCANLINE {
	/* The line's width scale, 16.16 fixed point; 0 for a line above the
	   horizon. */
	FIXED scale;
	/* The line's signed ground distance, halved, 16.16 fixed point; 0 for a
	   line above the horizon. */
	FIXED depth;
	/* Where the line starts on the map, from the origin, 16.16; 0 from
	   fdWorldMapBuildScanlineTable. */
	Sint32 offsetX;
	Sint32 offsetY; /* the same, down the map */
	Sint16 pa;		/* BG2PA as last worked out, kept while the yaw holds */
	Sint16 pc;		/* BG2PC, the same */
} WORLD_MAP_SCANLINE;

/* One screen line's affine parameters for a background, as the HBlank DMA
   copies them into the BG2 or BG3 registers, BG2PA to BG2Y. */
typedef struct WORLD_MAP_AFFINE_LINE {
	/* The matrix, 8.8 fixed point as BG2PA to BG2PD take it;
	   fdWorldMapUpdateAffineLines writes pa and pc only. */
	Sint16 pa, pb, pc, pd;
	/* the reference point, 20.8 fixed point as BG2X and BG2Y take it */
	Sint32 x, y;
} WORLD_MAP_AFFINE_LINE;

/* One screen line's affine parameters for BG2 and BG3. */
typedef struct WORLD_MAP_AFFINE_PAIR {
	WORLD_MAP_AFFINE_LINE bg2, bg3;
} WORLD_MAP_AFFINE_PAIR;

/* The world map's affine state in HEAP_SLOT_WORLD_MAP_AFFINE: the scanline
   terms and two pages of per-line affine parameters, one built while the other
   is shown. */
typedef struct WORLD_MAP_AFFINE_STATE {
	WORLD_MAP_SCANLINE	  scanlines[160];
	WORLD_MAP_AFFINE_PAIR pages[2][160];
	Uint8				  unused[4];
} WORLD_MAP_AFFINE_STATE;

STATIC_ASSERT(sizeof(WORLD_MAP_SCANLINE) == 20);
STATIC_ASSERT(offsetof(WORLD_MAP_SCANLINE, pa) == 16);
STATIC_ASSERT(sizeof(WORLD_MAP_AFFINE_LINE) == 16);
STATIC_ASSERT(sizeof(WORLD_MAP_AFFINE_PAIR) == 32);
STATIC_ASSERT(offsetof(WORLD_MAP_AFFINE_STATE, pages) == 3200);
STATIC_ASSERT(offsetof(WORLD_MAP_AFFINE_STATE, unused) == 13440);
STATIC_ASSERT(sizeof(WORLD_MAP_AFFINE_STATE) == 13444);

#endif
