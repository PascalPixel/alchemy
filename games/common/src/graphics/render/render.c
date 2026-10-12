//--------------------------------------------------------------------------
// render.c
// 表示：変換行列の退避・復帰と、回転・移動・拡大・カメラの行列づくり
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "transform.h"
#include "dma.h"
#include "io_reg.h"
#include "fixed_math.h"

// 変換行列 gTransform は s32 が 12 個（Q16 で 1.0 = 0x10000）。
// [0]〜[8] が 3x3 の回転、[9]〜[11] が平行移動。
// Iwram_TransformMatrix は渡した行列を今の行列にかけ合わせる。

#define MTX_DMA (DMA_ENABLE32 | DMA_32BIT | 12) // 行列 1 つ分の DMA

#if GAME_TLA
#include "ram_buffer.h"

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Render_EmptyHook(void)
{
}

//--------------------------------------------------------------------------
// 行列の退避先を確保し、今の行列を単位行列にする
//--------------------------------------------------------------------------
void Render_ResetTransformState(void)
{
	gTransformStackTop = Runtime_AllocateBlock(8, sizeof(gTransform));
	gTransformStackDepth = 0;
	{
		s32 zero;

		CAMELOT_ASM("transform.h's identity store, ⚓️ building the one before the zeros")
		__asm__ volatile("mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
						 "mov %0, #0\n\tmov r4, #0\n\t"
						 "stmia r0!, {r1, r2, %0, r4}\n\t"
						 "stmia r0!, {r1, r2, %0, r4}\n\t"
						 "stmia r0!, {r1, r2, %0, r4}"
			: "=l"(zero)
			: "r"(gTransform)
			: "r0", "r1", "r2", "r4", "memory");
	}
}

//--------------------------------------------------------------------------
// 退避先を作業領域の先頭に戻す
//--------------------------------------------------------------------------
void SceneTransform_RewindStack(void)
{
	gTransformStackTop = Ram_HeapSlots->transform_stack;
	gTransformStackDepth = 0;
}

#endif

//--------------------------------------------------------------------------
// 今の行列を退避する（1 段まで）
//--------------------------------------------------------------------------
void Graphics_SaveTransferWorkOnce(void)
{
	if (gTransformStackDepth <= 0)
	{
		Dma_Set(gTransform, gTransformStackTop, MTX_DMA, REG_DMA3);
		gTransformStackDepth++;
		gTransformStackTop += sizeof(gTransform);
	}
}

//--------------------------------------------------------------------------
// 今の行列を dst に写す
//--------------------------------------------------------------------------
void Graphics_SaveTransferWork(void *dst)
{
	Dma_Set(gTransform, dst, MTX_DMA, REG_DMA3);
}

//--------------------------------------------------------------------------
// src の行列を今の行列にする
//--------------------------------------------------------------------------
void Graphics_LoadTransferWork(const void *src)
{
	Dma_Set(src, gTransform, MTX_DMA, REG_DMA3);
}

//--------------------------------------------------------------------------
// 退避した行列を戻す
//--------------------------------------------------------------------------
void Graphics_RestoreTransferWork(void)
{
	if (gTransformStackDepth > 0)
	{
		--gTransformStackDepth;
		gTransformStackTop -= sizeof(gTransform);
		Dma_Set(gTransformStackTop, gTransform, MTX_DMA, REG_DMA3);
	}
}

