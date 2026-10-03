#include "GLOBAL_CELLS.H"
#ifndef ALCHEMY_MAP_RENDER_WORK_H
#define ALCHEMY_MAP_RENDER_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* A transport prefix used by both map modes. This is not an allocation
   or a complete map owner; each mode keeps its own full record. */

struct MapRenderWork {
    u8 unknown_000[0xfc];
    s8 active;
    u8 unknown_0fd[3];
    s16 first;
    s16 second;
    u8 unknown_104[0x16];
    /* The rotation sprites on the map turn to. */
    u16 rotation;
};

LAYOUT_OFFSET_GUARD(MapRenderWork_Active, struct MapRenderWork, active, 0xfc);
LAYOUT_OFFSET_GUARD(MapRenderWork_Window, struct MapRenderWork, first, 0x100);
LAYOUT_OFFSET_GUARD(MapRenderWork_Rotation, struct MapRenderWork, rotation, 0x11a);

#endif
