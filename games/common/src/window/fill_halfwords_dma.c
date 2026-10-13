//--------------------------------------------------------------------------
// fill_halfwords_dma.c
// メモリ：DMA で 16 ビットずつ埋める
//--------------------------------------------------------------------------

#include "types.h"
#include "dma.h"
#include "io_reg.h"

//--------------------------------------------------------------------------
// dst から count 個を value で埋め、埋め終えた次の位置を返す
//--------------------------------------------------------------------------
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count)
{
	volatile u16 fill;
	if (count > 0)
	{
		fill = value;
		Dma_Set(&fill, dst, count | (DMA_ENABLE32 | DMA_SRC_FIXED32), REG_DMA3);
		dst += count;
	}
	return dst;
}
