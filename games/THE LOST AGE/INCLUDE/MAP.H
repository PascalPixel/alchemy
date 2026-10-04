#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* A blend script's playback state. */
struct BlendScriptState {
    u16 *script;
    u16 *cursor;
    u16 delay;
    u16 paused;
};

/* A tile-animation channel. */
struct MapAnimation {
    const u16 *start;
    const u16 *cursor;
    u16 timer;
    u16 paused;
};

/* A map cell: its metatile, then the collision code. */
struct MapCell {
    u16 metatile_and_flags;
    u8 collision_code;
    u8 attribute_b;
};

/* A map layer: 56 bytes, eight more than The Broken Seal's, with the cell
   array at 0x30 and, after it, one byte per cell choosing the cell's
   four-byte terrain record in gMapCollision. */
struct MapLayer {
    u8 unknown_00[0x30];
    struct MapCell *cells;
    u8 *terrain_ids;
};

/* The field map's work block (heap slot map_work). It follows The Broken
   Seal's MapState to the blend script; the last BLDCNT written sits four
   bytes later, and the four layers follow it. */
struct MapWork {
    s32 *origin;
    s32 shake_x, shake_y, shake_decay;
    u8 *script;
    u16 flags;
    /* Nonzero selects 8 bpp characters for the animation copies. */
    u8 wide_tiles;
    u8 unknown_017;
    struct MapAnimation anim[16];
    struct BlendScriptState blend;   /* 0x0d8 */
    u8 unknown_0e4[0x23];
    u8 blend_control;                /* 0x107: last BLDCNT written */
    struct MapLayer layers[4];       /* 0x108 */
};

/* A position on the world map, in 16.16 fixed point. */
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
