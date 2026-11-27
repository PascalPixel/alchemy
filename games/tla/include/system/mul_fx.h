#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/* @file */
/* MulFX, the 16.16 fixed-point multiply. */
/* */
/* A file of its own, which system.h includes in place, because the units */
/* built as ARM code include it without the rest of system.h: another inline */
/* seen before their functions would renumber their labels. Its types and */
/* work areas are in system/definitions.h and system/globals.h. */

#include "system/asm.h"
#include "system/definitions.h"
#include "system/globals.h"

/* Multiplies 16.16 fixed-point values, returning the middle 32 product bits: */
/* IwramMulFX's three instructions written in place. */
static __inline__ FIXED MulFX(FIXED left, FIXED right) {
	u32 low;
	s32 high;
	s32 result;

	ASM_SMULL(high, low, left, right);
	high = INTtoFIXED(high);
	ASM_ORR_SHIFTED(result, high, low);
	return result;
}

/* The high word of a times b; by RECIPROCAL_OF(d), a quick a / d. */
u32 syMathUnsignedMulHigh(u32 a, u32 b);

#endif
