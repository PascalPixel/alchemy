#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "types.h"
#include "system/call_through_lr.h"

/// Fixed addresses of the routines resident in IWRAM.
#define Iwram_ClearWordsEntry                                                  \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size)), 0x03000258,            \
                IwramClearWords)
#define Iwram_FillWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size, u32 value)), 0x03000260, \
                IwramFillWords)
#define Iwram_MulQ16Entry                                                      \
  FIXED_ADDRESS((s32 (*)(s32 a, s32 b)), 0x0300021c, IwramMulQ16)
#define Iwram_RatioMulQ14Entry                                                 \
  FIXED_ADDRESS((s32 (*)(s32 divisor, s32 value)), 0x03000230, IwramRatioMulQ14)
#define Iwram_SqrtEntry                                                        \
  FIXED_ADDRESS((s32 (*)(s32 value)), 0x030002d4, IwramSqrt)
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

/// This game calls the fixed entries through lr (call_through_lr.h).
#define Iwram_ClearWords(destination, size)                                    \
  Call_ThroughLr2((const void *)Iwram_ClearWordsEntry, (s32)(destination),     \
                  (s32)(size))
#define Iwram_FillWords(destination, size, value)                              \
  Call_ThroughLr3((const void *)Iwram_FillWordsEntry, (s32)(destination),      \
                  (s32)(size), (s32)(value))
#define Iwram_MulQ16(a, b)                                                     \
  Call_ThroughLr2((const void *)Iwram_MulQ16Entry, (a), (b))
#define Iwram_RatioMulQ14(divisor, value)                                      \
  Call_ThroughLr2((const void *)Iwram_RatioMulQ14Entry, (divisor), (value))
#define Iwram_Sqrt(value)                                                      \
  Call_ThroughLr1((const void *)Iwram_SqrtEntry, (value))
#define Iwram_TransformMatrix(matrix)                                          \
  Call_ThroughLr1((const void *)Iwram_TransformMatrixEntry, (s32)(matrix))
#define Iwram_TransformVector(vector, result)                                  \
  Call_ThroughLr2((const void *)Iwram_TransformVectorEntry, (s32)(vector),     \
                  (s32)(result))
#define Iwram_SignedDivide(dividend, divisor)                                  \
  Call_ThroughLr2((const void *)Iwram_SignedDivideEntry, (dividend), (divisor))
#define Iwram_UnsignedDivide(dividend, divisor)                                \
  ((u32)Call_ThroughLr2((const void *)Iwram_UnsignedDivideEntry,               \
                        (s32)(dividend), (s32)(divisor)))
#define Iwram_CopyWords(destination, source, size)                             \
  Call_ThroughLr3((const void *)Iwram_CopyWordsEntry, (s32)(destination),      \
                  (s32)(source), (s32)(size))
#define Iwram_Decompress(source, destination)                                  \
  ((void *)Call_ThroughLr2((const void *)Iwram_DecompressEntry, (s32)(source), \
                           (s32)(destination)))

#endif
