#ifndef ALCHEMY_RESOURCE_384_H
#define ALCHEMY_RESOURCE_384_H

#include "types.h"

/* Behavior names for the address-stable overlay symbols. */

enum Resource384ActorId {
    RESOURCE384_ACTOR_8 = 8,
    RESOURCE384_ACTOR_9 = 9,
    RESOURCE384_ACTOR_10 = 10,
};

enum {
    RESOURCE384_SPECIAL_STEP = 10,
    RESOURCE384_ACTOR_FLAG = 0x14,
    RESOURCE384_SCRIPT_STATE = 521,
};

enum Resource384Cue {
    RESOURCE384_CUE_ACTOR_9_INTRO = 0x1388,
    RESOURCE384_CUE_ACTOR_10_INTRO = 0x1389,
    RESOURCE384_CUE_OPENING = 0x138a,
    RESOURCE384_CUE_ACTOR_9_BEAT = 0x138c,
    RESOURCE384_CUE_ACTOR_10_BEAT = 0x138d,
    RESOURCE384_CUE_ACTOR_8_BEAT = 0x138e,
};

#define RESOURCE384_HEADER_DATA  ((u8 *)0x020082d0)
#define RESOURCE384_SCENE_LAYOUT ((u8 *)0x02008348)

struct Resource384Actor {
    u8 unknown_00[0x59];
    u8 flags_59;
};

typedef char Resource384Actor_flags_59_offset[
    (u32)&((struct Resource384Actor *)0)->flags_59 == 0x59 ? 1 : -1
];

#endif
