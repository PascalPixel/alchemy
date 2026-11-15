#ifndef ALCHEMY_FIELD_COORDINATE_H
#define ALCHEMY_FIELD_COORDINATE_H

/// @file
/// FieldCoordinate, a position on the field.

#include "static_assert.h"
#include "types.h"

/// A field coordinate, read whole or as its fraction and its whole pixels.
/// The halves are a zero-length array over the whole, not a union: a union
/// member may alias anything, and the game's code reads coordinates as plain
/// words.
struct FieldCoordinate {
  struct {
    u16 fraction; ///< 1/65536 pixels
    s16 pixel;
  } part[0];
  s32 fixed; ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct FieldCoordinate) == 4);
STATIC_ASSERT(offsetof(struct FieldCoordinate, part[0].pixel) == 2);

/// A 16.16 coordinate's whole cell: a map grid cell is 16 pixels square.
#define FIELD_CELL_MASK 0xfff00000
/// The map grid cell a 16.16 coordinate lies in, counted from 0 along its
/// axis.
#define FIELD_CELL(coordinate) ((coordinate) >> 20)
/// The whole pixels of a 16.16 coordinate or distance, px() undone.
#define FIELD_PIXEL(coordinate) ((coordinate) >> 16)
/// A 16.16 coordinate's whole pixels, its fraction cleared.
#define FIELD_PIXEL_MASK 0xffff0000
/// The 16.16 coordinate of a map grid cell's first pixel, FIELD_CELL undone.
#define FIELD_CELL_ORIGIN(cell) ((cell) << 20)
/// The 16.16 coordinate of the middle of a map grid cell.
#define FIELD_CELL_CENTER(cell) (((cell) << 20) + px(8))

#endif
