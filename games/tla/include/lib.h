#ifndef ALCHEMY_LIB_H
#define ALCHEMY_LIB_H

/// @file
/// The arctangent table, the fixed-point square root and the basic colour
/// palette, which The Broken Seal declares in its system headers.
// XXX: which of The Lost Age's system headers declares each, so the name lib.h,
// which says nothing, can go; The Lost Age's module headers for its main
// image's system code (src/system/), settled in its own pass, would settle it.

#include "types.h"
#include "edition.h"

extern const u16 gMath_ArcTanTable[];
extern const u8 gSystem_BasicColorPalette[];
s32 FixedSqrt(s32 value);

#endif
