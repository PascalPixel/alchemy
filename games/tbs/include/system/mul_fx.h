#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/* syMulFXArm, the 16.16 fixed-point multiply.

   A file of its own, which system.h includes in place, because the units built
   as ARM code include it without the rest of system.h: another inline seen
   before their functions would renumber their labels. Its types and work areas
   are in system/definitions.h and system/globals.h. */

#include "system/definitions.h"
#include "system/globals.h"

/* Multiplies two 16.16 fixed-point values, returning the middle 32 bits of the
   product: syIWRAMMulFX written out where it is called. */
static __inline__ FIXED syMulFXArm(FIXED left, FIXED right)
{
	Uint32 low;
	Sint32 high;
	Sint32 result;

	ASM_SMULL(high, low, left, right);
	high = INTtoFIXED(high);
	ASM_ORR_SHIFTED(result, high, low);
	return result;
}

#endif
