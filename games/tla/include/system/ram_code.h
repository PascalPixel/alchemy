#ifndef ALCHEMY_RAM_CODE_H
#define ALCHEMY_RAM_CODE_H

#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"
#include "types.h"

/// @file
/// Copying an ARM routine to RAM to run it there: the decoders, the overlay
/// loader's branch patcher and the battle effects' canvas routines.

/// Copies the routine `code`, `bytes` long (its linked size), into a block
/// taken with Heap_Alloc and points `to` at it; Heap_Release frees it.
#define RamCode_Load(to, code, bytes)                                          \
  do {                                                                         \
    u32 size_;                                                                 \
                                                                               \
    size_ = (u32)(bytes);                                                      \
                                                                               \
    (to) = Heap_Alloc(size_);                                                  \
    Dma_Set((const void *)(code), (to), DMA_COPY_WORDS(size_ >> 2), REG_DMA3); \
  } while (0)

/// As RamCode_Load, into heap slot `slot`'s block; HeapSlot_Release frees it.
#define RamCode_LoadSlot(slot, code, bytes)                                    \
  do {                                                                         \
    u32 size_;                                                                 \
                                                                               \
    size_ = (u32)(bytes);                                                      \
                                                                               \
    Dma_Set((const void *)(code), HeapSlot_Alloc((slot), size_),               \
            DMA_COPY_WORDS(size_ >> 2), REG_DMA3);                             \
  } while (0)

#endif
