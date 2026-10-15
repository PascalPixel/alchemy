//--------------------------------------------------------------------------
// create_countdown_label_window.c
// 通信：残り時間の見出しのウィンドウを開く
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
#include "text_render_runtime.h"
#include "types.h"
#include "window.h"

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
    UiText_DrawStringAtOffset(gLink_TimeLabelString, win, 0, 0);
#else
    UiText_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
    return win;
}
