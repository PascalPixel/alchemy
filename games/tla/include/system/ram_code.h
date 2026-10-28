#ifndef ALCHEMY_RAM_CODE_H
#define ALCHEMY_RAM_CODE_H

#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"
#include "types.h"

// Copies an ARM routine to RAM for execution. bytes is its linker-defined size.

/// Allocates a heap copy; release it with Heap_Release when finished.
#define RamCode_Load(to, code, bytes)                                          \
  do {                                                                         \
    u32 size_;                                                                 \
                                                                               \
    size_ = (u32)(bytes);                                                      \
                                                                               \
    (to) = Heap_Alloc(size_);                                                  \
    Dma_Set((const void *)(code), (to), DMA_COPY_WORDS(size_ >> 2), REG_DMA3); \
  } while (0)

/// Allocates a heap-slot copy; release it with HeapSlot_Release.
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
