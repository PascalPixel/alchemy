#ifndef ALCHEMY_DMA_H
#define ALCHEMY_DMA_H

/// @file
/// DMA channel registers, and the DMA and CPU fills.

#include "io_reg.h"
#include "types.h"

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
            (DMA_ENABLE | DMA_SRC_FIXED) << 16 | ((size) / 2), REG_DMA3);      \
  }

/// Fills `size` bytes at `destination` with the word `value` by DMA3.
#define Dma_Fill32(value, destination, size)                                   \
  {                                                                            \
    volatile u32 fill_ = (value);                                              \
    Dma_Set((const void *)&fill_, (destination),                               \
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | ((size) / 4),     \
            REG_DMA3);                                                         \
  }

/// Dma_Fill32 through the caller's own volatile word `cell`, so a function that
/// fills several blocks keeps one word for all of them and forms its address
/// once; the word also takes the caller's place in the frame
/// (Map_InitializePerspectiveScene's sp+28, where Dma_Fill32's would be sp+16).
#define Dma_Fill32Cell(cell, value, destination, size)                         \
  {                                                                            \
    (cell) = (value);                                                          \
    Dma_Set((const void *)&(cell), (destination),                              \
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | ((size) / 4),     \
            REG_DMA3);                                                         \
  }

/// pret's CpuFill32: repeats one word through the BIOS CpuSet service.
#define Cpu_Fill32(value, destination, size)                                   \
  {                                                                            \
    volatile u32 fill_ = (value);                                              \
    Bios_CpuSet((const void *)&fill_, (destination),                           \
                CPU_SET_32BIT | CPU_SET_SRC_FIXED | ((size) / 4));             \
  }

/// Starts a DMA transfer: stores the source, destination and control words
/// into `channel`'s registers with one stmia.
CAMELOT_ASM("stmia of three registers with no paired ldmia, which no agscc "
            "pattern emits")
static __inline__ void Dma_Set(const void *source, void *destination,
                               u32 control, volatile u32 *channel) {
  // The channel is an input only, so callers keep it in a register across
  // adjacent transfers and form nearby I/O addresses from it.
  register volatile u32 *dma __asm__("r3") = channel;
  register u32 src __asm__("r0") = (u32)source;
  register u32 dst __asm__("r1") = (u32)destination;
  register u32 cnt __asm__("r2") = control;
  __asm__ volatile("stmia %2!, {%0, %3, %1}\n\tsub %2, #12"
                   : "+r"(src), "+r"(cnt)
                   : "r"(dma), "r"(dst)
                   : "memory", "cc");
}

#endif
