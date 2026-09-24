#ifndef ALCHEMY_MAP_RENDER_WORK_H
#define ALCHEMY_MAP_RENDER_WORK_H

#include "TYPES.H"

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

extern struct MapRenderWork *gMapWork;

#endif
