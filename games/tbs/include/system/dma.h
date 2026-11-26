#ifndef ALCHEMY_DMA_H
#define ALCHEMY_DMA_H

/// @file
/// DMA channel registers, and the DMA and CPU fills.

#include "types.h"
#include "system/io_reg.h"

/// A DMA channel's registers (DMAxSAD, DMAxDAD, DMAxCNT).
struct DmaRegisters {
  u32 source;
  u32 destination;
  u32 control; ///< The count in the low half, DMAxCNT_H's flags above it.
};

/// DMA3, the channel the game copies with, as its three registers.
#define DMA3 ((volatile struct DmaRegisters *)REG_DMA3)

/// Fills `size` bytes at `destination` with the halfword `value` by DMA3, from
/// a volatile copy of it on the stack.
#define Dma_Fill16(value, destination, size)                                   \
  {                                                                            \
    volatile u16 fill_ = (value);                                              \
    Dma_Set((const void *)&fill_, (destination),                               \
            DMA_FILL_HALFWORDS((size) / 2), REG_DMA3);                         \
  }

/// Fills `size` bytes at `destination` with the word `value` by DMA3.
#define Dma_Fill32(value, destination, size)                                   \
  {                                                                            \
    volatile u32 fill_ = (value);                                              \
    Dma_Set((const void *)&fill_, (destination), DMA_FILL_WORDS((size) / 4),   \
            REG_DMA3);                                                         \
  }

/// Dma_Fill32 through the caller's own volatile word `cell`, so a function that
/// fills several blocks keeps one word for all of them and forms its address
/// once.
#define Dma_Fill32Cell(cell, value, destination, size)                         \
  {                                                                            \
    (cell) = (value);                                                          \
    Dma_Set((const void *)&(cell), (destination), DMA_FILL_WORDS((size) / 4),  \
            REG_DMA3);                                                         \
  }

/// pret's CpuFill32: repeats one word through the BIOS CpuSet service.
#define Cpu_Fill32(value, destination, size)                                   \
  {                                                                            \
    volatile u32 fill_ = (value);                                              \
    Bios_CpuSet((const void *)&fill_, (destination),                           \
                CPU_SET_32BIT | CPU_SET_SRC_FIXED | ((size) / 4));             \
  }

/// The halfword of a channel's REG_DMA0-style halfword view that holds its
/// control (DMAxCNT_H): source, destination, count, then control.
#define DMA_CONTROL_HALFWORD 5

/// Stops DMA 0, which a scene may have left repeating each HBlank for a line
/// effect: drops its start timing and repeat, then its enable, and reads the
/// control back.
#define Dma0_Stop()                                                            \
  {                                                                            \
    volatile u16 *dma0_ = REG_DMA0;                                            \
                                                                               \
    dma0_[DMA_CONTROL_HALFWORD] &= ~DMA_START_TIMING_DRQ_REPEAT;               \
    dma0_[DMA_CONTROL_HALFWORD] &= ~DMA_ENABLE;                                \
    dma0_[DMA_CONTROL_HALFWORD];                                               \
  }

#include "system/asm.h"

/// Starts a DMA transfer: stores the source, destination and control words
/// into `channel`'s registers with one stmia.
static __inline__ void Dma_Set(const volatile void *source,
                               volatile void *destination, u32 control,
                               volatile u32 *channel) {
  ASM_REGISTER(volatile u32 *dma, "r3") = channel;
  ASM_REGISTER(u32 src, "r0") = (u32)source;
  ASM_REGISTER(u32 dst, "r1") = (u32)destination;
  ASM_REGISTER(u32 cnt, "r2") = control;
  ASM_DMA_STORE(src, cnt, dma, dst);
}

#endif
