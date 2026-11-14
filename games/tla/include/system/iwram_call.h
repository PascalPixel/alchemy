#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "types.h"

/// Fixed addresses of the routines resident in IWRAM.
#define Iwram_ClearWordsEntry                                                  \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size)), 0x03000258,            \
                IwramClearWords)
#define Iwram_FillWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size, u32 value)), 0x03000260, \
                IwramFillWords)
#define Iwram_MulQ16                                                           \
  FIXED_ADDRESS((s32 (*)(s32 a, s32 b)), 0x0300021c, IwramMulQ16)
/// The 16.16 product of two 16.16 numbers (Iwram_MulQ16), written as a call.
static inline s32 Math_MulQ16(s32 a, s32 b) { return Iwram_MulQ16(a, b); }
#define Iwram_RatioMulQ14Entry                                                 \
  FIXED_ADDRESS((s32 (*)(s32 divisor, s32 value)), 0x03000230, IwramRatioMulQ14)
#define Iwram_SqrtEntry                                                        \
  FIXED_ADDRESS((s32 (*)(s32 value)), 0x030002d4, IwramSqrt)
#define Iwram_ExpandBitRuns                                                    \
  FIXED_ADDRESS((s32 (*)(const void *bits, void *destination, s32 value)),     \
                0x03000314, IwramExpandBitRuns)
#define Iwram_TransformMatrixEntry                                             \
  FIXED_ADDRESS((void (*)(const s32 *matrix)), 0x030003c8, IwramTransformMatrix)
/// Reads all three word-aligned coordinates before writing the result. Input
/// may be an array or a record's coordinate prefix; output may overlap it.
#define Iwram_TransformVectorEntry                                             \
  FIXED_ADDRESS((void (*)(const void *vector, s32 *result)), 0x03000354,       \
                IwramTransformVector)
#define Iwram_SignedDivideEntry                                                \
  FIXED_ADDRESS((s32 (*)(s32 dividend, s32 divisor)), 0x03000528,              \
                IwramSignedDivide)
#define Iwram_UnsignedDivideEntry                                              \
  FIXED_ADDRESS((u32 (*)(u32 dividend, u32 divisor)), 0x03000534,              \
                IwramUnsignedDivide)
#define Iwram_DecompressEntry                                                  \
  FIXED_ADDRESS((void *(*)(const void *source, void *destination)),            \
                0x03000694, IwramDecompress)
#define Iwram_CopyWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, const void *source, s32 size)),    \
                0x03000730, IwramCopyWords)

/// value * Q16_ONE / divisor: the 16.16 ratio of two 16.16 numbers.
static inline s32 Iwram_RatioMulQ14(s32 divisor, s32 value) {
  return Iwram_RatioMulQ14Entry(divisor, value);
}

/// The integer square root, rounded down, of a value from 0; 0 for a
/// negative one. A 16.16 square comes back as 8.8.
static inline s32 Iwram_Sqrt(s32 value) { return Iwram_SqrtEntry(value); }

/// Multiplies matrix, a 3 by 3 matrix and a translation in 16.16 fixed point,
/// into the current transform.
static inline void Iwram_TransformMatrix(const s32 *matrix) {
  Iwram_TransformMatrixEntry(matrix);
}

static inline void Iwram_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
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

/// Clears size bytes, rounded down to whole words, from destination. Returns
/// the address after the last word.
static inline s32 Iwram_ClearWords(void *destination, s32 size) {
  return Iwram_ClearWordsEntry(destination, size);
}

/// As Iwram_ClearWords, filling with value.
static inline s32 Iwram_FillWords(void *destination, s32 size, u32 value) {
  return Iwram_FillWordsEntry(destination, size, value);
}

/// Copies size bytes, rounded down to whole words, from source to
/// destination.
static inline s32 Iwram_CopyWords(void *destination, const void *source,
                                  s32 size) {
  return Iwram_CopyWordsEntry(destination, source, size);
}

/// Iwram_FillWords as a statement of its own, the call in a once-only block,
/// which keeps the fill's entry and destination loaded ahead of its size. The
/// name is ours: twelve fills in the battle, its effects and the field's
/// Psynergy are written so, among them the battle camera's and the
/// transition's curtain tiles, which The Broken Seal fills by DMA with its
/// Dma_Fill32Cell block.
#define IWRAM_FILL_WORDS(destination, size, value)                             \
  do {                                                                         \
    Iwram_FillWords(destination, size, value);                                 \
  } while (0)

/// Iwram_CopyWords's offset from IwramIrqMain, for the callers that reach it
/// from the linked IWRAM code rather than at its fixed address.
#define Iwram_CopyWordsOffset FIXED_OFFSET(0x630, IwramCopyWords, IwramIrqMain)

/// Iwram_MulQ16's offset from IwramIrqMain, for the callers that reach it
/// from the linked IWRAM code rather than at its fixed address.
#define Iwram_MulQ16Offset FIXED_OFFSET(0x11c, IwramMulQ16, IwramIrqMain)

#endif
