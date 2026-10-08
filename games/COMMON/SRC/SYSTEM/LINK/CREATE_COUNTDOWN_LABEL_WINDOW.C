//--------------------------------------------------------------------------
// CREATE_COUNTDOWN_LABEL_WINDOW.C
// 通信：残り時間の見出しのウィンドウを開く
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "TYPES.H"
#include "WINDOW.H"

#define UiText_DrawStringAtOffset ((void (*)(u8 *, s32, s32, s32))UiText_DrawStringAtOffset)

void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

//--------------------------------------------------------------------------
// 左上に 6x4 のウィンドウを開いて見出しを書き、そのハンドルを返す
//--------------------------------------------------------------------------
s32 Link_CreateCountdownLabelWindow(void)
{
	s32 win = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
	UiText_DrawStringAtOffset(Link_TimeLabelString, win, 0, 0);
#else
	UiText_DrawStringInWindow(Link_TimeLabelString, win, 0, 0);
#endif
	return win;
}
