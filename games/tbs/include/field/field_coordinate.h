#ifndef ALCHEMY_FIELD_COORDINATE_H
#define ALCHEMY_FIELD_COORDINATE_H

/// @file
/// FieldCoordinate, a position on the field.

#include "types.h"
#include "static_assert.h"

/// A field coordinate, read whole or as its fraction and its whole pixels.
union FieldCoordinate {
  s32 fixed; ///< 16.16 fixed-point pixels
  struct {
    u16 fraction; ///< 1/65536 pixels
    s16 pixel;    ///< pixels
  } part;
};

STATIC_ASSERT(sizeof(union FieldCoordinate) == 4);
STATIC_ASSERT(offsetof(union FieldCoordinate, part.pixel) == 2);

#endif
