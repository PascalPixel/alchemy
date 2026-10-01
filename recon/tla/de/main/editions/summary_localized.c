/* Draft: this summary predates the DE label positions.
 * Its 168-byte complete output has 1 differing bytes over
 * the emitted span. Plain edition branches now match. */
#include "TYPES.H"

void RenderOutput_PrepareForRedraw(void);
void UiWindow_DrawDividerLine(s32, s32, s32, s32, s32);
void UiText_DrawStringAtOffset(void *, s32, s32, s32);
void UiText_DrawStringInWindow(s32, s32, s32, s32);
void UiText_DrawNumberAtOffset(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffset(s32, s32, s32, s32);
s32 Text_FormatPlayTime(s32, void *);
void UiText_DrawNumberInWindow(s32, s32, s32, s32, s32);
extern u8 MsgStatusLabel;
extern u8 MsgClassName;
extern u8 StatusMenu_LevelLetterString[];
extern u8 MsgCoins[];
extern u8 MsgPiece[];

/* The Spanish and French summaries set their level letters at a pixel offset
   further left and push the coins label right, around their longer words. */
#if defined(TBS_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

/* Draws one character entry into the summary surface: its name, the two-digit
 * value at +0x1c, the class message selected by +0x1d, the formatted value at
 * +0x20, the six-digit value at +0x24 and the closing label. */
void StatusMenu_DrawCharacterSummary(s32 surface, u8 *st)
{
    u32 buf[4];
    s32 extra;

    if (surface != 0) {
        RenderOutput_PrepareForRedraw();
        UiWindow_DrawDividerLine(surface, 0, 4, 13, 4);
        UiText_DrawStringAtOffset(st + 16, surface, 0, 0);
        extra = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
        UiText_DrawStringAtOffset(StatusMenu_LevelLetterString, surface, SUMMARY_LEVEL_X, 0);
#else
        UiText_DrawStringInWindow((s32)StatusMenu_LevelLetterString, surface, 72, 0);
#endif
        UiText_DrawNumberAtOffset(st[28], 2, surface, 80, extra);
        UiText_DrawCharacterAtOffset(st[29] + (s32)&MsgClassName, surface, 0, 16);
#if defined(TBS_EDITION_JA) || defined(TLA_EDITION_JA)
        /* The Japanese summary puts the coins above the play time. */
        extra = 32;
        UiText_DrawNumberInWindow(*(s32 *)(st + 36), 6, surface, 24, extra);
        UiText_DrawCharacterAtOffset((s32)MsgCoins, surface, 72, 32);
        UiText_DrawCharacterAtOffset((s32)&MsgStatusLabel, surface, 0, 48);
        UiText_DrawStringInWindow(Text_FormatPlayTime(*(s32 *)(st + 32), buf), surface, 48, 48);
#else
        UiText_DrawCharacterAtOffset((s32)&MsgStatusLabel, surface, 0, 32);
        UiText_DrawStringInWindow(Text_FormatPlayTime(*(s32 *)(st + 32), buf), surface, 48, 40);
        extra = 48;
        UiText_DrawNumberInWindow(*(s32 *)(st + 36), 6, surface, 0, extra);
#if defined(TBS_EDITION_FR)
        /* French names a single coin as one piece. */
        if (*(u32 *)(st + 36) > 1)
            UiText_DrawCharacterAtOffset((s32)MsgCoins, surface, SUMMARY_COINS_X, 48);
        else
            UiText_DrawCharacterAtOffset((s32)MsgPiece, surface, SUMMARY_COINS_X, 48);
#else
        UiText_DrawCharacterAtOffset((s32)MsgCoins, surface, SUMMARY_COINS_X, 48);
#endif
#endif
    }
}
