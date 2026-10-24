#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

/// @file
/// 16.16 fixed-point constants, sine and cosine, and the library division.

#include "types.h"

/// One in the engine's signed 16.16 fixed-point format.
#define Q16_ONE (1 << 16)

/// The sine of `angle`, a whole turn being 0x10000, in 16.16, read from
/// Trig_SineTable, 256 entries for the first quarter turn.
s32 Trig_Sin(s32 angle);
/// The cosine of `angle`, a whole turn being 0x10000, in 16.16; it falls
/// through into Trig_Sin a quarter turn on.
s32 Trig_Cos(s32 angle);

s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#endif
