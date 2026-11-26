#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/// @file
/// MulFX, the 16.16 fixed-point multiply.

#include "fixed_math.h"
#include "types.h"

// Its assembly is in system/asm.h, with the rest.
#define ASM_PART_MUL_FX
#include "system/asm.h"

#endif
