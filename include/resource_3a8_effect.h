#ifndef ALCHEMY_RESOURCE_3A8_EFFECT_H
#define ALCHEMY_RESOURCE_3A8_EFFECT_H

#include "types.h"

/* Fields independently observed at +0x06 and +0x66 in resource_3a8. */
struct Resource3a8Effect {
    u8 unknown_00[0x06];
    u16 unknown_06;
    u8 unknown_08[0x5e];
    s16 action_timer;
};

#define RESOURCE3A8_EFFECT_OFFSET(field) \
    ((u32)&(((struct Resource3a8Effect *)0)->field))
typedef char Resource3a8Effect_unknown_06_offset[
    RESOURCE3A8_EFFECT_OFFSET(unknown_06) == 0x06 ? 1 : -1
];
typedef char Resource3a8Effect_action_timer_offset[
    RESOURCE3A8_EFFECT_OFFSET(action_timer) == 0x66 ? 1 : -1
];
typedef char Resource3a8Effect_size[
    sizeof(struct Resource3a8Effect) == 0x68 ? 1 : -1
];
#undef RESOURCE3A8_EFFECT_OFFSET

#endif
