#ifndef ALCHEMY_FIXED_MATH_H
#define ALCHEMY_FIXED_MATH_H

#include "TYPES.H"

/* One full turn is 0x10000, matching the game's GBA-era angle convention. */

/* Sine-table lookups in LIB/TRIG.S; results are Q16. */
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/* The entries into the resident IWRAM divider (SYSTEM/IWRAM_ENTRY.S), by the
 * names the compiler calls them. */
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#endif
