#ifndef ALCHEMY_RESOURCE_3A8_EFFECT_H
#define ALCHEMY_RESOURCE_3A8_EFFECT_H

/// @file
/// The view a scene overlay takes of an effect; most fields are still unnamed.

#include "types.h"
#include "static_assert.h"

struct Resource3a8Effect {
  u8 unk00[0x06];
  u16 unk06;
  u8 unk08[0x5e];
  s16 actionTimer;
};

STATIC_ASSERT(offsetof(struct Resource3a8Effect, unk06) == 0x06);
STATIC_ASSERT(offsetof(struct Resource3a8Effect, actionTimer) == 0x66);
STATIC_ASSERT(sizeof(struct Resource3a8Effect) == 0x68);

#endif
