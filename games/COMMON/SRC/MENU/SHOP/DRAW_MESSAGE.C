//--------------------------------------------------------------------------
// DRAW_MESSAGE.C
// ショップ：ウィンドウのメッセージを書きかえる
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
#include "EDITION.H"
#include "FAR_RUNTIME.H"

s32 UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if !EDITION_INTERNATIONAL
void RenderOutput_ClearListFar();
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)
#endif

//--------------------------------------------------------------------------
// win を描きなおし、メッセージ mes を左上から書く（win が 0 なら何もしない）
// 日本版は描きなおしたあと 1 フレーム待つ
//--------------------------------------------------------------------------
void Shop_DrawMsg(s32 win, s32 mes)
{
	if (win != 0)
	{
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(win);
		UiText_DrawCharacterAtOffsetFar(mes, win, 0, 0);
#else
		RenderOutput_ClearListFar(win);
		WaitFrames(1);
		UiText_DrawResourceFar(mes, win, 0, 0);
#endif
	}
}
