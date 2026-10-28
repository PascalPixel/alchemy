#define OWN_Text_DrawCharacterAtOffsetFar
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

s32 Text_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if (LANGUAGE == LANGUAGE_JAPANESE)
void WindowSprite_ClearListFar();
#define WindowSprite_ClearListFar ((void (*)(s32))WindowSprite_ClearListFar)
#endif

/// Redraws win and writes message from the top left; does nothing if win is
/// 0.
/// The Japanese edition waits one frame after redrawing.
void Shop_DrawMsg(s32 win, s32 message) {
  if (win != 0) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    WindowSprite_RedrawSavedRectFar(win);
    Text_DrawCharacterAtOffsetFar(message, win, 0, 0);
#else
    WindowSprite_ClearListFar(win);
    wait(1);
    Text_DrawMessageFar(message, win, 0, 0);
#endif
  }
}
