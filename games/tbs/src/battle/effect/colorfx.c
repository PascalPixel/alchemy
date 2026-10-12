#include "audio_engine_symbols.h"
#include "unit/battle/effect/colorfx.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim12.h"
#include "battle.h"
#include "keystate.h"
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
#include "sound_ids.h"
#include "mem.h"
#include "fixed_math.h"
#include "effect_step.h"
#include "battle_efx.h"
#include "callback_scheduler.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "trackbuf.h"
#include "frame.h"

//--------------------------------------------------------------------------
// colorfx.c
// スロット：パレットの色処理とキャンバスの転送・線描き
//--------------------------------------------------------------------------

#define ABS(v) ((v) < 0 ? -(v) : (v))

// 火花の作業領域（マップのセルバッファを借りる）
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

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
		buf = (*(u8 * (*)[]) & gBattleFxWork)[1];
		switch (work->transfer_mode)
		{
		case 1: // 送ったあと transfer_value で塗りつぶす
			Dma_Set(buf, (void *)0x06003500, DMA_ENABLE32 | DMA_32BIT | 0x2000, REG_DMA3);
			Iwram_FillWords(buf, 0x8000, work->transfer_value);
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
	u8 *canvas = *((*(u8 * (*)[]) & gBattleFxWork) + 1);
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

struct Reel
{
	s32 position;
	u8	symbols[21];
	u8	held;
	s8	stop;
};

struct ReelObject
{
	volatile u32 attr01;
	volatile u32 attr2;
};

struct UiWindow;

struct ReelWork
{
	struct Reel		  reels[5];
	s32				  state;
	s32				  cursor;
	s32				  spins;
	s32				  bet;
	u16				  keys;
	u16				  direction;
	u16				  pressed;
	u16				  repeat;
	u8				  unknown_0a4[4];
	s32				  timer;
	s32				  winning_lines[7];
	struct ReelObject obj[128];
	struct UiWindow	 *window;
	struct UiWindow	 *sub_window;
	u8				  unknown_4d0[8];
	u16				  scanline_offsets[160];
#if EDITION_INTERNATIONAL
	s32 phase;
#endif
};

// gGameState seen from +12, the base the bytes load: won_prizes is gGameState.won_prizes
// (through the field itself ReelGame_RunFrame differs 12)
struct ReelSave
{
	u8 unknown_000[0x120];
	s8 won_prizes[16];
};

// 火花のセル（一辺 2..20 ピクセルの正方形）の位置
const u16 ReelGame_SparkCellOffsets[] = {0, 4, 20, 56, 120, 220, 364, 560, 816, 1140};

// リールの絵柄ごとの賞
const u8 ReelGame_SymbolPrizes[] = {0, 3, 1, 4, 2, 5};

// タイトルの文字ごとのタイル
const u8 ReelGame_TitleLetterTiles[REEL_TITLE_LETTERS] = {
#if defined(TBS_EDITION_JA)
	0, 1, 2, 3, 4, 2, 5, 6
#elif defined(TBS_EDITION_ES)
	7, 0, 1, 2, 3, 4, 5, 6, 7
#else
	0, 1, 2, 3, 4, 5, 6, 7
#endif
};

// 枠のスプライトの位置
const u8 ReelGame_FrameSpriteX[] = {2, 0, 232, 232, 32, 32, 200, 200, 28, 28, 208, 208, 219, 219};
const u8 ReelGame_FrameSpriteY[] = {2, 152, 0, 152, 8, 104, 8, 104, 118, 140, 118, 140, 6, 108};

// タイトルの文字の送り幅
const u8 ReelGame_TitleLetterWidths[REEL_TITLE_LETTERS] = {
#if defined(TBS_EDITION_JA)
	32, 24, 22, 28, 28, 22, 18
#elif defined(TBS_EDITION_ES)
	16, 28, 28, 26, 26, 28, 28, 20, 24
#else
	28, 28, 26, 26, 28, 28, 20, 24
#endif
};

void ReelGame_RunFrame(void)
{
	struct ReelWork			*work;
	struct BattleEffectWork *fx;
	volatile u16			*dma;
	s32						 n;
	s32						 blend;
	u16						 pad;
	u32						 held;
	s32						 coins;
	s32						 all;
	s32						 cnt;
	s32						 col;
	s32						 v;
	s32						 x;
	s32						 y;
	s32						 lever;
	s32						 k;
	s32						 i;

	work = ((struct ReelWork **)gBattleFxWork)[6];
	fx = ((struct BattleEffectWork **)gBattleFxWork)[0];
	n = 0;
	blend = 0x400;

	Random16();

	dma = REG_DMA0;
	dma[5] &= ~DMA_START_HBLANK_REPEAT;
	dma[5] &= ~DMA_ENABLE;
	dma[5];
	Dma_Set(work->scanline_offsets, (void *)&REG_BLDY, 0xa2600001, (volatile u32 *)dma);

	pad = gKeysHeld;
	held = gKeysHeld & KEYS_DPAD;
	work->pressed = pad & ~work->keys;
	work->direction = held;
	if ((work->keys & KEYS_DPAD) == work->direction)
	{
		if (work->repeat > 12)
			work->repeat = 12;
		if (work->repeat == 0)
		{
			work->repeat = 4;
		}
		else
		{
			work->repeat--;
			work->direction = n;
		}
	}
	else
	{
		work->repeat = 12;
	}
	work->keys = pad;

	if (gDebugPaused != 0)
		goto build_objects;

	if (work->state == 0)
	{
		coins = PartyInventory_CountItemFar(228);
		UiText_DrawNumberInWindowFar(
			coins - work->bet, REEL_COIN_DIGITS, work->sub_window, REEL_COIN_X, 0);
		UiText_DrawNumberInWindowFar(work->bet, 2, work->sub_window, 64, 8);
		if (work->pressed & KEY_B)
		{
			struct ReelSave *save = (struct ReelSave *)((u8 *)&gGameState + 12);

			work->state = 10;
			save->won_prizes[0] = -2;
			UiWork_FinalizeFar(work->window, 1);
			goto build_objects;
		}
		if (work->pressed & KEY_UP)
		{
			if (work->bet <= 3 && coins > work->bet)
			{
				work->bet++;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			}
			else
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			}
		}
		if (work->pressed & KEY_DOWN)
		{
			if (work->bet > 1)
			{
				work->bet--;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			}
			else
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			}
		}
		REG_BLDCNT = 0x3fd0;
		REG_BLDALPHA = 0x10;
		if (work->pressed & KEY_A)
		{
			work->state = 1;
			fx->frame = 0;
			UiWork_FinalizeFar(work->window, 1);
			for (k = 0; k != work->bet; k++)
				PartyInventory_RemoveFar(228);
			UiWork_FinalizeFar(work->sub_window, 1);
			AudioCommand_PlayFar(0x130);
		}
	}
	else if (work->state == 5)
	{
		all = 0;
		work->timer++;
		for (k = 0; k != 5 && work->reels[k].held != 0; k++)
			;
		if (k == 5)
			all = 1;
		if (work->pressed & KEY_A)
		{
			work->timer = 0;
			fx->frame = 0;
			if (work->spins == 4)
			{
				work->spins = 0;
				work->cursor = 0;
				work->state = 0;
				for (i = 0; i != 5; i++)
				{
					work->reels[i].held = 0;
					work->reels[i].stop = -1;
				}
			}
			else if (work->cursor <= 4)
			{
				AudioCommand_PlayFar(0x131);
				work->reels[work->cursor].held ^= 1;
			}
			else if (all == 0)
			{
				AudioCommand_PlayFar(0x130);
				work->state = 1;
				work->cursor = 0;
				for (i = 0; i != 5; i++)
					work->reels[i].stop = -1;
				work->spins++;
			}
			else
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			}
		}
		else
		{
			if (work->direction & KEY_RIGHT)
			{
				work->cursor = (work->cursor + 1) % 6;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			}
			if (work->direction & KEY_LEFT)
			{
				work->cursor = (work->cursor + 5) % 6;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			}
		}

		if (work->state == 5)
		{
#if EDITION_INTERNATIONAL
			if (work->cursor == 5)
			{
				if (all != 0)
				{
					if (work->phase != 1 && work->phase != 2)
					{
						UiWork_FinalizeFar(work->window, 1);
						work->window =
							UiWindow_CreateFar(REEL_ALL_HELD_X, 0, REEL_ALL_HELD_WIDTH, 4, 6);
						UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsAllHeld, work->window, 0, 0);
						work->phase = 1;
					}
					else if (work->phase == 1)
					{
						UiText_DrawCharacterAtOffsetFar(
							(s32)MsgSlotsCannotPull, work->window, 0, 8);
						work->phase = 2;
					}
				}
				else
				{
					if (work->phase != 3)
					{
						UiWork_FinalizeFar(work->window, 1);
						work->window = UiWindow_CreateFar(REEL_START_X, 0, REEL_START_WIDTH, 3, 6);
						UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsStart, work->window, 0, 0);
					}
					work->phase = 3;
				}
			}
			else if (work->reels[work->cursor].held == 0)
			{
				if (work->phase != 4)
				{
					UiWork_FinalizeFar(work->window, 1);
					work->window = UiWindow_CreateFar(REEL_HOLD_X, 0, REEL_HOLD_WIDTH, 3, 6);
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsHold, work->window, 0, 0);
				}
				work->phase = 4;
			}
			else
			{
				if (work->phase != 5)
				{
					UiWork_FinalizeFar(work->window, 1);
					work->window = UiWindow_CreateFar(REEL_RELEASE_X, 0, REEL_RELEASE_WIDTH, 3, 6);
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsRelease, work->window, 0, 0);
				}
				work->phase = 5;
			}
