#define OWN_UiText_DrawCharacterAtOffsetFar
//--------------------------------------------------------------------------
// draw_message.c
// ショップ：ウィンドウのメッセージを書きかえる
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
#include "edition.h"
#include "far_runtime.h"

s32 UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if !EDITION_INTERNATIONAL
void RenderOutput_ClearListFar();
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)
#endif

//--------------------------------------------------------------------------
// win を描きなおし、メッセージ message を左上から書く（win が 0 なら何もしない）
// 日本版は描きなおしたあと 1 フレーム待つ
//--------------------------------------------------------------------------
void Shop_DrawMsg(s32 win, s32 message)
{
    if (win != 0)
    {
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(win);
        UiText_DrawCharacterAtOffsetFar(message, win, 0, 0);
#else
        RenderOutput_ClearListFar(win);
        WaitFrames(1);
        UiText_DrawResourceFar(message, win, 0, 0);
#endif
    }
}
