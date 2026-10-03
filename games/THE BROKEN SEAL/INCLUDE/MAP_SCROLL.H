#ifndef ALCHEMY_MAP_SCROLL_H
#define ALCHEMY_MAP_SCROLL_H

#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "LAYOUT_GUARD.H"

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

struct PerspectiveWork {
    s32 *origin;
    s32 shake_x, shake_y, shake_decay;
    void *unknown_010;              /* 0x010 */
    u16 fade;                       /* 0x014 */
    u8 fade_step;                   /* 0x016 */
    u8 unknown_017[0xe4 - 0x17];
    s32 view_x;                    /* 0x0e4 */
    s32 view_y;                    /* 0x0e8 */
    s32 scale_x;                    /* 0x0ec */
    s32 scale_y;                    /* 0x0f0 */
    s32 limit_x;                    /* 0x0f4 */
    s32 limit_y;                    /* 0x0f8 */
    u8 unknown_0fc[4];
    u16 window_top;                 /* 0x100 */
    u16 window_bottom;              /* 0x102 */
    u16 shown_top;                  /* 0x104: the window the frame shows */
    u16 shown_bottom;               /* 0x106 */
    u16 window_pending;             /* 0x108 */
    u8 unknown_10a[6];
    void *tiles;                    /* 0x110 */
    u8 unknown_114[4];
    u16 pitch;                      /* 0x118 */
    u16 yaw;                        /* 0x11a */
    u8 unknown_11c[0x1c];
    u16 tile_ids[256];              /* 0x138 */
    u16 quarter_tiles[8];           /* 0x338: the tile in each layer quarter */
    s32 far_plane;                  /* 0x348 */
    s32 distance;                   /* 0x34c */
    u8 unknown_350[4];
    s32 zoom;                       /* 0x354 */
    u16 turn;                       /* 0x358 */
    u8 unknown_35a[2];
};

LAYOUT_SIZE_GUARD(PerspectiveWork_Size, struct PerspectiveWork, 0x35c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_View, struct PerspectiveWork, view_x, 0xe4);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Pitch, struct PerspectiveWork, pitch, 0x118);
LAYOUT_OFFSET_GUARD(PerspectiveWork_TileIds, struct PerspectiveWork, tile_ids, 0x138);
LAYOUT_OFFSET_GUARD(PerspectiveWork_QuarterTiles, struct PerspectiveWork, quarter_tiles, 0x338);
LAYOUT_OFFSET_GUARD(PerspectiveWork_FarPlane, struct PerspectiveWork, far_plane, 0x348);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Zoom, struct PerspectiveWork, zoom, 0x354);

struct BgScroll { u16 x, y; };

extern struct BgScroll gBgScroll[];

void Map_RenderPaletteMappedBlock(u32 layer, s32 x, s32 y);
void Map_RenderMetatileRow(u32 layer, s32 x, s32 y);

#endif