#else
			UiWork_FinalizeFar(work->window, 1);
			if (work->cursor == 5)
			{
				if (all != 0)
				{
					work->window = UiWindow_CreateFar(12, 0, 18, 3, 6);
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsAllHeld, work->window, 0, 0);
				}
				else
				{
					work->window = UiWindow_CreateFar(18, 0, 12, 3, 6);
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsStart, work->window, 0, 0);
				}
			}
			else
			{
				work->window = UiWindow_CreateFar(18, 0, 12, 3, 6);
				if (work->reels[work->cursor].held == 0)
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsHold, work->window, 0, 0);
				else
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsRelease, work->window, 0, 0);
			}
#endif
		}
		else
		{
			UiWork_FinalizeFar(work->window, 1);
		}
	}
	else if (work->state == 2)
	{
		work->timer++;
		blend = 0;
		if (work->timer == 60)
		{
			work->state = 3;
			AudioCommand_PlayFar(93);
			work->timer = 0;
			REG_BLDCNT = 0x3f44;
			REG_BLDALPHA = 0x1010;
			fx->transfer_mode = 2;
			fx->transfer_value = 75;
		}
	}
	else if (work->state == 3)
	{
		work->timer++;
		blend = 0;
		if (work->pressed & KEY_A)
		{
			work->state = 10;
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
		}
	}
	else if (work->state == 11)
	{
#if EDITION_INTERNATIONAL
		if (work->phase == 0)
		{
			work->phase = 1;
			UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsPullLever, work->window, 0, 8);
		}
