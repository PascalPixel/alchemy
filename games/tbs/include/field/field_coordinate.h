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

#endif
