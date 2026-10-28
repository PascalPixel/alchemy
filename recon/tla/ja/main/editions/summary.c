#include "types.h"

void WindowSprite_PrepareForRedraw(void);
void Window_DrawDividerLine(s32, s32, s32, s32, s32);
void Text_DrawStringAtOffset(void *, s32, s32, s32);
void Text_DrawStringInWindow(s32, s32, s32, s32);
void Text_DrawNumberAtOffset(s32, s32, s32, s32, s32);
void Text_DrawCharacterAtOffset(s32, s32, s32, s32);
void *Text_FormatPlayTime(s32, u8 *);
void Text_DrawNumberInWindow(s32, s32, s32, s32, s32);
extern u8 MsgStatusLabel;
extern u8 MsgClassName;
extern u8 gStatusMenu_LevelLetterString[];
extern u8 MsgCoins[];
extern u8 MsgPiece[];

#if defined(TBS_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

void StatusMenu_DrawCharacterSummary(s32 surface, u8 *st) {
  u32 buf[4];
  s32 extra;

  if (surface != 0) {
    WindowSprite_PrepareForRedraw();
    Window_DrawDividerLine(surface, 0, 4, 13, 4);
    Text_DrawStringAtOffset(st + 16, surface, 0, 0);
    extra = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
    Text_DrawStringAtOffset(gStatusMenu_LevelLetterString, surface,
                              SUMMARY_LEVEL_X, 0);
#else
    Text_DrawStringInWindow((s32)gStatusMenu_LevelLetterString, surface, 72,
                              0);
#endif
    Text_DrawNumberAtOffset(st[28], 2, surface, 80, extra);
    Text_DrawCharacterAtOffset(st[29] + (s32)&MsgClassName, surface, 0, 16);
#if defined(TBS_EDITION_JA)
    extra = 32;
    Text_DrawNumberInWindow(*(s32 *)(st + 36), 6, surface, 24, extra);
    Text_DrawCharacterAtOffset((s32)MsgCoins, surface, 72, 32);
    Text_DrawCharacterAtOffset((s32)&MsgStatusLabel, surface, 0, 48);
    Text_DrawStringInWindow(
        (s32)Text_FormatPlayTime(*(s32 *)(st + 32), (u8 *)buf), surface, 48,
        48);
#else
    Text_DrawCharacterAtOffset((s32)&MsgStatusLabel, surface, 0, 32);
    Text_DrawStringInWindow(
        (s32)Text_FormatPlayTime(*(s32 *)(st + 32), (u8 *)buf), surface, 48,
        40);
    extra = 48;
    Text_DrawNumberInWindow(*(s32 *)(st + 36), 6, surface, 0, extra);
#if defined(TBS_EDITION_FR)
    if (*(u32 *)(st + 36) > 1)
      Text_DrawCharacterAtOffset((s32)MsgCoins, surface, SUMMARY_COINS_X, 48);
    else
      Text_DrawCharacterAtOffset((s32)MsgPiece, surface, SUMMARY_COINS_X, 48);
#else
    Text_DrawCharacterAtOffset((s32)MsgCoins, surface, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}