#endif
		if (work->pressed & KEY_A)
		{
			work->state = 5;
#if EDITION_INTERNATIONAL
			work->phase = 0;
#endif
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
			UiWork_FinalizeFar(work->window, 1);
		}
	}
	else if (work->state == 20)
	{
		work->timer++;
		if (work->timer == 45)
			work->state = 10;
	}
	else if (work->state != 10)
	{
		if (work->timer == 4)
		{
			work->window = UiWindow_CreateFar(REEL_STOP_X, 17, REEL_STOP_WIDTH, 3, 6);
			UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsStopReels, work->window, 0, 0);
		}
		if (work->timer == 16)
			AudioCommand_PlayFar(0x132);
		if (work->timer > 56)
		{
			if (fx->frame > 31 || (work->pressed & KEY_R))
			{
				fx->frame = 0;
				for (i = 0; i != 5; i++)
				{
					if (work->reels[i].held == 0 && work->reels[i].stop == -1)
					{
						work->reels[i].stop = (Random16() & 3) + 4;
						AudioCommand_PlayFar(0x133);
						break;
					}
				}
			}
		}

		for (i = 0; i != 5; i++)
		{
			if (work->reels[i].stop > 0)
				work->reels[i].stop--;
		}

		cnt = 0;
		for (i = 0; i != 5; i++)
		{
			if (work->reels[i].held == 1)
				cnt++;
			else if (work->reels[i].stop == 0 && (work->reels[i].position & 15) == 8)
				cnt++;
		}

		if (cnt == 5)
		{
			struct ReelSave *save = (struct ReelSave *)((u8 *)&gGameState + 12);
			s32				 hits = 0;

			for (col = 0; col != 7; col++)
			{
				s32 mixed;
				s32 found;

				found = -1;
				mixed = 0;
				work->winning_lines[col] = 0;
				if (col > 3 - work->bet && col < work->bet + 3)
				{
					for (i = 0; i != 5; i++)
					{
						s32 sym;

						if (col == 0)
							sym = work->reels[i]
									  .symbols[(i - work->reels[i].position / 16 + 22) % 21];
						else if (col == 6)
							sym = work->reels[i]
									  .symbols[(-i - work->reels[i].position / 16 + 26) % 21];
						else
							sym = work->reels[i]
									  .symbols[(col - work->reels[i].position / 16 + 21) % 21];
						if (sym != 5)
						{
							if (found == -1)
								found = sym;
							else if (found != sym)
								mixed = 1;
						}
					}
					if (mixed == 0)
					{
						work->winning_lines[col] = 1;
						save->won_prizes[hits] = ReelGame_SymbolPrizes[found];
						hits++;
					}
				}
			}
			work->timer = 0;
			if (hits != 0)
			{
				save->won_prizes[hits] = -1;
				work->state = 2;
				AudioCommand_PlayFar(171);
				fx->transfer_mode = 1;
				fx->transfer_value = 0;
				REG_BLDCNT = 0;
				UiWork_FinalizeFar(work->window, 1);
			}
			else
			{
				work->state = 11;
#if EDITION_INTERNATIONAL
				work->phase = 0;
				UiWork_FinalizeFar(work->window, 1);
#if REEL_HINT_BEFORE_LAST_SPIN
				if (work->spins <= 3)
#endif
				{
					work->window = UiWindow_CreateFar(3, 16, 24, 4, 6);
					UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsChooseHeld, work->window, 0, 0);
				}
#else
				UiWork_FinalizeFar(work->window, 1);
				{
					s32 message;

					work->window = UiWindow_CreateFar(7, 16, 16, 4, 6);
					message = (s32)MsgSlotsChooseHeld;
					UiText_DrawCharacterAtOffsetFar(message, work->window, 0, 0);
					UiText_DrawCharacterAtOffsetFar(message + 1, work->window, 0, 8);
				}
#endif
				if (work->spins == 4)
				{
					s32				 left = PartyInventory_CountItemFar(228);
					struct UiWindow *window;
					s32				 message;

					if (left > 0)
						work->state = 20;
					else
						work->state = 20;
					if (work->bet > left)
						work->bet = left;
					work->spins = 0;
					work->cursor = 0;
					for (i = 0; i != 5; i++)
					{
						work->reels[i].held = 0;
						work->reels[i].stop = -1;
					}
					fx->transfer_mode = 1;
					fx->transfer_value = 0;
					REG_BLDCNT = 0;
					window = UiWindow_CreateFar(18, 0, 12, 4, 6);
					work->sub_window = window;
					message = (s32)MsgSlotsBet;
					UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
					UiText_DrawCharacterAtOffsetFar(message - 1, work->sub_window, 0, 0);
				}
			}
		}

		if (work->state == 1)
		{
			for (i = 0; i != 5; i++)
			{
				struct Reel *reel = &work->reels[i];

				if (reel->held == 0)
				{
					if (reel->stop != 0)
						reel->position += 8;
					else if ((reel->position & 15) != 8)
						reel->position += 8;
					if (reel->position == 336)
						reel->position = 0;
				}
			}
		}
		(fx->frame)++;
		work->timer++;
	}

