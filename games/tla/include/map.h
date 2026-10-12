#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "types.h"
#include "layout_guard.h"

struct BlendScriptState {
    u16 *script;
    u16 *cursor;
    u16 delay;
    u16 paused;
};

struct MapAnimation {
    const u16 *start;
    const u16 *cursor;
    u16 timer;
    u16 paused;
};

struct MapCell {
    u16 metatile_and_flags;
    u8 collision_code;
    u8 attribute_b;
};

struct MapLayer {
    s32 x, y;
    u8 unknown_08[0x28];
    struct MapCell *cells;
    u8 *terrain_ids;
};

struct MapWork {
    s32 *origin;
    s32 shake_x, shake_y, shake_decay;
    u8 *script;
    u16 flags;
    u8 wide_tiles;
    u8 unknown_017;
    struct MapAnimation anim[16];
    struct BlendScriptState blend;
    s32 view_x;
    s32 view_z;
    s32 min_x;
    s32 min_z;
    s32 max_x;
    s32 max_z;
    u8 unknown_0fc[0xb];
    u8 blend_control;
    struct MapLayer layers[4];
    u8 unknown_1e8[0x774];
    s32 far_plane;
    s32 zoom;
    s32 zoom_from;
    s32 zoom_to;
    s16 zoom_frames;
    s16 zoom_step;
};

struct MapViewWork {
    u8 unknown_000[0x100];
    s8 animation_active;
    u8 unknown_101[3];
    u16 window_top;
    u16 window_bottom;
    u8 unknown_108[0x18];
    u32 *resources;
    u8 unknown_124[0x84c];
    s8 extra_tiles;
    u8 frame_shown;
    u8 frame_pending;
    s8 area;
};

struct WorldPosition {
    s32 x;
    u8 unknown_04[4];
    s32 y;
};

s32 GetWorldMapCollision(struct WorldPosition *position);

LAYOUT_SIZE_GUARD(MapLayer_Size, struct MapLayer, 56);
LAYOUT_OFFSET_GUARD(MapWork_Layers, struct MapWork, layers, 0x108);

LAYOUT_SIZE_GUARD(MapAnimation_Size, struct MapAnimation, 12);
LAYOUT_OFFSET_GUARD(MapWork_Animation, struct MapWork, anim, 0x18);
LAYOUT_OFFSET_GUARD(MapWork_Blend, struct MapWork, blend, 0xd8);
LAYOUT_OFFSET_GUARD(MapWork_BlendControl, struct MapWork, blend_control, 0x107);
LAYOUT_OFFSET_GUARD(MapWork_MinX, struct MapWork, min_x, 0xec);
LAYOUT_OFFSET_GUARD(MapWork_FarPlane, struct MapWork, far_plane, 0x95c);
LAYOUT_OFFSET_GUARD(MapWork_ZoomStep, struct MapWork, zoom_step, 0x96e);

#endif
