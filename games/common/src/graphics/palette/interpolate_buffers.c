//--------------------------------------------------------------------------
// interpolate_buffers.c
// パレット：フェードの 1 フレームあたりの変化量を求める
//--------------------------------------------------------------------------

#include "types.h"
#include "iwram_call.h"

//--------------------------------------------------------------------------
// 512 色の RGB 成分（1536 個）について、今の値から目標の値までを
// frames で割った変化量を step に書く
// src    : 今の成分
// dst    : 目標の成分
// step   : 変化量の書き込み先
// frames : かけるフレーム数（0 以下なら何もしない）
//--------------------------------------------------------------------------
void Graphics_InterpolatePaletteBuffers(s16 *src, s16 *dst, s16 *step, s32 frames)
{
	s32 n;
	s32 from;
	s32 to;
	s32 (*divide)(s32, s32);

	if (frames > 0)
	{
		/* FAKEMATCH: the routine is called through a local pointer to its fixed entry; a plain call matches The Broken Seal through its IWRAM call layer but not The Lost Age, whose header calls the entry directly (all six of its editions differ). */
		divide = Iwram_SignedDivideEntry;
		n = 3 * 512 - 1;
		do
		{
			from = *src;
			to = *dst;
			*step = divide(to - from, frames);
			n--;
			src++;
			dst++;
			step++;
		} while (n >= 0);
	}
}
