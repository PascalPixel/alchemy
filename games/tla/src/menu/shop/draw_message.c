#define OWN_UiText_DrawCharacterAtOffsetFar
//--------------------------------------------------------------------------
// draw_message.c
// Shop: replace a window's message.
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
void RenderOutput_ClearListFar();
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)
#endif

/// Redraw win and write message from the top left; do nothing if win is 0.
/// The Japanese edition waits one frame after redrawing.
void Shop_DrawMsg(s32 win, s32 message) {
  if (win != 0) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    RenderOutput_RedrawSavedRectFar(win);
    UiText_DrawCharacterAtOffsetFar(message, win, 0, 0);
#else
    RenderOutput_ClearListFar(win);
    WaitFrames(1);
    UiText_DrawResourceFar(message, win, 0, 0);
#endif
  }
}
