#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

/// @file
/// The IWRAM call layer: the routines the game keeps resident in IWRAM, each at
/// its fixed address, and one typed inline per routine that every call goes
/// through.
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device. Code writes `Iwram_Sqrt(value)` or
/// `Iwram_SignedDivide(dividend, divisor)`, and the inline calls the routine at
/// its fixed address.

#include "types.h"

// Fixed addresses of IWRAM-resident routines. System_InstallIwramAndIrqs
// copies the resident routines to 0x03000000; C calls its routines at these
// numbers, and ARM code running from RAM reaches them as offsets from
// IwramIrqMain (IWRAM_ENTRY), so they cannot move.
#define Iwram_RatioMulQ14Entry                                                 \
  FIXED_ADDRESS((s32 (*)(s32 divisor, s32 value)), 0x0300013c, IwramRatioMulQ14)
#define Iwram_ClearWordsEntry                                                  \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size)), 0x03000164,            \
                IwramClearWords)
#define Iwram_FillWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size, u32 value)), 0x03000168, \
                IwramFillWords)
#define Iwram_SqrtEntry                                                        \
  FIXED_ADDRESS((s32 (*)(s32 value)), 0x030001d8, IwramSqrt)
#define Iwram_ExpandBitRuns                                                    \
  FIXED_ADDRESS((s32 (*)(const void *bits, void *destination, s32 value)),     \
                0x03000214, IwramExpandBitRuns)
/// Reads all three word-aligned coordinates before writing the result. Input
/// may be an array or a record's coordinate prefix; output may overlap it.
#define Iwram_TransformVectorEntry                                             \
  FIXED_ADDRESS((s32 (*)(const void *vector, s32 *result)), 0x03000250,        \
                IwramTransformVector)
#define Iwram_TransformMatrixEntry                                             \
  FIXED_ADDRESS((void (*)(const s32 *matrix)), 0x030002c0, IwramTransformMatrix)
#define Iwram_SignedDivideEntry                                                \
  FIXED_ADDRESS((s32 (*)(s32 dividend, s32 divisor)), 0x03000380,              \
                IwramSignedDivide)
#define Iwram_UnsignedDivideEntry                                              \
  FIXED_ADDRESS((u32 (*)(u32 dividend, u32 divisor)), 0x030003f0,              \
                IwramUnsignedDivide)
#define Iwram_DecompressEntry                                                  \
  FIXED_ADDRESS((void *(*)(const void *source, void *destination)),            \
                0x030005c0, IwramDecompress)
#define Iwram_CopyWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, const void *source, s32 size)),    \
                0x03001388, IwramCopyWords)

/// value * 0x10000 / divisor: the 16.16 ratio of two 16.16 numbers. It is
/// value times 0x40000000 / divisor, shifted down 14 from a 64-bit product,
/// so that quotient is truncated toward zero first and a large divisor costs
/// precision.
static inline s32 Iwram_RatioMulQ14(s32 divisor, s32 value) {
  return Iwram_RatioMulQ14Entry(divisor, value);
}

/// Clears size bytes, rounded down to whole words, from destination. Returns
/// the address after the last word.
static inline s32 Iwram_ClearWords(void *destination, s32 size) {
  return Iwram_ClearWordsEntry(destination, size);
}

/// As Iwram_ClearWords, filling with value.
static inline s32 Iwram_FillWords(void *destination, s32 size, u32 value) {
  return Iwram_FillWordsEntry(destination, size, value);
}

/// The integer square root, rounded down, of a value from 0; 0 for a
/// negative one. A 16.16 square comes back as 8.8.
static inline s32 Iwram_Sqrt(s32 value) { return Iwram_SqrtEntry(value); }

static inline s32 Iwram_TransformVector(const void *vector, s32 *result) {
  return Iwram_TransformVectorEntry(vector, result);
}

/// Multiplies matrix, a 3 by 3 matrix and a translation in 16.16 fixed point,
/// into the current transform, gTransform.
static inline void Iwram_TransformMatrix(const s32 *matrix) {
  Iwram_TransformMatrixEntry(matrix);
}

/// The quotient, rounded toward zero.
static inline s32 Iwram_SignedDivide(s32 dividend, s32 divisor) {
  return Iwram_SignedDivideEntry(dividend, divisor);
}

static inline u32 Iwram_UnsignedDivide(u32 dividend, u32 divisor) {
  return Iwram_UnsignedDivideEntry(dividend, divisor);
}

static inline void *Iwram_Decompress(const void *source, void *destination) {
  return Iwram_DecompressEntry(source, destination);
}

/// Copies size bytes, rounded down to whole words, from source to
/// destination.
static inline s32 Iwram_CopyWords(volatile void *destination,
                                  const volatile void *source, s32 size) {
  return Iwram_CopyWordsEntry((void *)destination, (const void *)source, size);
}

#ifdef MODERN
/// Calls an IWRAM routine that returns through ip instead of lr, with left and
/// right in r0 and r1, and returns its r0.
CAMELOT_ASM(
    "a call that returns through ip, which no procedure-call standard produces")
static inline s32 Iwram_CallReturnIp(s32 left, s32 right, void *routine) {
  s32 result;

  __asm__ volatile("mov r0, %1\n\t"
                   "mov r1, %2\n\t"
                   ".align 2\n\t"
                   "mov ip, pc\n\t"
                   "bx %3\n\t"
                   "mov %0, r0"
                   : "=r"(result)
                   : "r"(left), "r"(right), "r"(routine)
                   : "r0", "r1", "r2", "ip", "cc");
  return result;
}
#else
CAMELOT_ASM(
    "a call that returns through ip, which no procedure-call standard produces")
static __inline__ s32 Iwram_CallReturnIp(s32 left, s32 right, void *routine) {
  register s32 result __asm__("r0") = left;
  register s32 factor __asm__("r1") = right;

  __asm__ volatile(".align 2\n\t"
                   "mov ip, pc\n\t"
                   "bx %2\n\t"
                   : "+r"(result)
                   : "r"(factor), "r"(routine)
                   : "r1", "r2", "ip", "cc");
  return result;
}
#endif

extern u8 IwramMulQ16ReturnIp[];

/// (left * right) >> 16 with a 64-bit product: the 16.16 product of two 16.16
/// numbers.
#define Iwram_MulQ16(left, right)                                              \
  Iwram_CallReturnIp((left), (right), IwramMulQ16ReturnIp)

#define Iwram_CopyWordsOffset FIXED_OFFSET(0x1388, IwramCopyWords, IwramIrqMain)
#define Iwram_RatioMulQ14Offset                                                \
  FIXED_OFFSET(0x13c, IwramRatioMulQ14, IwramIrqMain)
#define Iwram_SqrtOffset FIXED_OFFSET(0x1d8, IwramSqrt, IwramIrqMain)

#endif
