#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "field_event.h"
#include "io_reg.h"
#include "battle_effect_runtime.h"

void KorimaPalette_SaveFirst(void)
{
	Dma_Set(gPaletteBufferWork->palette, KorimaPalette_First, DMA_ENABLE32 | DMA_32BIT | 0xe0,
		REG_DMA3);
}

void KorimaPalette_SaveSecond(void)
{
	Dma_Set(gPaletteBufferWork->palette, KorimaPalette_Second, DMA_ENABLE32 | DMA_32BIT | 0xe0,
		REG_DMA3);
}

void KorimaPalette_Capture(void)
{
	u16 *buf = gPaletteBufferWork->palette;

	Dma_Set((const void *)BG_PLTT, buf, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
	Dma_Set((const void *)OBJ_PLTT, buf + 0xe0, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
	Engine_ColorBufferApplySource(0x10000, 0);
}

void KorimaPalette_Restore(s32 second)
{
	u16 *buf = gPaletteBufferWork->palette;

	if (second)
		Dma_Set(KorimaPalette_Second, buf, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
	else
		Dma_Set(KorimaPalette_First, buf, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
	Engine_ColorBufferApplyTarget(0x10000, 0);
	KorimaPalette_Capture();
}