build_objects:
	if (work->state == 5)
	{
		x = work->cursor * 36 + 36;
		y = 128;
		v = 0;
		if ((work->timer & 15) <= 7)
			v = 1;
		if (work->cursor == 5)
		{
			x = 208;
			y = 32;
		}
		work->obj[n].attr01 = ((x - 12) << 16) | blend | (y + 8) | 0x80006000;
		work->obj[n].attr2 = (v << 4) + 0x2b0;
		n++;
		work->obj[n].attr01 = ((x + 12) << 16) | blend | (y + 8) | 0x90006000;
		work->obj[n].attr2 = (v << 4) + 0x2b0;
		n++;
		work->obj[n].attr01 = (x << 16) | blend | y | 0x80002000;
		work->obj[n].attr2 = 0x1f0;
		n++;
	}

	if (work->state == 3)
	{
		for (i = 0; i != REEL_TITLE_LETTERS; i++)
		{
			struct EffectStep *p = &fx->particles[i];

#if defined(TBS_EDITION_ES)
			if (i == 0)
				work->obj[n].attr01 = (((*(s16 *)((u8 *)&p->x + 2) + 0x1f0) & 0x1ff) << 16) | blend
									  | ((*(s16 *)((u8 *)&p->y + 2) + 0x108) & 255) | 0xb0000000;
			else
#endif
				work->obj[n].attr01 = (*(s16 *)((u8 *)&p->x + 2) << 16) | blend
									  | ((*(s16 *)((u8 *)&p->y + 2) + 256) & 255) | 0x80000000;
			work->obj[n].attr2 = ((ReelGame_TitleLetterTiles[i] << 4) + REEL_TITLE_TILES) | 0xf000;
			p->y += p->velocity_y;
			p->velocity_y += 0x4000;
			if (work->timer % 256 == i * 4 + 200)
			{
				p->velocity_y = 0x60000;
				p->variant = 0;
			}
			if (p->y > 0x400000)
			{
				p->y = 0x400000;
				if (p->variant <= 1)
					p->velocity_y = -p->velocity_y / 2;
				p->variant++;
			}
			n++;
		}
	}

	for (k = 0; k != 14; k++)
	{
		work->obj[n].attr01 =
			(ReelGame_FrameSpriteX[k] << 16) | blend | ReelGame_FrameSpriteY[k] | 0x80006000;
		if (k <= 3)
			work->obj[n].attr2 = 0x4e0;
		else
			work->obj[n].attr2 = 0x4e8;
		n++;
	}

	for (i = 0; i != 5; i++)
	{
		if (work->reels[i].held == 0)
		{
			work->obj[n].attr01 = blend | ((i * 36 + 32) << 16) | 0x8000207c;
			work->obj[n].attr2 = 0x460;
		}
		else
		{
			work->obj[n].attr01 = blend | ((i * 36 + 32) << 16) | 0x8000207c;
			work->obj[n].attr2 = 0x480;
		}
		n++;
	}

	for (i = 0; i != 5; i++)
	{
		work->obj[n].attr01 = blend | ((i * 16 + 32) << 16) | 0x80006003;
		if (i == work->spins)
			work->obj[n].attr2 = (i * 32 + 0x210) | 0x400;
		else
			work->obj[n].attr2 = (i * 32 + 0x220) | 0x400;
		n++;
	}

	for (col = 0; col != 7; col++)
	{
		work->obj[n].attr01 =
			(((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) | blend | (col * 16 + 5) | 0x80002000;
		if (col > 3 - work->bet && col < work->bet + 3)
			work->obj[n].attr2 = 0x5d0;
		else
			work->obj[n].attr2 = 0x510;
		n++;
	}

	for (i = 0; i != 5; i++)
	{
		struct Reel *reel = &work->reels[i];

		for (col = 0; col != 7; col++)
		{
			work->obj[n].attr01 =
				(col * 16 + reel->position % 16 + 4) | ((i * 32 + 40) << 16) | 0x80006000;
			work->obj[n].attr2 =
				(reel->symbols[(col - reel->position / 16 + 21) % 21] << 4) | 0x800;
			n++;
		}
	}

	lever = 40;
	if (work->state == 1)
	{
		if (work->timer <= 47)
			lever = ((Trig_Sin(work->timer * 682) << 6) >> 16) + 40;
		else if (work->timer <= 55)
			lever = ((Trig_Sin((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
	}
	work->obj[n].attr01 = lever | blend | 0x80d06000;
	work->obj[n].attr2 = 0x500;
	n++;

	for (k = 0; k != 8; k++)
	{
		work->obj[n].attr01 = blend | (k * 16 + 12) | 0x80ce6000;
		if (k == 0)
			work->obj[n].attr2 = 0x540;
		else if (k == 1)
			work->obj[n].attr2 = 0x550;
		else if (k == 6)
			work->obj[n].attr2 = 0x570;
		else if (k == 7)
			work->obj[n].attr2 = 0x580;
		else
			work->obj[n].attr2 = 0x560;
		n++;
	}

	while (n != 128)
	{
		work->obj[n].attr01 = 0x40f02000;
		work->obj[n].attr2 = 0;
		n++;
	}

	Dma_Set(work->obj, OAM, (n * 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
}

void ReelGame_InitTitle(void)
{
	struct ReelWork			*state;
	struct BattleEffectWork *work;
	struct EffectStep		*letter;
	const u8				*width;
	s32						 left;
	s32						 step;
	s32						 y;
	s32						 i;
	s32						 angle;
	struct UiWindow			*window;
	s32						 message;

	state = ((struct ReelWork **)gBattleFxWork)[6];
	work = ((struct BattleEffectWork **)gBattleFxWork)[0];
	left = 0;
	for (i = 0; i != 0x800; i++)
		SPARKS[i].variant = 0;

	letter = work->particles;
	y = -0x200000;
	width = ReelGame_TitleLetterWidths;
	for (i = 0; i != REEL_TITLE_LETTERS; i++)
	{
		letter[i].x = (left + REEL_TITLE_X) << 16;
		step = *width;
		width++;
		left += step;
		letter[i].y = y;
		letter[i].velocity_y = 0;
		letter[i].variant = 0;
		y += -0x80000;
	}

	for (i = 0; i != 160; i++)
		state->scanline_offsets[i] = 0;

	for (i = 0; i != 40; i++)
	{
		angle = i * 0x199;
		state->scanline_offsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
		state->scanline_offsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
	}

	state->spins = 0;
	state->state = 0;
	state->cursor = 0;
	work->transfer_mode = 1;
	work->transfer_value = 0;
	REG_BLDCNT = 0;
	window = UiWindow_CreateFar(18, 0, 12, 4, 6);
	state->sub_window = window;
	message = (s32)MsgSlotsBet;
	UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
	UiText_DrawCharacterAtOffsetFar(message - 1, state->sub_window, 0, 0);
}

void ReelGame_Run(void)
{
	s32						 picks[8];
	s32						 lit[7];
	DrawRectangle			 draw[2];
	u8						*sheet;
	void					*canvas;
	struct BattleEffectWork *work;
	struct ReelWork			*state;
	struct ReelSave			*save;
	u8						*entry;
	u8						*tiles;
	s32						 frame;
	u32						 count;
	s32						 i;
	s32						 j;
	s32						 k;

	sheet = Runtime_AllocateHeapBlock(41, 0x60e);
	canvas = Runtime_AllocateHeapBlock(40, 0x8000);
	work = Runtime_AllocateBlock(39, sizeof(struct BattleEffectWork));
	state = Runtime_AllocateBlock(45, sizeof(struct ReelWork));
	tiles = Ram_MapCellBuffer;
	save = (struct ReelSave *)((u8 *)&gGameState + 12);
	RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
	save->won_prizes[0] = -1;
	state->repeat = 0;
	state->bet = 1;
	Scheduler_ResetTaskTable();
	gOamCopyEnabled = 0;

	count = 0;
	j = 0;
	for (k = 0; k != 20; k++)
	{
		for (i = 0; i != 32; i++, j += 2)
		{
			if (i >= 5 && i <= 24 && k > 2 && k <= 13)
			{
				*(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0xa1a6;
			}
			else if (i > 29)
			{
				*(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0;
			}
			else
			{
				*(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0xa1a8 + count;
				count++;
			}
		}
	}
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), sheet);
	entry = Resource_GetTableEntry((s32)&ResourceId_SlotsFramePicture);
	Dma_Set(entry, (void *)&BG_PLTT_COLOR(10, 0), DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);
	entry += 32;
	Resource_DecodeType01(entry, tiles);
	j = 0;
	for (k = 0; k != 20; k++)
	{
		for (i = 0; i != 30; i++)
		{
			if (!(i >= 5 && i <= 24 && k > 2 && k <= 13))
			{
				Dma_Set(tiles + (k * 30 + i) * 32, (void *)(BG_CHAR_BLOCK(2) + 0x3500 + j),
					DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);
				j += 32;
			}
		}
	}
	Iwram_ClearWords(BG_SCREEN_BLOCK(5) + 0x500, 0x300);
	j = 0;
	for (k = 0; k != 20; k++)
	{
		for (i = 0; i != 32; i++, j += 2)
		{
			if (k < 2 || k > 16)
				*(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) = 0xbf;
			else
				*(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) = k * 32 + i + 148;
		}
	}
	REG_BG1CNT = BGCNT_SCREEN_BASE(5) | BGCNT_CHAR_BASE(2) | 1;
	REG_BG2CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
	gBgScroll[0].x = 0;
	gBgScroll[0].y = 0;
	gBgScroll[1].x = 0;
	gBgScroll[1].y = 0;
	gBgScroll[2].x = 0;
	gBgScroll[2].y = 0;
	REG_WININ = 0x3737;
	REG_WINOUT = 0x2727;
	REG_BLDCNT = 0x3f44;
	REG_BLDALPHA = 0x1010;
	REG_BG1HOFS = 0;
	REG_BG2HOFS = 0;
	REG_BG1VOFS = 0xff60;
	REG_BG2VOFS = 0xff60;
	REG_WIN0H = 0x28c8;
	REG_WIN0V = 0x1878;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0xa0;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0xa0;

	{
		u16 *shade = (u16 *)(work->sheet + 0x200);
		s32	 size = 0x8000;

		state->state = 0;
		state->cursor = 0;
		state->spins = 0;
		work->frame = 0;
		state->timer = 0;
		Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet), (void *)BG_PLTT,
			DMA_ENABLE32 | DMA_32BIT | 32, REG_DMA3);
		BG_PLTT_COLOR(4, 0) = 0x2f8b;
		BG_PLTT_COLOR(4, 1) = 0x5bf6;
		entry = Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
		Dma_Set(entry, (void *)OBJ_PLTT, DMA_ENABLE32 | DMA_32BIT | 480 / 4, REG_DMA3);
		entry += 480;
		Resource_DecodeType01(entry, tiles);
		Dma_Set(tiles, (void *)0x06010000, DMA_ENABLE32 | DMA_32BIT | 0x1b30, REG_DMA3);
		entry = Resource_GetTableEntry((s32)&ResourceId_SlotsJackpot);
		Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);
		entry += 32;
		Resource_DecodeType01(entry, tiles);
		Dma_Set(tiles, (void *)REEL_JACKPOT_TILES, REEL_JACKPOT_COPY, REG_DMA3);
		UiWork_InitializeWithResourceCountersFar();
		ReelGame_InitTitle();

		for (i = 0; i != 5; i++)
		{
			struct Reel *reel = &state->reels[i];

			reel->position = 8;
			reel->held = 0;
			reel->stop = -1;
			for (k = 0; k != 21; k++)
				reel->symbols[k] = Random16() % 5;
		}
		for (i = 0; i != 5; i++)
		{
			struct Reel *reel = &state->reels[i];

			for (j = 0; j != 8; j++)
			{
				picks[j] = Random16() % 21;
				for (k = 0; k != j; k++)
				{
					if (picks[j] == picks[k])
					{
						j--;
						break;
					}
				}
			}
			for (k = 0; k != 8; k++)
			{
				s32 rank = k;

				if (rank > 5)
					rank = 5;
				reel->symbols[picks[k]] = rank;
			}
		}

		BattleEffect_LoadWorkFar(46, 8, 7, 3, 2);
		draw[0] = gWorkSlot[46];
		BattleEffect_LoadWorkFar(47, 8, 7, 3, 3);
		draw[1] = gWorkSlot[47];
		Iwram_FillWords(canvas, size, 0);
		Dma_Set(canvas, (void *)0x06003500, DMA_ENABLE32 | DMA_32BIT | 0x8000 / 4, REG_DMA3);
		Dma_Set((void *)BG_PLTT, work, DMA_ENABLE32 | DMA_32BIT | 0x80, REG_DMA3);
		Dma_Set((void *)OBJ_PLTT, shade, DMA_ENABLE32 | DMA_32BIT | 0x80, REG_DMA3);
		Graphics_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0, 0x100);
		Graphics_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, 0, 0x100);
		REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON
					  | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
		if (PartyInventory_CountItemFar(228) == 1)
		{
			struct UiWindow *window = UiWindow_CreateFar(6, 16, 18, 3, 6);

			state->window = window;
			UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsControls, window, 0, 0);
		}
		else
		{
			struct UiWindow *window =
				UiWindow_CreateFar(REEL_HELP_WINDOW_X, 16, REEL_HELP_WINDOW_WIDTH, 4, 6);
			s32 message;

			state->window = window;
			message = (s32)MsgSlotsChangeBet;
			UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
			UiText_DrawCharacterAtOffsetFar(message + 1, state->window, 0, 8);
		}
		work->transfer_pending = 0;
		Scheduler_AddOrUpdateCallback(ReelGame_RunFrame, 0x480);
		Scheduler_AddOrUpdateCallback(BattlePres_ProcessPendingTileTransfer, 0x480);

		for (frame = 0; state->state != 10; frame++)
		{
			if (frame <= 16)
			{
				Graphics_ScaleRgb555(shade, (u16 *)OBJ_PLTT, frame << 12, 0x100);
				Graphics_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, frame << 12, 0x100);
			}
			if (state->state == 3)
			{
				s32 phase = frame % 80;

				if (phase <= 15)
					Palette_StepTowardResource((s32)&ResourceId_MercuryDjinnSheet);
				else if (phase <= 31)
					Palette_StepTowardResource((s32)&ResourceId_VenusDjinnSheet);
				else if (phase <= 47)
					Palette_StepTowardResource((s32)&ResourceId_EmberStreakSheet);
				else if (phase <= 63)
					Palette_StepTowardResource((s32)&ResourceId_LimePalette);
				else
					Palette_StepTowardResource((s32)&ResourceId_JupiterDjinnSheet);
				if (state->timer <= 15)
					Palette_DarkenSceneStep();
				if (state->timer > 16 && (frame & 7) == 0)
				{
					s32				   x = (Random16() & 127) + 56;
					s32				   y = (Random16() & 31) + 48;
					struct EffectStep *spark = &SPARKS[(frame / 8 & 3) * 256];

					for (j = 0; j != 256; j++)
					{
						s32 speed;
						s32 angle;

						speed = 255;
						speed &= Random16();
						angle = Random16() & 0xffff;
						spark->x = x << 16;
						spark->y = y << 16;
						spark->velocity_x = Trig_Sin(angle) * (speed + 64) >> 6;
						spark->velocity_y = -(Trig_Cos(angle) * (speed + 64)) >> 6;
						spark->variant = (Random16() & 15) + 16;
						spark++;
					}
				}
				for (j = 0; j != 1024; j++)
				{
					struct EffectStep *spark = &SPARKS[j];

					if (spark->variant > 0)
					{
						s32 x;

						spark->variant--;
						x = spark->x;
						if (x >= 0 && x <= 0xffffff)
						{
							s32 y = spark->y;

							if (y <= 0x7fffff)
							{
								if (y >= 0)
								{
									s32 px = x >> 16;
									s32 py = y >> 16;
									s32 size = spark->variant / 12 + 1;

									draw[j & 1](canvas, sheet + ReelGame_SparkCellOffsets[size - 1],
										px - size, py - size, size * 2, size * 2);
								}
							}
						}
						spark->x += spark->velocity_x;
						spark->y += spark->velocity_y;
						spark->velocity_x = spark->velocity_x * 60 / 64;
						spark->velocity_y = spark->velocity_y * 60 / 64;
					}
				}
			}
			if (state->state == 0 || state->state == 2)
			{
				for (j = 0; j != 7; j++)
					lit[j] = 0;
				if (state->state == 0)
				{
					lit[3] = 1;
					if (state->bet > 1)
					{
						lit[4] = 1;
						lit[2] = 1;
					}
					if (state->bet > 2)
					{
						lit[5] = 1;
						lit[1] = 1;
					}
					if (state->bet > 3)
					{
						lit[6] = 1;
						lit[0] = 1;
					}
				}
				else if ((frame & 7) <= 3)
				{
					for (j = 0; j != 7; j++)
						lit[j] = state->winning_lines[j];
				}
				for (j = 0; j != 3; j++)
				{
					s32 color = j != 1;

					color = 65 - color;
					if (lit[1])
						BattleFx_DrawCanvasLine(20, j + 19, 200, j + 19, color);
					if (lit[2])
						BattleFx_DrawCanvasLine(28, j + 35, 200, j + 35, color);
					if (lit[3])
						BattleFx_DrawCanvasLine(20, j + 51, 200, j + 51, color);
					if (lit[4])
						BattleFx_DrawCanvasLine(28, j + 67, 200, j + 67, color);
					if (lit[5])
						BattleFx_DrawCanvasLine(20, j + 83, 200, j + 83, color);
					if (lit[0])
						BattleFx_DrawCanvasLine(28, j + 5, 200, j + 91, color);
					if (lit[6])
						BattleFx_DrawCanvasLine(28, j + 97, 200, j + 11, color);
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		}

		for (j = 0; j != 17; j++)
		{
			Graphics_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0x10000 - (j << 12), 0x100);
			Graphics_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, 0x10000 - (j << 12), 0x100);
			WaitFrames(1);
		}
		Runtime_ReleaseHeapBlock(47);
		Runtime_ReleaseHeapBlock(46);
		Scheduler_RemoveCallback(BattlePres_ProcessPendingTileTransfer);
		Scheduler_RemoveCallback(ReelGame_RunFrame);
		Runtime_ReleaseHeapBlock(45);
		Runtime_ReleaseHeapBlock(40);
		Runtime_ReleaseHeapBlock(39);
		Runtime_ReleaseHeapBlock(41);
	}
}

void AudioTrack_ResetSlotBuckets(void)
{
	s32					   index;
	struct AudioSlotNode **bucket;

	for (index = 0; index < 0x400; index++)
	{
		Flash_Handler3->nodes[index].slot = index;
		Flash_Handler3->nodes[index].prev = NULL;
	}
	bucket = Flash_Handler3->bucket;
	for (index = 0xFF; index >= 0; index--)
	{
		*bucket++ = NULL;
	}
}

void AudioTrack_InsertSlotNode(s32 index)
{
	struct AudioTrackSlotWork *work = Flash_Handler3;

	work->nodes[index].prev = (struct AudioSlotNode *)&work->bucket[work->bucket_by_slot[index]];
	work->nodes[index].next = work->bucket[work->bucket_by_slot[index]];
	work->bucket[work->bucket_by_slot[index]] = &work->nodes[index];
	if (work->nodes[index].next != NULL)
		work->nodes[index].next->prev = &work->nodes[index];
}

void AudioTrack_RemoveSlotNode(s32 slot)
{
	if (Flash_Handler3->nodes[slot].prev != NULL)
	{
		if (Flash_Handler3->nodes[slot].next != NULL)
			Flash_Handler3->nodes[slot].next->prev = Flash_Handler3->nodes[slot].prev;
		Flash_Handler3->nodes[slot].prev->next = Flash_Handler3->nodes[slot].next;
	}
}

void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input)
{
	s32 limit = count;
	s32 current = 0;
	u32 base = start;

	if (current < limit)
	{
		u32 mask = 0x3ff;
		u32 removal = base + 0x124;

		do
		{
			u32						   slot = base + current;
			struct AudioTrackSlotWork *state;
			u32						   read_offset;
			u32						   next_offset;
			u8						   value;

			AudioTrack_RemoveSlotNode(removal & mask);
			state = Flash_Handler3;
			read_offset = state->input_cursor;
			value = input[read_offset];
			next_offset = read_offset + 1;
			state->input_cursor = next_offset;
			if (next_offset == state->input_limit)
			{
				state->bucket_by_slot[slot & mask] = -1;
				break;
			}

			state->bucket_by_slot[slot & mask] = value;
			current++;
			AudioTrack_InsertSlotNode(slot & mask);
			removal++;
		} while (current < limit);
	}

	current++;
	if (current < limit)
	{
		u32							mask = 0x3ff;
		struct AudioTrackSlotWork **root = &Flash_Handler3;
		s32							empty = -1;

		do
		{
			u32						   slot = (base + current) & mask;
			struct AudioTrackSlotWork *state;

			AudioTrack_RemoveSlotNode(slot);
			state = *root;
			current++;
			state->bucket_by_slot[slot] = empty;
		} while (current < limit);
	}
}

