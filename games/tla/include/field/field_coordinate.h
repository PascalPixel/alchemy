#ifndef ALCHEMY_FIELD_COORDINATE_H
#define ALCHEMY_FIELD_COORDINATE_H

/* @file */
/* FIELD_COORDINATE, a position on the field. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* A field coordinate, read whole or as its fraction and its whole pixels. */
/* The halves are a zero-length array over the whole, not a union: a union */
/* member may alias anything, and the game's code reads coordinates as plain */
/* words. */
typedef struct FIELD_COORDINATE {
	struct {
		u16 decimal; /* 1/65536 pixels */
		s16 integer;
	} part[0];
	FIXED fixed; /* 16.16 fixed-point pixels */
} FIELD_COORDINATE;

STATIC_ASSERT(sizeof(FIELD_COORDINATE) == 4);
STATIC_ASSERT(offsetof(FIELD_COORDINATE, part[0].integer) == 2);

/* A map grid cell's side: each cell is this many pixels square. */
#define MAPGRID_CELL_PIXELS 16
/* A whole pixel coordinate's map cell, in pixels: the pixel coordinate of the
 */
/* cell's first pixel. */
#define MAPGRID_CELL_PIXEL_MASK 0xfff0
/* A 16.16 coordinate's whole cell. */
#define FIELD_CELL_MASK 0xfff00000
/* A 16.16 coordinate's shift to its map grid cell: 16 bits of fraction and 4 */
/* of the cell's MAPGRID_CELL_PIXELS. */
#define FIELD_CELL_SHIFT 20
/* The map grid cell a 16.16 coordinate lies in, counted from 0 along its */
/* axis. */
#define FIELD_CELL(coordinate) ((coordinate) >> FIELD_CELL_SHIFT)
/* The half cell, 8 pixels, a 16.16 coordinate lies in, counted from 0 along */
/* its axis. */
#define FIELD_HALF_CELL(coordinate) ((coordinate) >> (FIELD_CELL_SHIFT - 1))
/* The 16.16 coordinate of a half cell's first pixel, FIELD_HALF_CELL undone. */
#define FIELD_HALF_CELL_ORIGIN(halfCell) ((halfCell) << (FIELD_CELL_SHIFT - 1))
/* A 16.16 coordinate's whole pixels, its fraction cleared. */
#define FIELD_PIXEL_MASK 0xffff0000
/* The 16.16 coordinate of a map grid cell's first pixel, FIELD_CELL undone. */
#define FIELD_CELL_ORIGIN(cell) ((cell) << FIELD_CELL_SHIFT)
/* The 16.16 coordinate of the middle of a map grid cell. */
#define FIELD_CELL_CENTER(cell)                                                \
	(FIELD_CELL_ORIGIN(cell) + toFIXED(MAPGRID_CELL_PIXELS / 2))

#endif
