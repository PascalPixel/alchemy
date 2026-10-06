#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

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
    u8 unknown_00[0x30];
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
    u8 unknown_0e4[0x23];
    u8 blend_control;
    struct MapLayer layers[4];
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

#endif
