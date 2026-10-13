#ifndef ALCHEMY_IRQLOCK_H
#define ALCHEMY_IRQLOCK_H

#include "io_reg.h"
#include "types.h"

// IME を ime（REG_IME を指すポインタ）越しに読み書きする 3 つの手順。
// 止めるときは IME にレジスタ自身のアドレスの下位 16 ビット
// （0x0208、ビット 0 は 0）が書かれる。
// Each step is its own statement macro, written do { } while (0), the shape
// ☀️'s lock sites and write-queue pushes and ⚓️'s callback scheduler match
// through: the saved copy is taken before IME is written. SerialRuntime_Initialize
// writes IRQ_LOCK's two steps out (see its tag). IME_RESTORE's and IRQ_UNLOCK's
// wrappers are devices, not this layer: a plain restore matches at some sites
// (see their tags).

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
/* FAKEMATCH: the do { } while (0) wrapper steers: written plainly as *(ime) = (saved), BattleFx_PlayUnitElementEffect differs 2 and BattleEffect_RunShockwaveScene differs 6 in all six ☀️ editions; BattlePresentation_SetPaletteLevel's plain restore matches, and IME_RESTORE((volatile u16 *)&REG_IME, saved) there differs 2 in all six (measured 2026-10-14). A device at each use, as IRQ_UNLOCK is (S2). */
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

// IRQ_LOCK で止めた IME を saved に戻す
/* FAKEMATCH: the do { } while (0) wrapper steers: it keeps the restore apart from the code before it. Written plainly as REG_IME = saved, 7 of ☀️'s 8 sched.c lock functions differ 2 in tbs-en (Scheduler_DisableOverlayCallbacks matches either way), and ⚓️'s Func_0813f89c and Func_08143114 differ 2 in all six editions; BattlePresentation_SetPaletteLevel's plain restore matches, and through IRQ_UNLOCK it differs 2 in all six (measured 2026-10-13). Not every use matches through it, so it is a device at each use, not a layer (S2). */
#define IRQ_UNLOCK(saved) \
	do                    \
	{                     \
		REG_IME = saved;  \
	} while (0)

#endif
