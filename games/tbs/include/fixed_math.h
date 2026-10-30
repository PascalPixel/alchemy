#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

/// @file
/// 16.16 fixed-point constants, sine and cosine, and the library division.

#include "types.h"

/// One in the game's signed 16.16 fixed-point format.
#define Q16_ONE (1 << 16)

/// A whole turn in the angles that Trig_Sin, Trig_Cos and the transform's
/// turns take: TRIG_TURN / 4 is a quarter turn.
#define TRIG_TURN (1 << 16)
/// Keeps an angle within one turn: Random16() & TRIG_ANGLE_MASK is a random
/// angle.
#define TRIG_ANGLE_MASK (TRIG_TURN - 1)

/// The sine of `angle`, a whole turn being TRIG_TURN, in 16.16, read from
/// Trig_SineTable, 256 entries for the first quarter turn.
s32 Trig_Sin(s32 angle);
/// The cosine of `angle`, a whole turn being TRIG_TURN, in 16.16; it falls
/// through into Trig_Sin a quarter turn on.
s32 Trig_Cos(s32 angle);

s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#endif
