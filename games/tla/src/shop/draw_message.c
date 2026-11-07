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
#include "version.h"

s32 Text_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if !TLA_PATCH_1
void WindowSprite_ClearListFar();
#define WindowSprite_ClearListFar ((void (*)(s32))WindowSprite_ClearListFar)
#endif

/// Redraws win and writes message from the top left; does nothing if win is
/// 0.
/// The Japanese release clears win and waits a frame; from patch 1 it redraws
/// the window's saved rectangle.
void Shop_DrawMsg(s32 win, s32 message) {
  if (win != 0) {
#if TLA_PATCH_1
    WindowSprite_RedrawSavedRectFar(win);
#else
    WindowSprite_ClearListFar(win);
    wait(1);
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
    Text_DrawCharacterAtOffsetFar(message, win, 0, 0);
#else
    Text_DrawMessageFar(message, win, 0, 0);
#endif
  }
}
