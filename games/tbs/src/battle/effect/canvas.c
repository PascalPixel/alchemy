//--------------------------------------------------------------------------
// canvas.c
// バトル演出：BG2 のキャンバス面を開く・閉じる
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
#include "heap_state.h"
#include "canvas.h"
#include "map_scroll.h"
#include "btldisp.h"
#include "btlwork.h"
#include "battle_effect_work.h"
#include "project.h"
#include "types.h"
#include "iwram_call.h"
#include "system.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "ram_buffer.h"

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

#define BattlePresentation_SetPaletteLevelFar \
	((void (*)(u16, s32))BattlePresentation_SetPaletteLevelFar)

typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
	/* FAKEMATCH: needed to match: direct calls share the size in r5 and move the routine to r6, changing the canvas tails */
	clear(destination, size);
}

void AudioCommand_PlayFar(s32);

// REG_DISPCNT への書き込みを I/O 書き込みキューに積む（IME を止めて）
#define QUEUE_DISPLAY_CONTROL(control)                                                                \
	do                                                                                                \
	{                                                                                                 \
		/* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */     \
		u32 saved;                                                                                    \
		s32 count;                                                                                    \
                                                                                                      \
		do                                                                                            \
		{                                                                                             \
			/* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
			saved = *ime;                                                                             \
		} while (0);                                                                                  \
		*ime = (u16)ime;                                                                              \
		count = q->count;                                                                             \
		if (count <= 31)                                                                              \
		{                                                                                             \
			u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);                                     \
			*(u16 *)&q->count = count + 1;                                                            \
			*destination++ = (control);                                                               \
			*destination++ = (u32)&REG_DISPCNT;                                                       \
			*destination = 0x20000;                                                                   \
		}                                                                                             \
		*ime = saved;                                                                                 \
	} while (0)

//--------------------------------------------------------------------------
// キャンバス面を開く（背景パレットをフェードさせ、BG2 をキャンバスにする）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginCanvasLayer(s32 bg_control)
{
	/* FAKEMATCH: needed to match: the one-pass IME read and u16 count store preserve queue scheduling and whole-function pointer lifetimes */

	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	volatile u16				*ime;
	struct IoWriteQueue			*q;
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	q = &gIoWriteQueue;
	ime = &REG_IME;
	QUEUE_DISPLAY_CONTROL(0x7741);
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
	QUEUE_DISPLAY_CONTROL(0x7741);

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

	ClearWords(Iwram_ClearWords, canvas, 0x4000);
	ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
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
	/* FAKEMATCH: needed to match: the one-pass IME read and u16 count store preserve queue scheduling and whole-function pointer lifetimes */

	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	volatile u16				*ime;
	struct IoWriteQueue			*q;
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	q = &gIoWriteQueue;
	ime = &REG_IME;
	QUEUE_DISPLAY_CONTROL(0x1741);
	gBgScroll[1].y = 32;
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 128);
	work->fade_frames = 24;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
	QUEUE_DISPLAY_CONTROL(0x1341);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	QUEUE_DISPLAY_CONTROL(0x1341);
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
	QUEUE_DISPLAY_CONTROL(0x7741);
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

	ClearWords(Iwram_ClearWords, canvas, 0x4000);
	ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL

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
	/* FAKEMATCH: needed to match: the one-pass IME/queue scopes and u16 count store preserve scheduling, loading the queue before the preceding store */

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
