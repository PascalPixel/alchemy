//--------------------------------------------------------------------------
// OBJECT.C
// リソース：アニメの記録を消す
//--------------------------------------------------------------------------

#include "DMA.H"
#include "ANIMSPR.H"
#include "IO_REG.H"

//--------------------------------------------------------------------------
// AnimationEntry 1 つ分を 0 で埋める（NULL なら何もしない）
//--------------------------------------------------------------------------
void ResourceMetadata_ClearRecord(void *dst)
{
	if (dst != 0)
	{
		volatile u32 zero;

		zero = 0;
		Dma_Set(&zero, dst,
			(DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32) | (sizeof(struct AnimationEntry) / 4),
			REG_DMA3);
	}
}
