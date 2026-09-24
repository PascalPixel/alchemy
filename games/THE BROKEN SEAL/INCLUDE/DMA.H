#ifndef GUARD_DMA_H
#define GUARD_DMA_H

#include "TYPES.H"

static __inline__ void Dma_Set(const void *source, void *destination, u32 control, volatile u32 *channel)
{
    register volatile u32 *dma __asm__("r3") = channel;
    register u32 src __asm__("r0") = (u32)source;
    register u32 dst __asm__("r1") = (u32)destination;
    register u32 cnt __asm__("r2") = control;
    __asm__ volatile("stmia %2!, {%0, %3, %1}\n\tsub %2, #12"
        : "+r"(src), "+r"(cnt) : "r"(dma), "r"(dst) : "memory", "cc");
}

#endif
