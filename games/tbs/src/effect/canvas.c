#include "unit/effect/effect.h"
#include "unit/effect/canvas.h"
#include "unit/effect/dispatch.h"
#include "unit/effect/burst.h"
#include "unit/effect/step.h"
#include "unit/object/objtable.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "btlwork.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "btltype.h"
#include "sound_ids.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"

void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
	struct BattleEffectWork *work;
	u8						*canvas;
	u8						 delay[128];
	s32						 x;
	s32						 y;
	s32						 pos;

	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	for (x = 0; x != 128; x++)
		delay[x] = Random16() & 0x3f;

	if (mode == 1)
	{
		s32 front;
		s32 speed;

		front = 0;
		speed = 1;
		x = 0;
		do
		{
			front += speed;
			speed++;
			for (; x != front; x++)
			{
				for (y = 0; y != 128; y++)
				{
					pos = x - delay[y];
					if (pos >= 0)
						if (pos <= 127)
							canvas[(((y / 8) * 16 + pos / 8) * 8 + (y & 7)) * 8 + (pos & 7)] =
								1 - value;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (front <= 256);
	}
	else
	{
		s32 front;
		s32 speed;

		front = 0;
		speed = 1;
		y = 0;
		do
		{
			front += speed / 2;
			speed += 4;
			for (; y != front; y++)
			{
				for (x = 0; x != 128; x++)
				{
					pos = y - delay[x];
					if (pos >= 0)
						if (pos <= 127)
							canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] =
								1 - value;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (front <= 191);
	}
}

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
	void				   **heap_cache = gBattleFxWork;
	struct BattleEffectWork *work = heap_cache[0];
	void					*source;
	s32						*counter;
	s32						 next_counter;

	if (work->transfer_pending == 1)
	{
		source = heap_cache[1];
		switch (work->transfer_mode)
		{
		case 0:
			Iwram_CopyWords((void *)0x06004000, source, 0x4000);
			break;
		case 1:
			Iwram_CopyWords((void *)0x06004000, source, 0x4000);
			Iwram_FillWords(source, 0x4000, work->transfer_value);
			break;
		case 2:
			if (work->transfer_value == 50)
			{
				ColorBuffer_BackupAndHalve(source, (void *)0x06004000, 0x4000);
			}
			else
			{
				ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
			}
			break;
		case 3:
			ColorBuffer_BackupAndDarken(source, work->transfer_value, (void *)0x06004000, 0x4000);
			break;
		case 4:
			ColorBuffer_BackupAndBrighten(source, work->transfer_value, (void *)0x06004000, 0x4000);
			break;
		}
		work->transfer_pending = 0;
		counter = &work->frames_since_transfer;
		next_counter = 1;
	}
	else
	{
		counter = &work->frames_since_transfer;
		next_counter = *counter + 1;
	}
	*counter = next_counter;
}

void BattleFx_FlushPendingGraphicsTransfer(void)
{
	void				   **heap_cache;
	struct BattleEffectWork *work;
	void					*source;
	s32						 transfer_mode;

	heap_cache = gBattleFxWork;
	work = heap_cache[0];
	source = Ram_MapCellBuffer;
	if (work->transfer_pending != 1)
		return;

	transfer_mode = work->transfer_mode;
	switch (transfer_mode)
	{
	case 0:
		Iwram_CopyWords((void *)0x06008000, source, 0x7800);
		break;
	case 1:
		Iwram_CopyWords((void *)0x06008000, source, 0x7800);
		Iwram_FillWords(source, 0x7800, work->transfer_value);
		break;
	case 2:
		if (work->transfer_value == 50)
		{
			ColorBuffer_BackupAndHalve(source, (void *)0x06008000, 0x7800);
		}
		else
		{
			ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
		}
		break;
	case 3:
		ColorBuffer_BackupAndDarken(source, work->transfer_value, (void *)0x06008000, 0x7800);
		break;
	}

	work->transfer_pending = 0;
}

#define FADE_STEP 1092

void Graphics_ApplyWindowBlendRegisters(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];

	REG_WIN0H = work->win0h;
	REG_WIN0V = work->win0v;
	REG_WIN1H = work->win1h;
	REG_WIN1V = work->win1v;
	REG_WININ = work->winin;
	REG_WINOUT = work->winout;
	REG_DISPCNT = work->dispcnt;
	REG_BLDCNT = work->bldcnt;
	REG_BLDALPHA = work->bldalpha;
}

void Display_ApplyBg2Reference(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];

	REG_BG2X = work->bg2x;
	REG_BG2Y = work->bg2y;
}

void Palette_StepFadeTransfer(void)
{
	struct BtlWork			*battle = WORK_SLOT(HEAP_SLOT_BATTLE);
	struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

	if (work->fade_frames > 0)
	{
		s32 step = ++work->fade_step;

		Graphics_ScaleRgb555ClampedFar(
			battle->palette, (void *)&BG_PLTT_COLOR(6, 0), 0x10000 - step * FADE_STEP, 128);
		work->fade_frames--;
	}
}

void BattleFx_ClearActorTimers(void)
{
	Iwram_ClearWords((*(struct BattleEffectWork **)gBattleFxWork)->actor_timers,
		sizeof((*(struct BattleEffectWork **)gBattleFxWork)->actor_timers));
}

void ObjectGroup_TickMemberTimers(void)
{
	struct BattleEffectWork *work;
	s32						 i;

	work = gBattleFxWork[0];
	i = 0;
	do
	{
		if (work->actor_timers[i] != 0)
		{
			if ((work->actor_timers[i] = work->actor_timers[i] - 1) == 0)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
			}
		}
		i += 1;
	} while (i != 8);
}

