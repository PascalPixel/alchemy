#ifndef ALCHEMY_FIELD_COORDINATE_H
#define ALCHEMY_FIELD_COORDINATE_H

/// @file
/// FieldCoordinate, a position on the field.

#include "types.h"
#include "static_assert.h"

union FieldCoordinate {
  s32 fixed;
  struct {
    u16 fraction;
    s16 pixel;
  } part;
};

STATIC_ASSERT(sizeof(union FieldCoordinate) == 4);
STATIC_ASSERT(offsetof(union FieldCoordinate, part.pixel) == 2);

#endif