void AudioTrack_CopyBufferedBytes(u8 *destination)
{
	struct AudioTrackSlotWork *work = Flash_Handler3;
	s32						   index;

	for (index = 0; index != work->out_cnt; index++)
	{
		destination[work->out_total] = work->out_buf[index];
		work->out_total++;
	}
}

#define WINDOW_MASK 0x3ff
#define MATCH_MAX 272
#define DIST_MAX 63
#define PRIME_COUNT 672

static __inline__ void AudioTrack_FindWindowMatch(s32 start)
{
	struct AudioSlotNode *node;
	s32					  len;

	Flash_Handler3->match_len = 1;
	if (Flash_Handler3->bucket_by_slot[start] == -1)
	{
		return;
	}
	node = Flash_Handler3->bucket[Flash_Handler3->bucket_by_slot[start]];
	if (start + MATCH_MAX > WINDOW_MASK)
	{
		while (node != NULL)
		{
			s32 dist = (start - node->slot) & WINDOW_MASK;

			if (dist > 0 && dist <= DIST_MAX)
			{
				for (len = 1; len < MATCH_MAX; len++)
				{
					if (Flash_Handler3->bucket_by_slot[(start + len) & WINDOW_MASK]
						!= Flash_Handler3->bucket_by_slot[(node->slot + len) & WINDOW_MASK])
					{
						break;
					}
				}
				if (Flash_Handler3->match_len < len)
				{
					Flash_Handler3->match_ofs = dist;
					Flash_Handler3->match_len = len;
					if (len == MATCH_MAX)
					{
						return;
					}
				}
			}
			node = node->next;
		}
	}
	else
	{
		while (node != NULL)
		{
			s32 dist = (start - node->slot) & WINDOW_MASK;

			if (dist > 0 && dist <= DIST_MAX)
			{
				for (len = 1; len < MATCH_MAX; len++)
				{
					if (Flash_Handler3->bucket_by_slot[start + len]
						!= Flash_Handler3->bucket_by_slot[(node->slot + len) & WINDOW_MASK])
					{
						break;
					}
				}
				if (Flash_Handler3->match_len < len)
				{
					Flash_Handler3->match_ofs = dist;
					Flash_Handler3->match_len = len;
					if (len == MATCH_MAX)
					{
						return;
					}
				}
			}
			node = node->next;
		}
	}
}

