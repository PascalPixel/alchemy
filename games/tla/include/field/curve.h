#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

/* @file */
/* The ground height of a map cell from its shape's height samples, one */
/* function for each kind of slope, and the tables they read. */

#include "fixed_math.h"
#include "types.h"

extern u8 Terrain_RoundedSquareMask[];
/* The sample fdTerrainGetSampleTableHeight reads at each position: 0, 1 or 2,
 */
/* split along two diagonals. */
extern u8 Terrain_SampleIndexTable[];

enum {
	/* Turns a height sample, in 8-pixel units, into 16.16 fixed-point pixels.
	 */
	CURVE_VALUE_SHIFT = 19,
	CURVE_HALF_STEPS = 8,  /* pixels to the middle of a 16-pixel cell */
	CURVE_FULL_STEPS = 16, /* pixels across a cell */
};

FIXED fdTerrainGetFlatHeight(const s8 *samples);
FIXED fdTerrainGetSlopeXHeight(const s8 *samples, s32 position);
FIXED fdTerrainGetSlopeYHeight(const s8 *samples, s32 unused, s32 position);
FIXED fdTerrainGetStepSumDiagonalHeight(const s8 *samples, s32 start, s32 end);
FIXED fdTerrainGetSampleTableHeight(const s8 *samples, u32 position, u32 row);

#endif
