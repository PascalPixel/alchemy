#ifndef ALCHEMY_MAPFRAME_H
#define ALCHEMY_MAPFRAME_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* The ground-plane table built for each of the 160 screen lines. The
   transform replaces the offsets and coefficients before presentation. */
struct MapScanline {
    s32 scale, depth;
    s32 offset_x, offset_y;
    s16 pa, pc;
};

/* The BG2/BG3 affine register order streamed by the HBlank DMA. */
struct MapAffineLine {
    s16 pa, pb, pc, pd;
    s32 x, y;
};

struct MapAffinePair {
    struct MapAffineLine bg2, bg3;
};

/* Slot 7's allocation holds the input table and two output pages. */
struct MapFrameWork {
    struct MapScanline scanlines[160];
    struct MapAffinePair pages[2][160];
    u8 unknown_3480[4];
};

LAYOUT_SIZE_GUARD(MapScanline_Size, struct MapScanline, 20);
LAYOUT_OFFSET_GUARD(MapScanline_Coefficients, struct MapScanline, pa, 16);
LAYOUT_SIZE_GUARD(MapAffineLine_Size, struct MapAffineLine, 16);
LAYOUT_SIZE_GUARD(MapAffinePair_Size, struct MapAffinePair, 32);
LAYOUT_OFFSET_GUARD(MapFrameWork_Pages, struct MapFrameWork, pages, 0xc80);
LAYOUT_OFFSET_GUARD(MapFrameWork_UnknownTail, struct MapFrameWork, unknown_3480, 0x3480);
LAYOUT_SIZE_GUARD(MapFrameWork_Size, struct MapFrameWork, 0x3484);

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct MapScanline *row);

#endif