s32 AudioTrack_PackStream(const u8 *input, u8 *dst, s32 size)
{
	s32 defer;
	s32 len;
	s32 save_ofs;
	s32 save_len;
	s32 reach[2];
	s16 code;

	defer = 0;
	Flash_Handler3 = (struct AudioTrackSlotWork *)gMapCellBuffer;
	AudioTrack_ResetSlotBuckets();
	Flash_Handler3->input_limit = size;
	Flash_Handler3->pos = 0;
	Flash_Handler3->input_cursor = 0;
	Flash_Handler3->out_total = 0;
	Flash_Handler3->flag_mask = 0x80;
	Flash_Handler3->out_buf[0] = 0;
	Flash_Handler3->out_cnt = 1;
	AudioTrack_ConsumeSlotBytes(0, PRIME_COUNT, input);

	while (Flash_Handler3->bucket_by_slot[Flash_Handler3->pos] != -1)
	{
		AudioTrack_FindWindowMatch(Flash_Handler3->pos);
		if (defer == 0)
		{
			len = Flash_Handler3->match_len;
			if (Flash_Handler3->match_len > 1)
			{
				save_ofs = Flash_Handler3->match_ofs;
				save_len = len;
				AudioTrack_FindWindowMatch((Flash_Handler3->pos + 1) & WINDOW_MASK);
				if (Flash_Handler3->match_len > 2)
				{
					reach[0] = Flash_Handler3->match_len + 1;
					AudioTrack_FindWindowMatch((Flash_Handler3->pos + len) & WINDOW_MASK);
					reach[1] = Flash_Handler3->match_len + len;
					if (reach[0] >= reach[1])
					{
						save_len = 1;
						defer = 1;
					}
				}
				Flash_Handler3->match_ofs = save_ofs;
				Flash_Handler3->match_len = save_len;
			}
		}

		if (Flash_Handler3->match_len > 1)
		{
			defer = 0;
			Flash_Handler3->out_buf[0] |= Flash_Handler3->flag_mask;
			if (Flash_Handler3->match_len <= 16)
			{
				code = ((Flash_Handler3->match_ofs << 4) & ~0xfff)
					   | (Flash_Handler3->match_ofs & 0xff)
					   | (((Flash_Handler3->match_len - 1) << 8) & 0xf00);
				Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = (u16)code >> 8;
				Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = code;
			}
			else
			{
				code = ((Flash_Handler3->match_ofs << 4) & ~0xfff)
					   | (Flash_Handler3->match_ofs & 0xff);
				Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = (u16)code >> 8;
				Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = code;
				Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = Flash_Handler3->match_len - 17;
			}
		}
		else
		{
			Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] =
				Flash_Handler3->bucket_by_slot[Flash_Handler3->pos];
			Flash_Handler3->match_len = 1;
		}

		AudioTrack_ConsumeSlotBytes(
			Flash_Handler3->pos + PRIME_COUNT, Flash_Handler3->match_len, input);
		Flash_Handler3->pos = (Flash_Handler3->pos + Flash_Handler3->match_len) & WINDOW_MASK;
		Flash_Handler3->flag_mask >>= 1;
		if (Flash_Handler3->flag_mask == 0)
		{
			AudioTrack_CopyBufferedBytes(dst);
			Flash_Handler3->flag_mask = 0x80;
			Flash_Handler3->out_buf[0] = 0;
			Flash_Handler3->out_cnt = 1;
		}
	}

	Flash_Handler3->out_buf[0] |= Flash_Handler3->flag_mask;
	Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = 0;
	Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = 0;
	AudioTrack_CopyBufferedBytes(dst);
	return Flash_Handler3->out_total;
}
