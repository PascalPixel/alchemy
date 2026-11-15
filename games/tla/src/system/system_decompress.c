/// @file
/// Resource decoding, through RAM copies of ARM code.
// Part of system.c.

#include "system_internal.h"
#include "system/ram_code.h"

/// Decompresses the halfword LZ stream at `source` to `destination`, with a
/// copy of the ARM decoder on the stack.
void Decompress_Halfwords(const void *source, void *destination) {
  u8 routine[(u32)Decompress_HalfwordsCodeSize];

  Dma_Set((const void *)Decompress_HalfwordLz, routine,
          DMA_COPY_WORDS((u32)Decompress_HalfwordsCodeSize >> 2), REG_DMA3);
  ((void (*)(const void *, void *))routine)(source, destination);
}

/// Decompresses the tagged LZ data at `source` to `destination` with a heap
/// copy of its ARM decoder, and returns what the decoder returns.
s32 Decompress_TaggedLz(const void *source, void *destination) {
  s32 (*routine)(const void *, void *);
  s32 result;

  RamCode_Load(routine, Decompress_TaggedLzArmCode,
               Decompress_TaggedLzCodeSize);
  result = routine(source, destination);
  Heap_Release(routine);
  return result;
}

/// As Decompress_TaggedLz, with the move-to-front LZ decoder.
s32 Decompress_MoveToFrontLz(const void *source, void *destination) {
  s32 (*routine)(const void *, void *);
  s32 result;

  RamCode_Load(routine, Decompress_MoveToFrontLzCommandsCode,
               Decompress_MoveToFrontLzCommandsCodeSize);
  result = routine(source, destination);
  Heap_Release(routine);
  return result;
}

/// As Decompress_TaggedLz, with the byte LZ decoder.
s32 Decompress_ByteLz(const void *source, void *destination) {
  s32 (*routine)(const void *, void *);
  s32 result;

  RamCode_Load(routine, Resource_DecodeByteLzCode,
               Resource_DecodeByteLzCodeSize);
  result = routine(source, destination);
  Heap_Release(routine);
  return result;
}
