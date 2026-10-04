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

/* The field map's work block (heap slot map_work). It follows The Broken
   Seal's MapState to the blend script; the last BLDCNT written sits four
   bytes later. */
struct MapWork {
    u8 unknown_000[0x14];
    u16 flags;
    /* Nonzero selects 8 bpp characters for the animation copies. */
    u8 wide_tiles;
    u8 unknown_017;
    struct MapAnimation anim[16];
    struct BlendScriptState blend;   /* 0x0d8 */
    u8 unknown_0e4[0x23];
    u8 blend_control;                /* 0x107: last BLDCNT written */
};

LAYOUT_SIZE_GUARD(MapAnimation_Size, struct MapAnimation, 12);
LAYOUT_OFFSET_GUARD(MapWork_Animation, struct MapWork, anim, 0x18);
LAYOUT_OFFSET_GUARD(MapWork_Blend, struct MapWork, blend, 0xd8);
LAYOUT_OFFSET_GUARD(MapWork_BlendControl, struct MapWork, blend_control, 0x107);

#endif
