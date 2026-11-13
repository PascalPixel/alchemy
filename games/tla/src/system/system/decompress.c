/// @file
/// Resource decoding, through RAM copies of ARM code.
// Part of system.c.

#include "../system_internal.h"
#include "system/ram_code.h"

void Decompress_Halfwords(const void *source, void *destination) {
  u8 routine[(u32)Resource_DecompressHalfwordsCodeSize];

  Dma_Set((const void *)Resource_DecodeHalfwordLzCode, routine,
          DMA_COPY_WORDS((u32)Resource_DecompressHalfwordsCodeSize >> 2),
          REG_DMA3);
  ((void (*)(const void *, void *))routine)(source, destination);
}

s32 Decompress_TaggedLz(const void *source, void *destination) {
  s32 (*routine)(const void *, void *);
  s32 result;

  RamCode_Load(routine, Resource_Decode01Code, Resource_Decode01CodeSize);
  result = routine(source, destination);
  Heap_Release(routine);
  return result;
}

s32 Resource_DecodeType2(const void *source, void *destination) {
  s32 (*routine)(const void *, void *);
  s32 result;

  RamCode_Load(routine, Resource_Decode2Code, Resource_Decode2CodeSize);
  result = routine(source, destination);
  Heap_Release(routine);
  return result;
}

s32 Resource_DecodeByteLzInRam(const void *source, void *destination) {
  s32 (*routine)(const void *, void *);
  s32 result;

  RamCode_Load(routine, Resource_DecodeByteLzCode,
               Resource_DecodeByteLzCodeSize);
  result = routine(source, destination);
  Heap_Release(routine);
  return result;
}
