#ifndef GUARD_DMA_H
#define GUARD_DMA_H

#include "types.h"

// DMA チャンネルのレジスタ (DMAxSAD, DMAxDAD, DMAxCNT)
struct DmaRegisters
{
	u32 source;
	u32 destination;
	u32 control;
};

#define Dma_Fill32(value, destination, size)                                    \
	{                                                                           \
		volatile u32 fill_ = (value);                                           \
		Dma_Set((const void *)&fill_, (destination), 0x85000000 | ((size) / 4), \
			(volatile u32 *)0x040000d4);                                        \
	}

/* Unpinning trials: an ordinary input-only channel stays in r4 across
 * adjacent transfers. A read/write early-clobber channel instead reloads
 * its address at each call. Returning that channel and chaining the first
 * two window transfers matches all 66 instructions in TLA's window loader,
 * but the twelve ROMs differ: other callers also derive nearby I/O addresses
 * from the known DMA channel. Runtime_InstallIwramAndIrqs loses its final
 * channel-to-DISPSTAT subtraction. Those uses must be preserved together;
 * the partial return-and-chain change is not adopted. */
CAMELOT_ASM("stmia of three registers with no paired ldmia, which no agscc pattern emits")
static __inline__ void Dma_Set(
	const void *source, void *destination, u32 control, volatile u32 *channel)
{
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
