#include "GLOBAL_CELLS.H"
#ifndef ALCHEMY_MAP_SCROLL_H
#define ALCHEMY_MAP_SCROLL_H

#include "TYPES.H"

struct MapLayerScroll {
    s32 x, y;
    s32 offset_x, offset_y;
    s32 scale_x, scale_y;
    s32 speed_x, speed_y;
    s32 phase_x, phase_y;
    u16 mask_x, mask_y;
    s32 unknown_2c;
};

struct MapScrollWork {
    s32 *origin;
    s32 shake_x, shake_y, shake_decay;
    u8 unknown_010[0xe4 - 0x10];
    s32 view_x, view_y;
    s32 min_x, min_y, max_x, max_y;
    s32 unknown_0fc;
    u8 enabled[3];
    u8 unknown_103;
    struct MapLayerScroll layers[3];
};

struct BgScroll { u16 x, y; };

extern struct BgScroll gBgScroll[];

void Map_RenderPaletteMappedBlock(u32 layer, s32 x, s32 y);
void Map_RenderMetatileRow(u32 layer, s32 x, s32 y);

#endif
