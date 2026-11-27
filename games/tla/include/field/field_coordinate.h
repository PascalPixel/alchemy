#ifndef ALCHEMY_FIELD_COORDINATE_H
#define ALCHEMY_FIELD_COORDINATE_H

// FIELD_COORDINATE, a position on the field.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

// A field coordinate, read whole or as its fraction and its whole pixels. The halves are a
// zero-length array over the whole, not a union: a union member may alias anything, and the game's
// code reads coordinates as plain words.
typedef struct FIELD_COORDINATE
{
	struct
	{
		Uint16 decimal; // 1/65536 pixels
		Sint16 integer;
	} part[0];

	FIXED fixed; // 16.16 fixed-point pixels
} FIELD_COORDINATE;

STATIC_ASSERT(sizeof(FIELD_COORDINATE) == 4);
STATIC_ASSERT(offsetof(FIELD_COORDINATE, part[0].integer) == 2);

// A map grid metatile's side: each metatile is this many pixels square.
#define MAPGRID_METATILE_PIXELS                  16
// A whole pixel coordinate's map metatile, in pixels: the pixel coordinate of the metatile's first
// pixel.
#define MAPGRID_METATILE_PIXEL_MASK              0xfff0
// A 16.16 coordinate's whole metatile.
#define FIELD_METATILE_MASK                      0xfff00000
// A 16.16 coordinate's shift to its map grid metatile: 16 bits of fraction and 4 of the metatile's
// MAPGRID_METATILE_PIXELS.
#define FIELD_METATILE_SHIFT                     20
// The map grid metatile a 16.16 coordinate lies in, counted from 0 along its axis.
#define FIELD_METATILE(coordinate)               ((coordinate) >> FIELD_METATILE_SHIFT)
// The half metatile, 8 pixels, a 16.16 coordinate lies in, counted from 0 along its axis.
#define FIELD_HALF_METATILE(coordinate)          ((coordinate) >> (FIELD_METATILE_SHIFT - 1))
// The 16.16 coordinate of a half metatile's first pixel, FIELD_HALF_METATILE undone.
#define FIELD_HALF_METATILE_ORIGIN(halfMetatile) ((halfMetatile) << (FIELD_METATILE_SHIFT - 1))
// A 16.16 coordinate's whole pixels, its fraction cleared.
#define FIELD_PIXEL_MASK                         0xffff0000
// The 16.16 coordinate of a map grid metatile's first pixel, FIELD_METATILE undone.
#define FIELD_METATILE_ORIGIN(metatile)          ((metatile) << FIELD_METATILE_SHIFT)
// The 16.16 coordinate of the middle of a map grid metatile.
#define FIELD_METATILE_CENTER(metatile)                                                            \
	(FIELD_METATILE_ORIGIN(metatile) + toFIXED(MAPGRID_METATILE_PIXELS / 2))

#endif
