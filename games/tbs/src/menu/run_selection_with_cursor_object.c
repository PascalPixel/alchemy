//--------------------------------------------------------------------------
// run_selection_with_cursor_object.c
// メニュー：フラグの一覧画面（カーソルで選んで A で切りかえる）
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
#include "uiflow.h"

struct TextObject {
    u8 storage[12];
};

void UiTextResource_Initialize(struct TextObject *object, s32 *resource);
void UiTextResource_SetPosition(struct TextObject *object, s32 x, s32 y);

//--------------------------------------------------------------------------
// 1 ページ 16x16 のフラグを表示し、閉じるまで入力を受ける
// ページ 8 から始める。戻り値はいつも 0
//--------------------------------------------------------------------------
s32 Menu_RunSelectionWithCursorObject(void)
{
	struct TextObject obj; // カーソルのオブジェクト
	s32 cur[2];	 // カーソルの列・行
	s32 res;
	s32 page;
	s32 win;
	s32 ret;

	page = 8;
	win = 0;
	cur[0] = win;
	cur[1] = win;
	win = UiWindow_Create(1, 0, 28, 20, 2);
	Menu_DrawFlagBitTable(win, page);
	UiTextResource_Initialize(&obj, &res);
	for (;;)
	{
		WaitFrames(1);
		ret = Menu_HandleFlagGridInput(win, &page, cur);
		if (ret == -1)
			break; // 閉じる
		if (ret == 1)
			Menu_DrawFlagBitTable(win, page); // フラグかページが変わった
		UiTextResource_SetPosition(&obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
	}
	UiTextResource_Release(res);
	UiWork_Finalize(win, 2);
	return 0;
}
