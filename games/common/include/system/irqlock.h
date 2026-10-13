#ifndef ALCHEMY_IRQLOCK_H
#define ALCHEMY_IRQLOCK_H

#include "io_reg.h"
#include "types.h"

// IME を ime（REG_IME を指すポインタ）越しに読み書きする 3 つの手順。
// 止めるときは IME にレジスタ自身のアドレスの下位 16 ビット
// （0x0208、ビット 0 は 0）が書かれる。
// Each step is its own statement macro, written do { } while (0), the shape
// ☀️'s lock sites and write-queue pushes and ⚓️'s callback scheduler match
// through: the saved copy is taken before IME is written, and the restore
// stands apart from the code around it. ⚓️'s own queue writers (io.c) do
// not match through it yet, and SerialRuntime_Initialize writes IRQ_LOCK's
// two steps out (see its tag).

// 元の IME を saved に入れる
#define IME_SAVE(ime, saved) \
	do                       \
	{                        \
		(saved) = *(ime);    \
	} while (0)

// 割り込みを止める
#define IME_DISABLE(ime)          \
	do                            \
	{                             \
		*(ime) = (u16)(u32)(ime); \
	} while (0)

// IME を saved に戻す
#define IME_RESTORE(ime, saved) \
	do                          \
	{                           \
		*(ime) = (saved);       \
	} while (0)

// 割り込みを止めて、元の IME を saved に入れる。IRQ_UNLOCK で元に戻す。
#define IRQ_LOCK(saved)                \
	do                                 \
	{                                  \
		volatile u16 *ime_ = &REG_IME; \
                                       \
		IME_SAVE(ime_, saved);         \
		IME_DISABLE(ime_);             \
	} while (0)

#define IRQ_UNLOCK(saved) \
	do                    \
	{                     \
		REG_IME = saved;  \
	} while (0)

#endif
