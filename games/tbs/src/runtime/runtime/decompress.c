/// @file
/// Part of runtime.c, the runtime object, compiled only through it.
/// Resource decoding and pixel buffer shading, through RAM copies of ARM code.

#include "../runtime_internal.h"

/// Decompresses the halfword LZ stream at `source` to `destination`, with a
/// copy of the ARM decoder on the stack.
void Resource_DecompressHalfwords(const void *source, void *destination) {
  u32 words = (u32)Resource_DecompressHalfwordsCodeSize >> 2;
  u32 routine[words];
  Dma_Set(Resource_DecodeHalfwordLz, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(const void *, void *))routine)(source, destination);
}

/// Each decoder copies its ARM routine `code` to RAM, runs it and frees the
/// copy, and returns the number of bytes it decoded.
#define RESOURCE_DEFINE_DECODE(name, code, bytes)                              \
  s32 name(const void *source, void *destination) {                            \
    s32 (*routine)(const void *, void *);                                      \
    s32 result;                                                                \
                                                                               \
    RamCode_Load(routine, code, bytes);                                        \
    result = routine(source, destination);                                     \
    MemFree(routine);                                                          \
    return result;                                                             \
  }

/// Decodes a type-01 resource and returns its unpacked byte count.
RESOURCE_DEFINE_DECODE(Resource_DecodeType01, Resource_DecodeType01Arm,
                       Resource_DecodeType01CodeSize)
/// Decodes a type-02 resource and returns its unpacked byte count.
RESOURCE_DEFINE_DECODE(Resource_DecodeType2, Resource_Decode2,
                       Resource_Decode2CodeSize)
/// Decodes a byte-oriented LZ resource and returns its unpacked byte count.
RESOURCE_DEFINE_DECODE(Resource_DecodeByteLz, Resource_DecodeByteLzArm,
                       Resource_DecodeByteLzCodeSize)

// Shading of 8-bit pixel buffers whose values are steps 0 to 63 of a colour
// ramp. Each function copies `bytes` of `buffer` to `backup`, then shades the
// buffer in place with a stack copy of its ARM kernel. `amount` is added to
// every byte of the four-byte groups at once, so it holds the step in each
// byte. The Nonzero and Partial forms only write back four-byte groups that
// can change: not all zero, or not all at the limit already.

/// Raises every pixel by `amount`, stopping at 63.
void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup,
                                   u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_BrightenKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// Lowers every pixel by `amount`, stopping at 0.
void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup,
                                 u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_DarkenKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// Scales every pixel to three quarters.
void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup,
                                             u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_ScaleThreeQuartersKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/// Halves every pixel.
void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_HalveKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/// As ColorBuffer_BackupAndBrighten, skipping groups already all at 63.
void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup,
                                          u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_BrightenPartialKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// As ColorBuffer_BackupAndDarken, skipping groups already all at 0.
void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup,
                                        u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_DarkenPartialKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// Scales every pixel to three quarters, skipping all-zero groups.
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup,
                                                    u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_ScaleNonzeroThreeQuartersKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/// Halves every pixel, skipping all-zero groups.
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_HalveNonzeroKernel, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | words, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

#undef RESOURCE_DEFINE_DECODE
