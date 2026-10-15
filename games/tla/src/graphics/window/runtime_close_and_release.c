//--------------------------------------------------------------------------
// runtime_close_and_release.c
// ウィンドウ：ヒープ 16 番のウィンドウを閉じてブロックを返す
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

//--------------------------------------------------------------------------
// ヒープ 16 番のウィンドウを閉じ、ブロックを返す
//--------------------------------------------------------------------------
void UiWork_CloseAndRelease(void)
{
    UiWork_FinalizeAndReleaseBlock16();
}
