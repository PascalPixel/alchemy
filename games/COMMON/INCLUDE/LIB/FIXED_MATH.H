#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

#include "TYPES.H"

/* One full turn is 0x10000. */

/* Sine-table lookups in LIB/TRIG.S; results are Q16. */
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/* Resident IWRAM division routines. */
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#endif
