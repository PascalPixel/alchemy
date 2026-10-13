//--------------------------------------------------------------------------
// glow_update_sine.c
// パレット：BG パレット 14 の色 8〜11 を sin で明滅させる
//--------------------------------------------------------------------------

#include "edition.h"
#if EDITION_INTERNATIONAL || defined(TLA_EDITION_JA)
#include "types.h"
#include "fixed_math.h"
#include "rgb555.h"

extern u8 gFrameTick[];

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、4 色を位相をずらした sin で明滅させる
// s = sin / 16384 として RGB555 で 赤 22 + 2s・緑 16 + s・青 20 + s
//--------------------------------------------------------------------------
void PaletteGlow_UpdateSine(void)
{
	s32			 *tick;
	volatile u16 *pal;
	s32			  col;
	s32			  s;
	s32			  i;

	tick = (s32 *)gFrameTick;
	pal = (volatile u16 *)0x050001D0; // BG パレット 14 の色 8
	i = 0;
	do
	{
		s = Trig_Sin((*tick + i * 8) * 0x300); // 1 色ごとに 8 フレームずらす
		col = s / 16384;
		{
			u32 r = col << 1;
			u32 g = col;

			r += 22;
			g += 16;
			col += 20;
			Palette_PutRgb555(pal, r, g, col);
		}
	} while (++i <= 3);
}

#endif
