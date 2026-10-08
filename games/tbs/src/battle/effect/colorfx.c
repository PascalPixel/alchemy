//--------------------------------------------------------------------------
// colorfx.c
// スロット：パレットの色処理とキャンバスの転送・線描き
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_effect_work.h"
#include "heap_state.h"
#include "game_state.h"
#include "types.h"
#include "dma.h"
#include "iwram_call.h"
#include "resource.h"
#include "io_reg.h"
void AudioCommand_PlayFar(s32 cue);

#define gBattleFxWork ((u8 **)&gBattleFxWork)


static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
	/* FAKEMATCH: needed to match: a direct call changes BattlePres_ProcessPendingTileTransfer from mov r0, r4 to lsl r1, r1, #8 */
	Iwram_FillWords(dst, size, value);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

//--------------------------------------------------------------------------
// スロットを始める
// 画面を消し、乱数の種をプレイ時間で決めてから ReelGame_Run へ入る
//--------------------------------------------------------------------------
s32 Runtime_BlankDisplayLoadValueAndRun(void)
{
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
	gRandomSeed = gGameState.play_time;
	AudioCommand_PlayFar(9);
	ReelGame_Run();
	return 0;
}

//--------------------------------------------------------------------------
// RGB555 の色に倍率をかけて書き込む
// src   : 元の色
// dst   : 書き込み先
// scale : 倍率（0x10000 で等倍）
// count : 色数
//--------------------------------------------------------------------------
s32 Graphics_ScaleRgb555(u16 *src, u16 *dst, s32 scale, s32 count)
{
	s32 n;
	u32 r_mask;
	u32 g_mask;
	u32 b_mask;
	u32 col;
	u32 r;
	u32 g;
	u32 b;

	if (count > 0)
	{
		r_mask = 0x1f;
		g_mask = 0x3e0;
		b_mask = 0x7c00;
		n = count;
		do
		{
			// 各成分を元の位置のままかけ、上位 16bit を取る
			col = *src;
			r = col & r_mask;
			g = col & g_mask;
			b = b_mask & col;
			r *= scale;
			g *= scale;
			b *= scale;
			col = ((r >> 16) & r_mask) | ((g >> 16) & g_mask);
			col |= (b >> 16) & b_mask;
			*dst = col;
			src++;
			dst++;
			n--;
		} while (n != 0);
	}
	return 0;
}

//--------------------------------------------------------------------------
// キャンバスの転送（スケジューラのコールバック）
// transfer_pending が立ったフレームだけ、256x128 の 8bpp キャンバスを
// VRAM 0x06003500 へ送る
//--------------------------------------------------------------------------
void BattlePres_ProcessPendingTileTransfer(void)
{
	struct BattleEffectWork *work;
	u8						*buf;

	work = *(void **)gBattleFxWork;
	if (work->transfer_pending == 1)
	{
		buf = gBattleFxWork[1];
		switch (work->transfer_mode)
		{
		case 1: // 送ったあと transfer_value で塗りつぶす
			Dma_Set(buf, (void *)0x06003500, DMA_ENABLE32 | DMA_32BIT | 0x2000, REG_DMA3);
			FillWords(buf, 0x8000, work->transfer_value);
			break;
		case 2: // 残像：0 以外の色番号を 1/2 または 3/4 に落とす
			if (work->transfer_value == 50)
				ColorBuffer_BackupAndHalveNonzero(buf, (u8 *)0x06003500, 0x8000);
			else
				ColorBuffer_BackupAndScaleNonzeroThreeQuarters(buf, (u8 *)0x06003500, 0x8000);
			break;
		}
		work->transfer_pending = 0;
		work->frames_since_transfer = 1;
	}
	else
	{
		(work->frames_since_transfer)++;
	}
}

