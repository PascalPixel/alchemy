#ifndef ALCHEMY_PERSPECT_H
#define ALCHEMY_PERSPECT_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct PerspectiveWork {
    s32 *origin;
    s32 shake_x, shake_y, shake_decay;
    u8 *script;                     /* 0x010 */
    u16 fade;                       /* 0x014 */
    u8 fade_step;                   /* 0x016 */
    u8 unknown_017[0xe4 - 0x17];
    s32 view_x;                    /* 0x0e4 */
    s32 view_y;                    /* 0x0e8 */
    s32 scale_x;                    /* 0x0ec */
    s32 scale_y;                    /* 0x0f0 */
    s32 limit_x;                    /* 0x0f4 */
    s32 limit_y;                    /* 0x0f8 */
    s8 animation_active;           /* 0x0fc */
    u8 unknown_0fd[3];
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
    u32 *resources;                /* 0x11c: current area graphics */
    u8 unknown_120[0x18];
    u16 tile_ids[256];              /* 0x138 */
    u16 quarter_tiles[8];           /* 0x338: the tile in each layer quarter */
    s32 far_plane;                  /* 0x348 */
    s32 distance;                   /* 0x34c */
    s32 previous_zoom;
    s32 zoom;                       /* 0x354 */
    u16 zoom_transition_frames;                       /* 0x358 */
    s16 zoom_transition_step;
};


LAYOUT_SIZE_GUARD(PerspectiveWork_Size, struct PerspectiveWork, 0x35c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_View, struct PerspectiveWork, view_x, 0xe4);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Pitch, struct PerspectiveWork, pitch, 0x118);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Active, struct PerspectiveWork, animation_active, 0xfc);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Resources, struct PerspectiveWork, resources, 0x11c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_TileIds, struct PerspectiveWork, tile_ids, 0x138);
LAYOUT_OFFSET_GUARD(PerspectiveWork_QuarterTiles, struct PerspectiveWork, quarter_tiles, 0x338);
LAYOUT_OFFSET_GUARD(PerspectiveWork_FarPlane, struct PerspectiveWork, far_plane, 0x348);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Zoom, struct PerspectiveWork, zoom, 0x354);
LAYOUT_OFFSET_GUARD(PerspectiveWork_PreviousZoom, struct PerspectiveWork, previous_zoom, 0x350);
LAYOUT_OFFSET_GUARD(PerspectiveWork_TransitionFrames, struct PerspectiveWork, zoom_transition_frames, 0x358);
LAYOUT_OFFSET_GUARD(PerspectiveWork_TransitionStep, struct PerspectiveWork, zoom_transition_step, 0x35a);

#endif
