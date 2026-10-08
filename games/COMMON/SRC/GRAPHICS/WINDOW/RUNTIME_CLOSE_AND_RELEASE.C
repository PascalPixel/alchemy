//--------------------------------------------------------------------------
// RUNTIME_CLOSE_AND_RELEASE.C
// ウィンドウ：ヒープ 16 番のウィンドウを閉じてブロックを返す
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

//--------------------------------------------------------------------------
// ヒープ 16 番のウィンドウを閉じ、ブロックを返す
//--------------------------------------------------------------------------
void UiWork_CloseAndRelease(void)
{
	UiWork_FinalizeAndReleaseBlock16();
}