//--------------------------------------------------------------------------
// canvas.c
// バトル演出：BG2 のキャンバス面を開く・閉じる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// キャンバスは 128x128 の 8bpp（0x4000 バイト）で、BG2 のアフィン面に
// キャラクタブロック 1 から並べる。横は 2 倍に伸ばして 256 ドットで見せる
//
// REG_DISPCNT（モード 1・OBJ 1 次元）
//   0x7741 : BG0〜2・OBJ・WIN0/1
//   0x7341 : BG0・BG1・OBJ・WIN0/1（BG2 を消す）
//   0x7541 : BG0・BG2・OBJ・WIN0/1（BG1 を消す）
//   0x1741 : BG0〜2・OBJ
//   0x1341 : BG0・BG1・OBJ
//   0x1541 : BG0・BG2・OBJ
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// キャンバス面を開く（背景パレットをフェードさせ、BG2 をキャンバスにする）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginCanvasLayer(s32 bg_control)
{
	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);
	gBgScroll[1].y = 32;
	WaitFrames(1);

	// 背景パレットのフェードを始める
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 0);
	work->fade_frames = 0;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);

	// BG2 を消している間に設定を変える
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	display->second_mode = 0;
	WaitFrames(1);

	REG_BLDCNT = 0x3f44;   // BG2 を半透明にして下の面と重ねる
	REG_BLDALPHA = 0x100e; // 14:16
	REG_BG2X = 0;
	REG_BG2Y = -0x1000; // 16 ライン下げる
	REG_BG2PA = 0x80;	// 横 2 倍
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;	// 0〜240
	REG_WIN0V = 0x1088; // 16〜136
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);

	// スクリーン：16x16 キャラクタを順に並べる（1 キャラ 1 バイトなので 2 個ずつ書く）
	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
	}

	Iwram_ClearWords(canvas, 0x4000);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	work->shake_frames = 0;
	work->saved_bg1_x = gBgScroll[1].x;
	work->saved_bg1_y = gBgScroll[1].y;
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// キャンバス面を開く（ウインドウなしで始め、fade_frames を 24 にする版）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_OpenCanvasLayer(s32 bg_control)
{
	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1741, 0x20000);
	gBgScroll[1].y = 32;
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 128);
	work->fade_frames = 24;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	display->second_mode = 0;
	WaitFrames(1);

	REG_BLDCNT = 0x3f44;
	REG_BLDALPHA = 0x100e;
	REG_BG2X = 0;
	REG_BG2Y = -0x1000;
	REG_BG2PA = 0x80;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);
	WaitFrames(1);

	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
	}

	Iwram_ClearWords(canvas, 0x4000);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// キャンバス面を開き、BG2 を 256x256 の折り返しありにする
