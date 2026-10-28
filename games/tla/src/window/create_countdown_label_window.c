/// @file
/// Link communication: open the remaining-time label window.

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
#include "text_render.h"
#include "types.h"
#include "window.h"
#include "edition.h"

#define Text_DrawStringAtOffset                                              \
  ((void (*)(u8 *, s32, s32, s32))Text_DrawStringAtOffset)

void Text_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

/// Opens a 6x4 window at the upper left, draws its label and returns its
/// handle.
s32 Link_CreateCountdownLabelWindow(void) {
  s32 win = Window_Create(0, 0, 6, 4, 6);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||                      \
    (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||                      \
    (LANGUAGE == LANGUAGE_ITALIAN)
  Text_DrawStringAtOffset(gLink_TimeLabelString, win, 0, 0);
#else
  Text_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
  return win;
}
