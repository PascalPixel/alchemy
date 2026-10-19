//--------------------------------------------------------------------------
// draw_four_numbers_in_row.c
// Saves: draw the recorded Djinn counts in a row, one per element.
//--------------------------------------------------------------------------

#include "text_render_runtime.h"
#include "types.h"
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
#include "text/msg_ids.h"

/// The portion of the save record (SaveSummary) read here.
struct NumberRow {
  u8 unk00[0x28];
  s8 values[4]; ///< Djinn count for each element.
};

/// Draws four two-digit numbers 24 pixels apart at y = 16.
/// win: destination; nothing is drawn if zero.
void UiText_DrawFourNumbersInRow(struct RenderInput *win,
                                 struct NumberRow *row) {
  s32 i;

  if (win != 0) {
    for (i = 0; i < 4; i++)
      UiText_DrawNumberAtOffset(row->values[i], 2, win, i * 24, 16);
  }
}

//--------------------------------------------------------------------------

// draw_character_summary.c

// Saves: draw the recorded name, level, class, coins, and play time.

//--------------------------------------------------------------------------

#define UiText_DrawStringAtOffset                                              \
  ((void (*)(void *, s32, s32, s32))UiText_DrawStringAtOffset)
#define UiText_DrawNumberAtOffset                                              \
  ((void (*)(s32, s32, s32, s32, s32))UiText_DrawNumberAtOffset)
#define UiText_DrawCharacterAtOffset                                           \
  ((void (*)(s32, s32, s32, s32))UiText_DrawCharacterAtOffset)

void RenderOutput_PrepareForRedraw(void);
void UiWindow_DrawDividerLine(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(s32, s32, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, s32, s32, s32);

// Edition-specific x positions of the level and coin labels, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_SPANISH)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_FRENCH)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

/// win: destination; do nothing if zero.
/// rec: save record (SaveSummary).
///      +16 name, +28 level, +29 class, +32 play time, +36 coins.
void StatusMenu_DrawCharacterSummary(s32 win, u8 *rec) {
  u32 buf[4]; // Formatted play time.
  s32 y;

  if (win != 0) {
    RenderOutput_PrepareForRedraw();
    UiWindow_DrawDividerLine(win, 0, 4, 13, 4);
    UiText_DrawStringAtOffset(rec + 16, win, 0, 0);
    y = 0;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||                      \
    (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
    UiText_DrawStringAtOffset(gStatusMenu_LevelLetterString, win,
                              SUMMARY_LEVEL_X, 0);
#else
    UiText_DrawStringInWindow((s32)gStatusMenu_LevelLetterString, win, 72, 0);
#endif
    UiText_DrawNumberAtOffset(rec[28], 2, win, 80, y);
    UiText_DrawCharacterAtOffset(rec[29] + (s32)MsgClassName, win, 0, 16);
#if (LANGUAGE == LANGUAGE_JAPANESE)
    y = 32;
    UiText_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 24, y);
    UiText_DrawCharacterAtOffset((s32)MsgCoins, win, 72, 32);
    UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 48);
    UiText_DrawStringInWindow(
        (s32)Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf), win, 48, 48);
#else
    UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 32);
    UiText_DrawStringInWindow(
        (s32)Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf), win, 48, 40);
    y = 48;
    UiText_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 0, y);
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_FRENCH)
    // French uses the singular form for zero or one coin.
    if (*(u32 *)(rec + 36) > 1)
      UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
    else
      UiText_DrawCharacterAtOffset((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
    UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}
