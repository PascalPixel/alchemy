//--------------------------------------------------------------------------
// create_countdown_label_window.c
// Link communication: open the remaining-time label window.
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

#define UiText_DrawStringAtOffset                                              \
  ((void (*)(u8 *, s32, s32, s32))UiText_DrawStringAtOffset)

void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

//--------------------------------------------------------------------------
// Open a 6x4 window at the upper left, draw its label, and return its handle.
//--------------------------------------------------------------------------
s32 Link_CreateCountdownLabelWindow(void) {
  s32 win = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) ||                      \
    defined(TLA_EDITION_IT)
  UiText_DrawStringAtOffset(gLink_TimeLabelString, win, 0, 0);
#else
  UiText_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
  return win;
}
