//--------------------------------------------------------------------------
// uiwork.c
// ウィンドウ：メッセージのチャンネルの後始末と、メッセージに埋める値の表
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "window.h"
#include "edition.h"
#if GAME_TLA
#include "ram_buffer.h"
#else
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "tbs_edition.h"

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);

extern s32 gKeysHeld; // 押しているキー
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern s32 gKeyState; // 押したキー
#endif
extern s32 gKeysPressedLatch; // 押したキー（ためておいたもの）
s32		   AudioCommand_GetStateByteFar();

//--------------------------------------------------------------------------
// ウィンドウをすべて閉じる
// 3 つのチャンネルのウィンドウを閉じ、開閉の動きが終わるまで待ってから、
// 直接開いたウィンドウを閉じる
//--------------------------------------------------------------------------
void UiWork_DrainPending(void)
{
	struct UiRenderWork	 *uw;
	struct UiChannelSlot *ch;
	struct UiWindow		 *win;
	u32					  done;
	struct UiWindow		 *work;
	struct UiWindow		 *poll_work;
	s32					  i;
	u16					  flag;

	uw = (struct UiRenderWork *)gWindowWork[0];
	ch = uw->channels;
	win = uw->windows;
	i = 0;
	do
	{
		work = ch->work;
		if (work != 0 && work->flags != 0)
			UiWork_Finalize(work, 0);
		i++;
		ch++;
	} while (i != 3);

	// 閉じ終えたチャンネルを空ける。動いているものがあれば 1 フレーム待つ
poll:
	done = 1;
	ch = uw->channels;
	i = 0;
	do
	{
		poll_work = ch->work;
		if (poll_work != 0)
		{
			if ((*(s32 *)&poll_work->frame) == 0) // frame と duration がともに 0
			{
				flag = poll_work->flags;
				if (flag == 0)
					ch->work = (struct UiWindow *)(u32)flag;
				else
					done = 0;
			}
			else
			{
				done = 0;
			}
		}
		i++;
		ch++;
	} while (i != 3);
	i = 0;
	if (!done)
	{
		WaitFrames(1);
		goto poll;
	}

	// 直接開いたウィンドウ
	goto win_test;
win_loop:
	if (win->flags != 0)
		UiWork_Finalize(win, 0);
	win++;
	i++;
win_test:
	if (i != WINDOW_COUNT)
		goto win_loop;
}

//--------------------------------------------------------------------------
// チャンネルのウィンドウの効果 4 を 1 フレーム進める
// countdown のあいだ 1 タイル大きい枠を描き、終わったら元の枠に戻す
//--------------------------------------------------------------------------
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *ch)
{
	struct UiWindow *work = ch->work;
	s32				 effect = work->effect;
	s32				 x = work->x;
	s32				 y = work->y;
	s32				 w = work->width;
	s32				 h = work->height;

	if (effect != 4)
		return;

	UiWindow_DrawFrame(x - 1, y - 1, w + 2, h + 2);
	ch->countdown--;
	if (ch->countdown != 0)
		return;

	ch->work->effect = 0;
	UiWindow_EraseBorderRect(x - 1, y - 1, w + 2, h + 2);
	UiWindow_DrawFrame(x, y, w, h);
}

#endif

//--------------------------------------------------------------------------
// メッセージに埋める値の表（8 つ）を空にする
//--------------------------------------------------------------------------
void UiWork_ClearValueNameTables(void)
{
	s32					 no;
	struct UiRenderWork *work;

#if GAME_TLA
	work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
	/* FAKEMATCH: the barrier keeps the work-pointer load before the table offsets. */
	__asm__("");
#else
	work = (struct UiRenderWork *)gWindowWork[0];
#endif
	no = 0;

	do
	{
		work->values[no] = 0;
		work->names[no] = 0;
		no++;
	} while (no != 8);
}

//--------------------------------------------------------------------------
// 表のあいているところに、種類 kind の値 value を入れる
// （いっぱいなら入れない）
//--------------------------------------------------------------------------
void UiWork_PushValueSlot(u32 value, u32 kind)
{
#if GAME_TLA
	struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
#else
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
#endif
	u32 no = 0;
	u32 limit = 8;

	do
	{
		if (work->names[no] == 0)
		{
			work->values[no] = value;
			work->names[no] = kind;
			break;
		}
		no++;
	} while (no != limit);
}

#if !GAME_TLA

//--------------------------------------------------------------------------
// 表から種類 kind の値をさがす（なければ 0）
// clear : 0 でなければ、見つけたものを表から消す
//--------------------------------------------------------------------------
u32 UiRender_LookupNamedValue(u32 kind, u32 clear)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	u32					 i;
	u32					 value = 0;

	for (i = 0; i < 8; i++)
	{
		if (work->names[i] == kind)
		{
			value = work->values[i];
			if (clear != 0)
			{
				work->values[i] = 0;
				work->names[i] = 0;
			}
			break;
		}
	}
	return value;
}

//--------------------------------------------------------------------------
// メッセージの待ちを打ち切るか
// busy で音の状態が 0 のとき、または A・B・L・R を押しているとき、
// countdown を 0 にして 1 を返す
//--------------------------------------------------------------------------
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	s32					 cancel = 0;
	s32					 zero;

	if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
		cancel = 1;
	zero = 0;
	if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancel = 1;
	if (cancel != zero)
	{
		ch->countdown = zero;
		return 1;
	}
	return zero;
}

//--------------------------------------------------------------------------
// メッセージの待ちを打ち切るか（押したキーで見る）
// mode が 0 でなければ、ためておいたキーで見る
//--------------------------------------------------------------------------
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	s32					 key;
	s32					 cancel = 0;
	u8					 zero;

	if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
		cancel = 1;
	key = gKeyState;
	zero = 0;
	if (work->mode != zero)
		key = gKeysPressedLatch;
	if (key & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancel = 1;
	if (cancel != 0)
	{
		ch->countdown = zero;
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 書き終えたチャンネルのウィンドウを閉じ、10 フレーム待つ
//--------------------------------------------------------------------------
void UiWork_FinalizePendingCore(void)
{
	s32					  i;
	struct UiChannelSlot *ch;
	struct UiWindow		 *work;

	ch = ((struct UiRenderWork *)gWindowWork[0])->channels;
	i = 0;
	do
	{
		work = ch->work;
		if (work != NULL && (*(s32 *)&work->frame) == 0 && work->flags != 0 && work->state != 0)
		{
			UiWork_Finalize(work, 2 & work->flags);
		}
		i++;
		ch++;
	} while (i != 3);
	WaitFrames(10);
}

#endif
