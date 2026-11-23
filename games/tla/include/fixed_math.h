#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

/// @file
/// 16.16 fixed-point constants and pixels, sine and cosine, and the library
/// division.

#include "types.h"

/// A signed 16.16 fixed-point number, as SGL types one: FIXED_ONE is 1.0.
typedef s32 FIXED;

/// One in the game's signed 16.16 fixed-point format.
#define FIXED_ONE (1 << 16)

/// A damping is a fraction of a speed kept each frame, out of DAMPING_SCALE:
/// speed * damping / DAMPING_SCALE.
#define DAMPING_SCALE 64

/// Whole pixels as 16.16, for arithmetic on a stored position or speed.
#define INTtoFIXED(value) ((value) << 16)
/// The whole pixels of a 16.16 value, toFIXED() undone: an arithmetic shift, so
/// a negative value rounds down.
#define FIXEDtoINT(value) ((value) >> 16)
/// The whole-pixel half of a 16.16 coordinate, read in place as its high
/// halfword.
#define PIXEL_PART(v) (((s16 *)&(v))[1])
/// Twice a 16.16 value, as a whole number: FIXEDtoINT a shift short.
#define FIXEDtoINT_TWICE(value) ((value) >> 15)
/// Four times a 16.16 value, as a whole number: FIXEDtoINT two shifts short.
#define FIXEDtoINT_FOUR_TIMES(value) ((value) >> 14)

/// A whole or decimal number as 16.16, for the macros that take plain pixels
/// and decimal speeds: toFIXED(0.8) is 52428. It multiplies where toFIXED()
/// shifts, so toFIXED(-64) is defined C.
#define toFIXED(value) ((s32)((value) * FIXED_ONE))

/// A whole turn in the angles that Trig_Sin and Trig_Cos take: DEGtoANG(90)
/// is a quarter turn.
#define TRIG_TURN (1 << 16)
/// An angle given in degrees, as SGL writes it: DEGtoANG(90) is a quarter turn.
#define DEGtoANG(degrees) ((degrees) * TRIG_TURN / 360)
/// Keeps an angle within one turn: Random16() & TRIG_ANGLE_MASK is a random
/// angle.
#define TRIG_ANGLE_MASK (TRIG_TURN - 1)

/// The sine of `angle`, a whole turn being TRIG_TURN, in 16.16.
FIXED Trig_Sin(s32 angle);
/// The cosine of `angle`, a whole turn being TRIG_TURN, in 16.16; it falls
/// through into Trig_Sin a quarter turn on.
FIXED Trig_Cos(s32 angle);

s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#endif