// スクリーンの各行は 16 キャラのキャンバスと、16 キャラの 0 番
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginTiledCanvas(s32 bg_control)
{
	s32 row;
	s32 col;
	s32 ofs;

	BattleFx_BeginCanvasLayer(bg_control);
	REG_BG2CNT = bg_control | 0x6784;
	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
		for (col = 0; col != 8; col++, ofs += 2)
		{
			*(volatile u16 *)((u8 *)BG_CANVAS_MAP + ofs) = 0;
		}
	}
}

//--------------------------------------------------------------------------
// キャンバス面を閉じ、背景パレットのレベルを 8 フレームで 21 から 0 へ戻す
//--------------------------------------------------------------------------
void BattleFx_EndCanvasLayer(void)
{
	struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork			*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	s32						 i;

	AudioCommand_PlayFar(289);
	gBgScroll[1].x = work->saved_bg1_x;
	gBgScroll[1].y = work->saved_bg1_y;
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	REG_BG2CNT = 0x787;
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	gBgScroll[1].y = 32;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	REG_BLDCNT = 0;
	WaitFrames(1);

	BattlePresentation_ConfigurePaletteFadeFar(2, btl->background, 7);
	WaitFrames(1);
	for (i = 0; i != 8; i++)
	{
		BattlePresentation_SetPaletteLevelFar(btl->background, 21 - i * 3);
		WaitFrames(1);
	}

	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// BG1 とウインドウを消して、背景パレットのフェードを設定する
//--------------------------------------------------------------------------
void BattleFx_SetTransitionFlagAndDisplay(void)
{
	struct BtlWork *btl;
	s32			   *flag;

	flag = &((struct BattleBackgroundView *)Ram_WorkSlot[HEAP_SLOT_BATTLE_BACKGROUND])->second_mode;
	btl = Ram_WorkSlot[HEAP_SLOT_BATTLE];
	*flag = 1;
	QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(2, btl->background, 0);
	WaitFrames(1);
}

void BattleFx_SetupCanvasTileMap(void)
{
	u8						   **cells = (u8 **)&BtlWk;
	void						*canvas = cells[31];
	struct BtlWork				*screen = (struct BtlWork *)cells[0];
	struct BattleBackgroundView *layer = (struct BattleBackgroundView *)cells[35];
	s32							 row;
	s32							 col;
	s32							 off;

	BattleFx_ClearActorTimers();
	BattlePresentation_ConfigurePaletteFadeFar(2, screen->background, 0);
	gBgScroll[1].y = 32;
	layer->second_mode = 0;
	Iwram_ClearWords((void *)0x06003fc0, 64);
	Iwram_FillWords((void *)0x0600f900, 0x200, -1);
	off = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 32; col++, off += 2)
		{
			if (col > 15)
				*(volatile u16 *)(0x0600fb00 + off) = 0xff;
			else
				*(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
		}
	}
	REG_DISPCNT = 0x7741;
	REG_BG1CNT = 0x1f81;
	REG_BLDCNT = 0x3f42;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	REG_BLDALPHA = 0x100e;
	Iwram_ClearWords(canvas, 0x4000);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------

// canvas_draw_clipped_line.c

// バトル演出：キャンバスへ線を引く（y をキャンバス内に収める）

//--------------------------------------------------------------------------

#define ABS(v) ((v) < 0 ? -(v) : (v))

//--------------------------------------------------------------------------
// 128x128 の 8bpp キャンバス（横 16 タイル）に線を引く
// 既にある色番号より大きいときだけ書き込む
// x0, y0 : 始点（ドット）
// x1, y1 : 終点（ドット）
// color  : 色番号
//--------------------------------------------------------------------------
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
	s32 dx = x1 - x0;
	s32 dy = y1 - y0;
	s32 frac = 0x80; // 8bit 小数の 0.5
	u8 *canvas = ((u8 **)gBattleFxWork)[1];
	s32 step;
	s32 i;
	s32 x;
	s32 y;
	s32 tmp;
	u32 ofs;

	if (y0 < 0)
		y0 = 0;
	if (y0 > 127)
		y0 = 127;
	if (y1 < 0)
		y1 = 0;
	if (y1 > 127)
		y1 = 127;

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
			ofs = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
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
			ofs = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
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
