#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

#include "TYPES.H"

/* One full turn is 0x10000, matching the game's GBA-era angle convention. */

/* Sine-table lookups in LIB/TRIG.S; results are Q16. */
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/* IWRAM division entries (SYSTEM/DIVISION_ENTRY.S). */
s32 Math_Div(s32 numerator, s32 denominator);
s32 Math_Mod(s32 numerator, s32 denominator);

#endif
