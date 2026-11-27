#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

/* @file */
/* The IWRAM call layer: the functions the game keeps resident in IWRAM, each */
/* at its fixed address, and one typed inline per function that every call */
/* goes through. */
/* */
/* Code writes `sySquart(value)` or `syIntegerDivide(dividend, divisor)`, */
/* and the inline calls the function at its fixed address. */
/* */
/* A file of its own, which system.h includes in place, because the units */
/* built as ARM code include it without the rest of system.h: another inline */
/* seen before their functions would renumber their labels. Its types and */
/* work areas are in system/definitions.h and system/globals.h. */

#include "system/definitions.h"
#include "system/globals.h"

/* value * FIXED_ONE / divisor: the 16.16 ratio of two 16.16 numbers. */
static inline FIXED syDivFX(FIXED divisor, FIXED value) {
	return fdFixedDivEntry(divisor, value);
}

/* Clears size bytes, rounded down to whole words, from destination. Returns */
/* the address after the last word. */
static inline s32 syWordsClear(void *destination, s32 size) {
	return syWordsClearEntry(destination, size);
}

/* As syWordsClear, filling with value. */
static inline s32 syWordsFill(void *destination, s32 size, u32 value) {
	return syWordsFillEntry(destination, size, value);
}

/* The integer square root, rounded down, of a value from 0; 0 for a */
/* negative one. A 16.16 square comes back as 8.8. */
static inline s32 sySquart(s32 value) { return syMathSqrtEntry(value); }

/* Transforms a vector of three 16.16 coordinates by CurrentMatrix into */
/* result. */
static inline s32 syCalcPoint(const POINT vector, POINT result) {
	return syVectorTransformEntry(vector, result);
}

/* Multiplies matrix, a 3 by 3 matrix and a translation in 16.16 fixed point, */
/* into the current transform, CurrentMatrix. */
static inline void syMultiMatrix(const MATRIX matrix) {
	syMatrixMultiplyEntry(matrix);
}

/* The quotient, rounded toward zero. */
static inline s32 syIntegerDivide(s32 dividend, s32 divisor) {
	return syIntegerDivideEntry(dividend, divisor);
}

/* The unsigned quotient, rounded down. */
static inline u32 syIntegerDivideUnsigned(u32 dividend, u32 divisor) {
	return syIntegerDivideUnsignedEntry(dividend, divisor);
}

/* Unpacks a sprite's codec 3 picture straight into its pixels at destination,
 */
/* where a byte above 223 skips that many less 223 transparent pixels, and */
/* returns null. A picture whose two-byte packed size is 0 is stored unpacked:
 */
/* it returns the picture after that size, for the caller to draw. */
static inline void *fdDecompressSprPicture(const void *source,
										   void *destination) {
	return fdDecompressSprPictureEntry(source, destination);
}

/* Copies size bytes, rounded down to whole words, from source to */
/* destination. */
static inline void syWordsCopy(volatile void *destination,
							   const volatile void *source, s32 size) {
	syWordsCopyEntry((void *)destination, (const void *)source, size);
}

/* syWordsCopy as a statement of its own: how the link battle and the */
/* Battle Arena copy the Djinn recovery table into the send buffer, as The */
/* Lost Age writes it there and in its IWRAM_FILL_WORDS fills. */
#define IWRAM_COPY_WORDS(destination, source, size)                            \
	do {                                                                       \
		syWordsCopy(destination, source, size);                                \
	} while(0)

#ifdef MODERN
/* Calls an IWRAM function that returns through ip instead of lr, with left and
 */
/* right in r0 and r1, and returns its r0. */
/* FAKEMATCH(asm): handwritten assembly: a call that returns through ip, which
 * no procedure-call standard produces. */
static inline s32 syCallReturnIp(s32 left, s32 right, void *routine) {
	s32 result;

	ASM_CALL_RETURN_IP_MODERN(result, left, right, routine);
	return result;
}
#else
static __inline__ s32 syCallReturnIp(s32 left, s32 right, void *routine) {
	ASM_REGISTER(s32 result, "r0") = left;
	ASM_REGISTER(s32 factor, "r1") = right;

	ASM_CALL_RETURN_IP(result, factor, routine);
	return result;
}
#endif

/* (left * right) >> 16 with a 64-bit product: the 16.16 product of two 16.16 */
/* numbers. */
#define syMulFX(left, right) syCallReturnIp((left), (right), IwramMulFXReturnIp)

#endif
