#ifndef ALCHEMY_FXBLEND_H
#define ALCHEMY_FXBLEND_H

/// @file
/// FieldBlendWork, the field's colour blend work.

#include "types.h"
#include "static_assert.h"

/// The storm's lightning work: the colours it flashes between and their
/// difference, the countdown to the next flash and whether the thunder is
/// loud.
struct FieldBlendWork {
  s16 from[0x540];
  s16 to[0x540];
  s16 delta[0x540];
  s16 timer;
  s16 enabled;
  s16 loud;
  u8 unk1f86[2];
};

STATIC_ASSERT(sizeof(struct FieldBlendWork) == 0x1f88);
STATIC_ASSERT(offsetof(struct FieldBlendWork, to) == 0xa80);
STATIC_ASSERT(offsetof(struct FieldBlendWork, delta) == 0x1500);
STATIC_ASSERT(offsetof(struct FieldBlendWork, timer) == 0x1f80);
STATIC_ASSERT(offsetof(struct FieldBlendWork, loud) == 0x1f84);

#endif
