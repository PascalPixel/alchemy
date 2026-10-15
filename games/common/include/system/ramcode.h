#ifndef GUARD_RAMCODE_H
#define GUARD_RAMCODE_H

#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "types.h"

// ARM ルーチンを RAM へ写して使う。bytes はルーチンの大きさ (リンカの記号)

// ヒープに場所を取って写す。使い終わったら MemFree
#define RamCode_Load(to, code, bytes)                                             \
    do                                                                            \
    {                                                                             \
        u32 size_;                                                                \
                                                                                  \
        size_ = (u32)(bytes);                                                     \
                                                                                  \
        (to) = MemAlloc(size_);                                                   \
        Dma_Set((const void *)(code), (to), 0x84000000 | (size_ >> 2), REG_DMA3); \
    } while (0)

// 作業スロットに場所を取って写す。使い終わったら Runtime_ReleaseHeapBlock
#define RamCode_LoadSlot(slot, code, bytes)                                     \
    do                                                                          \
    {                                                                           \
        u32 size_;                                                              \
                                                                                \
        size_ = (u32)(bytes);                                                   \
                                                                                \
        Dma_Set((const void *)(code), Runtime_AllocateHeapBlock((slot), size_), \
            0x84000000 | (size_ >> 2), REG_DMA3);                               \
    } while (0)

#endif
