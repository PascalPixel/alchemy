#ifndef ALCHEMY_RESOURCE_3AF_MOTION_H
#define ALCHEMY_RESOURCE_3AF_MOTION_H

#include "types.h"

/*
 * The portion of a resource_3af motion record observed by the 0x02000030
 * update. Both components start at 0x10000 and move in fixed-point-sized
 * steps; their broader role is not yet known.
 */
struct Resource3afMotion {
    u8 unknown_00[0x18];
    s32 component_a;
    s32 component_b;
    u8 unknown_20[0x44];
    s16 countdown;
};

#define RESOURCE3AF_MOTION_OFFSET(field) \
    ((u32)&(((struct Resource3afMotion *)0)->field))

typedef char Resource3afMotion_component_a_offset[
    RESOURCE3AF_MOTION_OFFSET(component_a) == 0x18 ? 1 : -1
];
typedef char Resource3afMotion_component_b_offset[
    RESOURCE3AF_MOTION_OFFSET(component_b) == 0x1C ? 1 : -1
];
typedef char Resource3afMotion_countdown_offset[
    RESOURCE3AF_MOTION_OFFSET(countdown) == 0x64 ? 1 : -1
];

#undef RESOURCE3AF_MOTION_OFFSET

#endif
