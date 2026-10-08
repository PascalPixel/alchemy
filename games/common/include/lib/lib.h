#ifndef LIB_H
#define LIB_H

/* LIB */

#include "types.h"
#include "edition.h"

#if GAME_TLA
extern const u16 Math_ArcTanTable[];
extern const u8 System_BasicColorPalette[];
s32 FixedSqrt(s32 value);
#endif

#endif
