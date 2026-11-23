#ifndef ALCHEMY_DMA_H
#define ALCHEMY_DMA_H

/// @file
/// DMA and the BIOS copy service: fills, and one transfer on a channel.

#include "io_reg.h"
#include "types.h"

/// The BIOS CpuSet service (bios.s).
void Bios_CpuSet(const void *source, void *destination, u32 control);

/// DMA channel registers (DMAxSAD, DMAxDAD, DMAxCNT).
struct DmaRegisters {
  u32 source;
  u32 destination;
  u32 control;
};

/// DMA3, the channel the game copies with, as its three registers.
#define DMA3 ((volatile struct DmaRegisters *)&REG_DMA3SAD)

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

/// Dma_Fill32 with the caller's word, a local the function declares at its own
/// scope: the word takes that place in the frame rather than Dma_Fill32's inner
/// one, and a function that fills several blocks keeps one word for all of
/// them, set before each transfer, and forms its address once
/// (ActorSystem_Initialize's two fills).
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

/// Starts a transfer on DMA channel `channel`: its source, destination and
/// control written in one store.
CAMELOT_ASM("stmia of three registers with no paired ldmia, which no agscc "
            "pattern emits")
static __inline__ void Dma_Set(const void *source, void *destination,
                               u32 control, volatile u32 *channel) {
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
