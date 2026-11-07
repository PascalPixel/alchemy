#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

#include "types.h"

/// One in the game's signed 16.16 fixed-point format.
#define Q16_ONE (1 << 16)

/// A whole turn in the angles that Trig_Sin and Trig_Cos take: TRIG_TURN / 4
/// is a quarter turn.
#define TRIG_TURN (1 << 16)

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#endif
