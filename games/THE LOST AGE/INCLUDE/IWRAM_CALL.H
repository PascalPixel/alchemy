#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "TYPES.H"

/* The resident runtime's entries that ⚓️'s ROM code calls. They run in
 * IWRAM, beyond a ROM bl's reach, and ROM code reaches them as Camelot's
 * did: through their fixed entry addresses, loaded before the arguments.
 * The build checks each entry against the routine named beside it. */
#define Iwram_FillWords ((s32 (*)(void *destination, s32 size, u32 value))0x03000260) /* IwramFillWords */
#define Iwram_Sqrt ((s32 (*)(s32 value))0x030002d4) /* IwramSqrt */
#define Iwram_SignedDivide ((s32 (*)(s32 dividend, s32 divisor))0x03000528) /* IwramSignedDivide */
#define Iwram_UnsignedDivide ((u32 (*)(u32 dividend, u32 divisor))0x03000534) /* IwramUnsignedDivide */

#endif
