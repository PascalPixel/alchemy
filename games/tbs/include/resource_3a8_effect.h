#ifndef ALCHEMY_RESOURCE_3A8_EFFECT_H
#define ALCHEMY_RESOURCE_3A8_EFFECT_H

#include "types.h"
#include "layout_guard.h"

struct Resource3a8Effect {
  u8 unk00[0x06];
  u16 unk06;
  u8 unk08[0x5e];
  s16 actionTimer;
};

LAYOUT_OFFSET_GUARD(Resource3a8Effect_unknown_06_offset,
                    struct Resource3a8Effect, unk06, 0x06);
LAYOUT_OFFSET_GUARD(Resource3a8Effect_action_timer_offset,
                    struct Resource3a8Effect, actionTimer, 0x66);
LAYOUT_SIZE_GUARD(Resource3a8Effect_size, struct Resource3a8Effect, 0x68);

#endif