//--------------------------------------------------------------------------
// 画面の色を 1 段暗くする
// BG パレット 10 の 16 色と、OBJ パレットの 1〜239 番の各成分を 1 ずつ下げる
//--------------------------------------------------------------------------
void Palette_DarkenSceneStep(void)
{
	u16 *col;
	s32	 i;

	col = (u16 *)&BG_PLTT_COLOR(10, 0);
	for (i = 0; i != 16; i++)
	{
		s32 b = (*col >> 10) & 0x1f;
		s32 g = (*col >> 5) & 0x1f;
		s32 r = *col & 0x1f;

		b--;
		g--;
		r--;

		if (b < 0)
			b = 0;
		if (g < 0)
			g = 0;
		if (r < 0)
			r = 0;
		*col++ = (b << 10) | (g << 5) | r;
	}
	col = (u16 *)&OBJ_PLTT[1];
	for (i = 0; i != 239; i++)
	{
		s32 b = (*col >> 10) & 0x1f;
		s32 g = (*col >> 5) & 0x1f;
		s32 r = *col & 0x1f;

		b--;
		g--;
		r--;

		if (b < 0)
			b = 0;
		if (g < 0)
			g = 0;
		if (r < 0)
			r = 0;
		*col++ = (b << 10) | (g << 5) | r;
	}
}

//--------------------------------------------------------------------------
// BG パレットの 64 色を、リソースのパレットへ 1 段近づける
// 0 番（背景色）は書き戻さない
// resource : パレットを持つリソース
//--------------------------------------------------------------------------
void Palette_StepTowardResource(s32 resource)
{
	u16	 dst[64];
	u16 *col = (u16 *)BG_PLTT;
	s32	 i;

	Dma_Set(Resource_GetTableEntry(resource), dst, DMA_ENABLE32 | DMA_32BIT | 0x20, REG_DMA3);
	for (i = 0; i != 64; i++, col++)
	{
		s32 r = *col & 0x1f;
		s32 g = (*col >> 5) & 0x1f;
		s32 b = (*col >> 10) & 0x1f;
		s32 dst_r = dst[i] & 0x1f;
		s32 dst_g = (dst[i] >> 5) & 0x1f;
		s32 dst_b = (dst[i] >> 10) & 0x1f;

		if (r < dst_r)
			r++;
		else if (r > dst_r)
			r--;
		if (g < dst_g)
			g++;
		else if (g > dst_g)
			g--;
		if (b < dst_b)
			b++;
		else if (b > dst_b)
			b--;
		dst[i] = (b << 10) | (g << 5) | r;
	}
	Dma_Set(dst + 1, (void *)&BG_PLTT[1], DMA_ENABLE32 | 63, REG_DMA3);
}

//--------------------------------------------------------------------------
// 256x128 の 8bpp キャンバス（横 32 タイル）に線を引く
// 既にある色番号より大きいときだけ書き込む
// x0, y0 : 始点（ドット）
// x1, y1 : 終点（ドット）
// color  : 色番号
//--------------------------------------------------------------------------
void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
	s32 dx = x1 - x0;
	s32 dy = y1 - y0;
	s32 frac = 0x80; // 8bit 小数の 0.5
	u8 *canvas = *(gBattleFxWork + 1);
	s32 step;
	s32 i;
	s32 x;
	s32 y;
	s32 tmp;
	u32 ofs;

	if (ABS(dx) < ABS(dy))
	{
		// 縦長：y を 1 ドットずつ進め、x は小数で追う
		if (dy < 0)
		{
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			dx = x1 - x0;
			dy = y1 - y0;
		}
		step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
		x = x0;
		for (i = y0; i != y1; i++)
		{
			ofs = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
			ofs = (ofs << 3) + (x & 7);
			if (canvas[ofs] < color)
				canvas[ofs] = color;
			frac += step;
			if (frac & 0x100)
			{
				if (dx > 0)
					x++;
				else
					x--;
				frac &= ~0x100;
			}
		}
	}
	else
	{
		// 横長：x を 1 ドットずつ進め、y は小数で追う
		if (dx < 0)
		{
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			dx = x1 - x0;
			dy = y1 - y0;
		}
		step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
		y = y0;
		for (i = x0; i != x1; i++)
		{
			ofs = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
			ofs = (ofs << 3) + (i & 7);
			if (canvas[ofs] < color)
				canvas[ofs] = color;
			frac += step;
			if (frac & 0x100)
			{
				if (dy > 0)
					y++;
				else
					y--;
				frac &= ~0x100;
			}
		}
	}
}
