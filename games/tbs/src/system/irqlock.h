#ifndef ALCHEMY_TBS_IRQLOCK_H
#define ALCHEMY_TBS_IRQLOCK_H

#include "io_reg.h"
#include "types.h"

// 割り込みを止めて、元の IME を saved に入れる。IRQ_UNLOCK で元に戻す。
// IME にはレジスタ自身のアドレスの下位 16 ビット（0x0208、ビット 0 は 0）が書かれる
/* FAKEMATCH: the two blocks that run once are meaningless, and the address is stored through a pointer of its own. Without either block the interrupt-master register's address is loaded before the first address the guarded code uses; a constant 0x208 store loads it from the literal pool where the ROM stores the address register (strh r3, [r3]). One text for Scheduler_FindCallback, AddOrUpdateCallback, RemoveCallback, EnableCallbacks, EnableUnmaskedOverlayCallbacks, SetCallbackMask, DisableCallbacks, DisableOverlayCallbacks and Runtime_SetIrqHandler. */
#define IRQ_LOCK(saved)                   \
	do                                    \
	{                                     \
		saved = REG_IME;                  \
		{                                 \
			volatile u16 *ime = &REG_IME; \
                                          \
			*ime = (u32)ime;              \
		}                                 \
		do                                \
		{

#define IRQ_UNLOCK(saved) \
		} while (0);      \
		REG_IME = saved;  \
	} while (0)

#endif
