#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

/// @file
/// The routines resident in IWRAM, called at their fixed addresses: the
/// fixed-point multiply and divides, the square root, word fills and copies,
/// the matrix multiply and the picture decoder.

#include "fixed_math.h"
#include "types.h"

/// Fixed addresses of the routines resident in IWRAM.
#define Iwram_ClearWordsEntry                                                  \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size)), 0x03000258,            \
                IwramClearWords)
#define Iwram_FillWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size, u32 value)), 0x03000260, \
                IwramFillWords)
/// (left * right) >> 16 with a 64-bit product: the 16.16 product of two 16.16
/// numbers.
#define Iwram_MulFX                                                            \
  FIXED_ADDRESS((s32 (*)(s32 a, s32 b)), 0x0300021c, IwramMulFX)
/// The 16.16 product of two 16.16 numbers (Iwram_MulFX), written as a call.
static inline FIXED Math_MulFX(FIXED a, FIXED b) { return Iwram_MulFX(a, b); }
/// value / divisor as a 16.16 number: value times 2^30 / divisor, shifted
/// down 14.
#define Iwram_DivFXEntry                                                       \
  FIXED_ADDRESS((s32 (*)(s32 divisor, s32 value)), 0x03000230, IwramDivFX)
/// The square root of value, rounded down; 0 for a negative value.
#define Iwram_SqrtEntry                                                        \
  FIXED_ADDRESS((s32 (*)(s32 value)), 0x030002d4, IwramSqrt)
/// Paints value into destination's 16-pixel rows of bytes wherever a glyph's
/// rows of bits, a halfword each, are set, as a glyph's shadow is drawn: twelve
/// rows, fourteen outside the Japanese edition.
#define Iwram_ExpandBitRuns                                                    \
  FIXED_ADDRESS((s32 (*)(const void *bits, void *destination, s32 value)),     \
                0x03000314, IwramExpandBitRuns)
/// Multiplies the current matrix by matrix.
#define Iwram_MultiplyMatrixEntry                                              \
  FIXED_ADDRESS((void (*)(const FIXED *matrix)), 0x030003c8,                   \
                IwramMultiplyMatrix)
/// Reads all three word-aligned coordinates before writing the result. Input
/// may be an array or a record's coordinate prefix; output may overlap it.
#define Iwram_TransformVectorEntry                                             \
  FIXED_ADDRESS((s32 (*)(const void *vector, s32 *result)), 0x03000354,        \
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

/// value * FIXED_ONE / divisor: the 16.16 ratio of two 16.16 numbers.
static inline FIXED Iwram_DivFX(FIXED divisor, FIXED value) {
  return Iwram_DivFXEntry(divisor, value);
}

/// The integer square root, rounded down, of a value from 0; 0 for a
/// negative one. A 16.16 square comes back as 8.8.
static inline s32 Iwram_Sqrt(s32 value) { return Iwram_SqrtEntry(value); }

/// Multiplies matrix, a 3 by 3 matrix and a translation in 16.16 fixed point,
/// into the current transform.
static inline void Iwram_MultiplyMatrix(const FIXED *matrix) {
  Iwram_MultiplyMatrixEntry(matrix);
}

/// Writes to result the 16.16 point vector moved by the current matrix: turned
/// by its 3 by 3 part, then offset by its translation.
static inline s32 Iwram_TransformVector(const void *vector, s32 *result) {
  return Iwram_TransformVectorEntry(vector, result);
}

/// The quotient, rounded toward zero.
static inline s32 Iwram_SignedDivide(s32 dividend, s32 divisor) {
  return Iwram_SignedDivideEntry(dividend, divisor);
}

/// The unsigned quotient, rounded down.
static inline u32 Iwram_UnsignedDivide(u32 dividend, u32 divisor) {
  return Iwram_UnsignedDivideEntry(dividend, divisor);
}

/// Unpacks a PICTURE_FORMAT_COMPRESSED picture into destination, a byte above
/// 223 leaving that many less 223 bytes there untouched, and returns NULL; one
/// whose first halfword is 0 is not packed, and comes back as the data after
/// that halfword for the caller to decode.
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

/// Iwram_FillWords as a statement of its own, as twelve fills in the battle,
/// its effects and the field's Psynergy are written: among them the battle
/// camera's and the transition's curtain tiles, which The Broken Seal fills by
/// DMA with Dma_Fill32Cell.
#define IWRAM_FILL_WORDS(destination, size, value)                             \
  do {                                                                         \
    Iwram_FillWords(destination, size, value);                                 \
  } while (0)

/// Iwram_CopyWords as a statement of its own, as IWRAM_FILL_WORDS is: the link
/// code copies the Djinn recovery table into its send buffer so, in the lobby
/// and in the battle, as The Broken Seal's does.
#define IWRAM_COPY_WORDS(destination, source, size)                            \
  do {                                                                         \
    Iwram_CopyWords(destination, source, size);                                \
  } while (0)

/// Iwram_CopyWords's offset from IwramIrqMain, for the callers that reach it
/// from the linked IWRAM code rather than at its fixed address.
#define Iwram_CopyWordsOffset FIXED_OFFSET(1584, IwramCopyWords, IwramIrqMain)

/// Iwram_MulFX's offset from IwramIrqMain, for the callers that reach it
/// from the linked IWRAM code rather than at its fixed address.
#define Iwram_MulFXOffset FIXED_OFFSET(284, IwramMulFX, IwramIrqMain)

/// The IWRAM routine behind Iwram_MulFX, called by its linked name.
FIXED IwramMulFX(FIXED a, FIXED b) __attribute__((long_call));

#endif
