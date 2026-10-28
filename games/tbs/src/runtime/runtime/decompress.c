/// @file
/// Resource decoding and pixel buffer shading, through RAM copies of ARM code.
// Part of runtime.c.

#include "../runtime_internal.h"

/// Decompresses the halfword LZ stream at `source` to `destination`, with a
/// copy of the ARM decoder on the stack. Called by the palette editor.
void Decompress_Halfwords(const void *source, void *destination) {
  u32 words = (u32)Decompress_HalfwordsCodeSize >> 2;
  u32 routine[words];
  Dma_Set(Decompress_HalfwordLz, routine, DMA_COPY_WORDS(words), REG_DMA3);
  ((void (*)(const void *, void *))routine)(source, destination);
}

/// Each decoder copies its ARM routine `code` to RAM, runs it and frees the
/// copy, and returns the number of bytes it decoded. The copy is a Heap_Alloc
/// block, IWRAM when it fits, and Heap_Release puts the pool's next address
/// back where it was.
#define RESOURCE_DEFINE_DECODE(name, code, bytes)                              \
  s32 name(const void *source, void *destination) {                            \
    s32 (*routine)(const void *, void *);                                      \
    s32 result;                                                                \
                                                                               \
    RamCode_Load(routine, code, bytes);                                        \
    result = routine(source, destination);                                     \
    Heap_Release(routine);                                                     \
    return result;                                                             \
  }

/// Decodes a tagged LZ resource (a first byte of 0 for the general form, 1 for
/// the palette form) and returns its unpacked byte count.
RESOURCE_DEFINE_DECODE(Decompress_TaggedLz, Decompress_TaggedLzArm,
                       Decompress_TaggedLzCodeSize)
/// Decodes a move-to-front LZ resource (a first byte of 2) and returns its
/// unpacked byte count.
RESOURCE_DEFINE_DECODE(Decompress_MoveToFrontLz,
                       Decompress_MoveToFrontLzCommands,
                       Decompress_MoveToFrontLzCommandsCodeSize)
/// Decodes a byte-oriented LZ resource and returns its unpacked byte count.
RESOURCE_DEFINE_DECODE(Decompress_ByteLz, Decompress_ByteLzArm,
                       Decompress_ByteLzCodeSize)

// Shading of 8-bit pixel buffers whose values are steps 0 to 63 of a colour
// ramp. Each function copies `bytes` of `buffer` to `backup`, then shades the
// buffer in place with a stack copy of its ARM kernel. `amount` is added to
// every byte of the four-byte groups at once, so it holds the step in each
// byte. The Nonzero and Partial forms only write back four-byte groups that
// can change: not all zero, or not all at the limit already.

/// Raises every pixel by `amount`, stopping at 63.
void PaletteFade_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup,
                                   u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndBrightenCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_BrightenKernel, routine, DMA_COPY_WORDS(words), REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// Lowers every pixel by `amount`, stopping at 0.
void PaletteFade_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup,
                                 u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndDarkenCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_DarkenKernel, routine, DMA_COPY_WORDS(words), REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// Scales every pixel to three quarters.
void PaletteFade_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup,
                                             u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndScaleThreeQuartersCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_ScaleThreeQuartersKernel, routine, DMA_COPY_WORDS(words),
          REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/// Halves every pixel. Called by the battle animation canvas's transfers.
void PaletteFade_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndHalveCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_HalveKernel, routine, DMA_COPY_WORDS(words), REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/// As PaletteFade_BackupAndBrighten, skipping groups already all at 63.
void PaletteFade_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup,
                                          u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndBrightenPartialCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_BrightenPartialKernel, routine, DMA_COPY_WORDS(words),
          REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// As PaletteFade_BackupAndDarken, skipping groups already all at 0.
void PaletteFade_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup,
                                        u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndDarkenPartialCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_DarkenPartialKernel, routine, DMA_COPY_WORDS(words),
          REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/// Scales every pixel to three quarters, skipping all-zero groups.
void PaletteFade_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup,
                                                    u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_ScaleNonzeroThreeQuartersKernel, routine,
          DMA_COPY_WORDS(words), REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/// Halves every pixel, skipping all-zero groups. Called by the Lucky Wheels
/// wheels' canvas transfer.
void PaletteFade_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes) {
  u32 words = (u32)PaletteFade_BackupAndHalveNonzeroCodeSize >> 2;
  u32 routine[words];
  Dma_Set(PaletteFade_HalveNonzeroKernel, routine, DMA_COPY_WORDS(words),
          REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

#undef RESOURCE_DEFINE_DECODE
