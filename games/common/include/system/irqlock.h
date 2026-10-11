#ifndef ALCHEMY_IRQLOCK_H
#define ALCHEMY_IRQLOCK_H

#include "io_reg.h"
#include "types.h"

// 割り込みを止めて、元の IME を saved に入れる。IRQ_UNLOCK で元に戻す。
// IME にはレジスタ自身のアドレスの下位 16 ビット（0x0208、ビット 0 は 0）が書かれる
/* FAKEMATCH: the blocks that run once are meaningless: one around the read of the old value, one around the guarded code. Without the first, 23 objects with queue pushes differ; without the second, the callback scheduler and Runtime_SetIrqHandler differ; a restore through the same pointer changes 16 objects. A constant 0x208 store loads it from the literal pool where the ROM stores the address register (strh r3, [r3]). One text for 14 lock sites (eight in the callback scheduler, Runtime_SetIrqHandler, SerialRuntime_Initialize, BeginTransferA and BeginTransferB, LinkLobby_InitializeSerial, the battle link's seed copy) and, through IO_WRITE_QUEUE_PUSH, every write-queue push but five. Not covered: BattlePresentation_SetPaletteLevel, which keeps the old value in a stack slot, and the five pushes named at IO_WRITE_QUEUE_PUSH. ⚓️ uses the same text at its two Disable sites in the callback scheduler (disable_callbacks.c), where it replaced two asm barriers (2026-10-11). */
#define IRQ_LOCK(saved)         \
	do                          \
	{                           \
		volatile u16 *ime_;     \
                                \
		do                      \
		{                       \
			ime_ = &REG_IME;    \
			saved = *ime_;      \
		} while (0);            \
		*ime_ = (u16)(u32)ime_; \
		do                      \
		{

#define IRQ_UNLOCK(saved) \
	}                     \
	while (0)             \
		;                 \
	REG_IME = saved;      \
	}                     \
	while (0)

#endif
