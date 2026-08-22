#ifndef ALCHEMY_MAP_RENDER_WORK_H
#define ALCHEMY_MAP_RENDER_WORK_H

#include "types.h"

struct MapRenderWork {
    u8 unknown_000[0xfc];
    s8 active;
    u8 unknown_0fd[3];
    s16 first;
    s16 second;
};

extern struct MapRenderWork *Data_03001e70;

#endif
