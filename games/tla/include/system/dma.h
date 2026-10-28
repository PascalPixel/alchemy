#ifndef GUARD_DMA_H
#define GUARD_DMA_H

#include "io_reg.h"
#include "types.h"

/// DMA channel registers (DMAxSAD, DMAxDAD, DMAxCNT).
struct DmaRegisters {
  u32 source;
  u32 destination;
  u32 control;
};

#define Dma_Fill16(value, destination, size)                                   \
  {                                                                            \
    volatile u16 fill_ = (value);                                              \
    Dma_Set((const void *)&fill_, (destination),                               \
            (DMA_ENABLE | DMA_SRC_FIXED) << 16 | ((size) / 2), REG_DMA3);      \
  }

#define Dma_Fill32(value, destination, size)                                   \
  {                                                                            \
    volatile u32 fill_ = (value);                                              \
    Dma_Set((const void *)&fill_, (destination),                               \
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | ((size) / 4),     \
            REG_DMA3);                                                         \
  }

/// Dma_Fill32 with the caller's word, a volatile local the function declares
/// at its own scope: the word takes that place in the frame (in
/// Map_InitializePerspectiveScene sp+28, where Dma_Fill32's inner word lands
/// at sp+16), and a function that fills several blocks keeps one word for
/// all of them and forms its address once.
/// The fill word is local to the function. Repeated fills reuse that one word,
/// assigning its value before each transfer.
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

/* Unpinning trials: an ordinary input-only channel stays in r4 across
 * adjacent transfers. A read/write early-clobber channel instead reloads
 * its address at each call. Returning that channel and chaining the first
 * two window transfers matches all 66 instructions in TLA's window loader,
 * but the twelve ROMs differ: other callers also derive nearby I/O addresses
 * from the known DMA channel. System_InstallIwramAndIrqs loses its final
 * channel-to-DISPSTAT subtraction. Those uses must be preserved together;
 * the partial return-and-chain change is not adopted. */
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
