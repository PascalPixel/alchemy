#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/// @file
/// MulFX, the 16.16 fixed-point multiply.

#include "fixed_math.h"
#include "types.h"

// Its assembly is in system/asm.h, with the rest.
#define ASM_PART_MUL_FX
#include "system/asm.h"

/// The high word of a times b; by RECIPROCAL_OF(d), a quick a / d.
u32 Math_UnsignedMulHigh(u32 a, u32 b);

/// The reciprocal of `d` as a 32-bit fraction, rounded up: the high word of
/// v times it is v / d, for the small v divided here.
#define RECIPROCAL_OF(d) (0xffffffffU / (d) + 1)

#endif
