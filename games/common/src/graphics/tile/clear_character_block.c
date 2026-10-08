//--------------------------------------------------------------------------
// clear_character_block.c
// タイル：BG のキャラクターとパレットの半分を消す
//--------------------------------------------------------------------------

#include "dma.h"
#include "io_reg.h"
#include "types.h"

//--------------------------------------------------------------------------
// 256 色のキャラクター 30K バイトを色 1 で埋め、パレット 128 色を 0 にする
// alt : 0 なら前半（VRAM 0x06000000・パレット 0〜127）
//       1 なら後半（VRAM 0x06008000・パレット 128〜255、色 129 で埋める）
//--------------------------------------------------------------------------
void Graphics_ClearCharacterBlockAndPalette(s32 alt)
{
	u32			 v;
	u32			 vram;
	u32			 pal;
	volatile u32 fill;

	if (alt == 0)
	{
		v = 0x01010101;
		vram = 0x06000000;
		pal = 0x05000000;
	}
	else
	{
		v = 0x81818181;
		vram = 0x06008000;
		pal = 0x05000100;
	}
	fill = v;
	Dma_Set((const void *)&fill, (void *)vram, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 7680,
		REG_DMA3);
	fill = 0;
	Dma_Set((const void *)&fill, (void *)pal, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 64,
		REG_DMA3);
}
