/// @file
/// Resource decoding and pixel buffer shading, through RAM copies of ARM code.
// Part of system.c.

#include "system_internal.h"

/// Decompresses the halfword LZ stream at `source` to `destination`, with a
/// copy of the ARM decoder on the stack.
void Decompress_Halfwords(const void *source, void *destination) {
  u32 words = (u32)Decompress_HalfwordsCodeSize >> 2;
  u32 routine[words];
  Dma_Set(Decompress_HalfwordLz, routine, DMA_COPY_WORDS(words), REG_DMA3);
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

/// The canvas fades of battle effects, the element glow and the Lucky Wheels. A
/// canvas is a buffer of 8-bit pixels, each a step 0 to 63 of a colour ramp.
/// Each routine copies the canvas to `destination`, the tiles on screen, 16
/// bytes at a time, and shades each group in the canvas as it goes, so what is
/// drawn next lands on a dimmer copy of this frame. `amount` holds the step in
/// each of its four bytes (CANVAS_FILL_WORD).
#define CANVAS_DEFINE_FADE_BY(name, code, size)                                \
  void name(u8 *buffer, u32 amount, u8 *destination, u32 bytes) {              \
    u32 words = (u32)size >> 2;                                                \
    u32 routine[words];                                                        \
    Dma_Set(code, routine, DMA_COPY_WORDS(words), REG_DMA3);                   \
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, destination,     \
                                              bytes);                          \
  }
#define CANVAS_DEFINE_FADE(name, code, size)                                   \
  void name(u8 *buffer, u8 *destination, u32 bytes) {                          \
    u32 words = (u32)size >> 2;                                                \
    u32 routine[words];                                                        \
    Dma_Set(code, routine, DMA_COPY_WORDS(words), REG_DMA3);                   \
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, destination, bytes);          \
  }

/// Raises every pixel in the canvas by `amount`, stopping at 63.
CANVAS_DEFINE_FADE_BY(Canvas_CopyAndBrighten, Canvas_BrightenArm,
                      Canvas_CopyAndBrightenCodeSize)
/// Lowers every pixel in the canvas by `amount`, stopping at 0.
CANVAS_DEFINE_FADE_BY(Canvas_CopyAndDarken, Canvas_DarkenArm,
                      Canvas_CopyAndDarkenCodeSize)
/// Scales every pixel to three quarters.
CANVAS_DEFINE_FADE(Canvas_CopyAndScaleThreeQuarters,
                   Canvas_ScaleThreeQuartersArm,
                   Canvas_CopyAndScaleThreeQuartersCodeSize)
/// Halves every pixel.
CANVAS_DEFINE_FADE(Canvas_CopyAndHalve, Canvas_HalveArm,
                   Canvas_CopyAndHalveCodeSize)
/// As Canvas_CopyAndBrighten, skipping groups already all at 63.
CANVAS_DEFINE_FADE_BY(Canvas_CopyAndBrightenPartial, Canvas_BrightenPartialArm,
                      Canvas_CopyAndBrightenPartialCodeSize)
/// As Canvas_CopyAndDarken, skipping groups already all at 0.
CANVAS_DEFINE_FADE_BY(Canvas_CopyAndDarkenPartial, Canvas_DarkenPartialArm,
                      Canvas_CopyAndDarkenPartialCodeSize)
/// Scales every pixel to three quarters, skipping all-zero groups.
CANVAS_DEFINE_FADE(Canvas_CopyAndScaleNonzeroThreeQuarters,
                   Canvas_ScaleNonzeroThreeQuartersArm,
                   Canvas_CopyAndScaleNonzeroThreeQuartersCodeSize)
/// Halves every pixel, skipping all-zero groups.
CANVAS_DEFINE_FADE(Canvas_CopyAndHalveNonzero, Canvas_HalveNonzeroArm,
                   Canvas_CopyAndHalveNonzeroCodeSize)

#undef RESOURCE_DEFINE_DECODE
#undef CANVAS_DEFINE_FADE_BY
#undef CANVAS_DEFINE_FADE
