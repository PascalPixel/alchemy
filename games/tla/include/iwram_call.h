#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "types.h"

// IWRAM に常駐するルーチンの固定アドレス
#define Iwram_ClearWordsEntry                                                  \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size)), 0x03000258,            \
                IwramClearWords)
#define Iwram_FillWordsEntry                                                   \
  FIXED_ADDRESS((s32 (*)(void *destination, s32 size, u32 value)), 0x03000260, \
                IwramFillWords)
#define Iwram_MulQ16                                                           \
  FIXED_ADDRESS((s32 (*)(s32 a, s32 b)), 0x0300021c, IwramMulQ16)
#define Iwram_RatioMulQ14Entry                                                 \
  FIXED_ADDRESS((s32 (*)(s32 divisor, s32 value)), 0x03000230, IwramRatioMulQ14)
#define Iwram_SqrtEntry                                                        \
  FIXED_ADDRESS((s32 (*)(s32 value)), 0x030002d4, IwramSqrt)
#define Iwram_TransformMatrixEntry                                             \
  FIXED_ADDRESS((void (*)(const s32 *matrix)), 0x030003c8, IwramTransformMatrix)
#define Iwram_TransformVectorEntry                                             \
  FIXED_ADDRESS((void (*)(const s32 *vector, s32 *result)), 0x03000354,        \
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

// このゲームでは固定アドレスを直接呼ぶ
#define Iwram_ClearWords Iwram_ClearWordsEntry
#define Iwram_FillWords Iwram_FillWordsEntry
#define Iwram_RatioMulQ14 Iwram_RatioMulQ14Entry
#define Iwram_Sqrt Iwram_SqrtEntry
#define Iwram_TransformMatrix Iwram_TransformMatrixEntry
#define Iwram_TransformVector Iwram_TransformVectorEntry
#define Iwram_SignedDivide Iwram_SignedDivideEntry
#define Iwram_UnsignedDivide Iwram_UnsignedDivideEntry
#define Iwram_CopyWords Iwram_CopyWordsEntry
#define Iwram_Decompress Iwram_DecompressEntry

#endif
